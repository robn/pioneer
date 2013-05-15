// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "LuaSerializer.h"
#include "LuaObject.h"
#include "galaxy/StarSystem.h"
#include "Body.h"
#include "Ship.h"
#include "SpaceStation.h"
#include "Planet.h"
#include "Star.h"
#include "Player.h"
#include "Pi.h"
#include "Game.h"

// every module can save one object. that will usually be a table.  we call
// each serializer in turn and capture its return value we build a table like
// so:
// {
//   'Assassination' = { ... },
//   'DeliverPackage' = { ... },
//   ...
// }
// entire table then gets pickled and handed to the writer
//
// on load, we unpickle the table then call the registered unserialize
// function for each module with its table
//
// we keep a copy of this table around. next time we save we overwrite the
// each individual module's data. that way if a player loads a game with dat
// for a module that is not currently loaded, we don't lose its data in the
// next save


// pickler can handle simple types (boolean, number, string) and will drill
// down into tables. it can do userdata for a specific set of types - Body and
// its kids and SystemPath. anything else will cause a lua error
//
// pickle format is newline-seperated. each line begins with a type value,
// followed by data for that type as follows
//   fNNN.nnn - number (float)
//   bN       - boolean. N is 0 or 1 for true/false
//   sNNN     - string. number is length, followed by newline, then string of bytes
//   t        - table. followed by a float (fNNN.nnn) uniquely identifying the
//            - table, then more pickled stuff (ie recursive)
//   n        - end of table
//   r        - reference to previously-seen table. followed by the table id
//   uXXXX    - userdata. XXXX is type, followed by newline, followed by data
//     Body       - data is a single stringified number for Serializer::LookupBody
//     SystemPath - data is four stringified numbers, newline separated
//   oXXXX    - object. XXX is type, followed by newline, followed by one
//              pickled item (typically t[able])


// on serialize, if an item has a metatable with a "class" attribute, the
// "Serialize" function under that namespace will be called with the type. the
// data returned will then be serialized as an "object" above.
//
// on deserialize, the data after an "object" item will be passed to the
// "Deserialize" function under that namespace. that data returned will be
// given back to the module

Json::Value LuaSerializer::pickle(SaveLoad::SaveContext *sc, lua_State *l, int idx, const std::string &name)
{
	LUA_DEBUG_START(l);

	idx = lua_absindex(l, idx);

	std::string objectClass;

	if (lua_getmetatable(l, idx)) {
		lua_getfield(l, -1, "class");
		if (lua_isnil(l, -1))
			lua_pop(l, 2);

		else {
			objectClass = lua_tostring(l, -1);

			lua_getglobal(l, objectClass.c_str());
			if (!lua_isnil(l, -1))
				lua_getfield(l, -1, "Serialize");
			if (lua_isnil(l, -1))
				luaL_error(l, "No Serialize method found for class '%s'\n", objectClass.c_str());

			lua_pushvalue(l, idx);
			pi_lua_protected_call(l, 1, 1);

			lua_remove(l, idx);
			lua_insert(l, idx);

			lua_pop(l, 3);

			if (lua_isnil(l, idx)) {
				LUA_DEBUG_END(l, 0);
				return Json::Value();
			}
		}
	}

	Json::Value value(Json::nullValue);

	switch (lua_type(l, idx)) {
		case LUA_TNIL:
			break;

		case LUA_TNUMBER: {
			value = Json::Value(Json::objectValue);
			value["type"] = "float";
			value["data"] = SaveLoad::DoubleToHexFloat(lua_tonumber(l, idx));
			break;
		}

		case LUA_TBOOLEAN:
			value = Json::Value(!!lua_toboolean(l, idx));
			break;

		case LUA_TSTRING:
			value = Json::Value(lua_tostring(l, idx));
			break;

		case LUA_TTABLE: {
			value = Json::Value(Json::objectValue);
			value["type"] = "table";

			lua_pushinteger(l, lua_Integer(lua_topointer(l, idx)));         // ptr

			lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");    // ptr reftable
			lua_pushvalue(l, -2);                                           // ptr reftable ptr
			lua_rawget(l, -2);                                              // ptr reftable ???

			if (!lua_isnil(l, -1)) {
				// pointer to previously-seen table
				// assuming that a pointer fits in 64 bits
				value["ptr"] = Json::UInt64(lua_topointer(l, idx));
				lua_pop(l, 3);
			}

			else {

				lua_pushvalue(l, -3);                                       // ptr reftable nil ptr
				lua_pushvalue(l, idx);                                      // ptr reftable nil ptr table
				lua_rawset(l, -4);                                          // ptr reftable nil
				lua_pop(l, 3);                                              // [empty]

				// record ref for pointers
				// assuming that a pointer fits in 64 bits
				value["ref"] = Json::UInt64(lua_topointer(l, idx));

				Json::Value data(Json::arrayValue);

				lua_pushvalue(l, idx);
				lua_pushnil(l);
				while (lua_next(l, -2)) {
					Json::Value item(Json::arrayValue);

					item.append(pickle(sc, l, -2, name));
					item.append(pickle(sc, l, -1, name));

					data.append(item);

					lua_pop(l, 1);
				}
				lua_pop(l, 1);

				value["data"] = data;
			}

			break;
		}

		case LUA_TUSERDATA: {
			LuaObjectBase *lo = static_cast<LuaObjectBase*>(lua_touserdata(l, idx));
			void *o = lo->GetObject();
			if (!o)
				Error("Lua serializer '%s' tried to serialize an invalid object", name.c_str());

			value = Json::Value(Json::objectValue);

			if (lo->Isa("SystemPath")) {
				const SystemPath *sbp = static_cast<const SystemPath*>(o);
				value["type"] = "userdata";
				value["class"] = "SystemPath";
				value["data"] = sbp->Save().GetJson();
			}

			else if (lo->Isa("Body")) {
				Body *b = static_cast<Body*>(o);
				value["type"] = "userdata";
				value["class"] = "Body";
				value["data"] = sc->GetRefId(b);
			}

			else
				Error("Lua serializer '%s' tried to serialize unsupported userdata value", name.c_str());

			break;
		}

		default:
			Error("Lua serializer '%s' tried to serialize %s value", name.c_str(), lua_typename(l, lua_type(l, idx)));
			break;
	}

	LUA_DEBUG_END(l, 0);

	if (objectClass.empty())
		return value;

	Json::Value wrap(Json::objectValue);
	wrap["type"] = "object";
	wrap["class"] = objectClass;
	wrap["data"] = value;
	return wrap;
}

/* XXX DESERIALIZER
const char *LuaSerializer::unpickle(lua_State *l, const char *pos)
{
	LUA_DEBUG_START(l);

	char type = *pos++;

	switch (type) {

		case 'f': {
			char *end;
			double f = strtod(pos, &end);
			if (pos == end) throw SavedGameCorruptException();
			lua_pushnumber(l, f);
			pos = end+1; // skip newline
			break;
		}

		case 'b': {
			if (*pos != '0' && *pos != '1') throw SavedGameCorruptException();
			bool b = (*pos == '0') ? false : true;
			lua_pushboolean(l, b);
			pos++;
			break;
		}

		case 's': {
			char *end;
			int len = strtol(pos, const_cast<char**>(&end), 0);
			if (pos == end) throw SavedGameCorruptException();
			end++; // skip newline
			lua_pushlstring(l, end, len);
			pos = end + len;
			break;
		}

		case 't': {
			lua_newtable(l);

			lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");
			pos = unpickle(l, pos);
			lua_pushvalue(l, -3);
			lua_rawset(l, -3);
			lua_pop(l, 1);

			while (*pos != 'n') {
				pos = unpickle(l, pos);
				pos = unpickle(l, pos);
				lua_rawset(l, -3);
			}
			pos++;

			break;
		}

		case 'r': {
			pos = unpickle(l, pos);

			lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");
			lua_pushvalue(l, -2);
			lua_rawget(l, -2);

			if (lua_isnil(l, -1))
				throw SavedGameCorruptException();

			lua_insert(l, -3);
			lua_pop(l, 2);

			break;
		}

		case 'u': {
			const char *end = strchr(pos, '\n');
			if (!end) throw SavedGameCorruptException();
			int len = end - pos;
			end++; // skip newline

			if (len == 10 && strncmp(pos, "SystemPath", 10) == 0) {
				pos = end;

				Sint32 sectorX = strtol(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				Sint32 sectorY = strtol(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				Sint32 sectorZ = strtol(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				Uint32 systemNum = strtoul(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				Uint32 sbodyId = strtoul(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				const SystemPath sbp(sectorX, sectorY, sectorZ, systemNum, sbodyId);
				LuaObject<SystemPath>::PushToLua(sbp);

				break;
			}

			if (len == 4 && strncmp(pos, "Body", 4) == 0) {
				pos = end;

				Uint32 n = strtoul(pos, const_cast<char**>(&end), 0);
				if (pos == end) throw SavedGameCorruptException();
				pos = end+1; // skip newline

				Body *body = Pi::game->GetSpace()->GetBodyByIndex(n);
				if (pos == end) throw SavedGameCorruptException();

				switch (body->GetType()) {
					case Object::BODY:
						LuaObject<Body>::PushToLua(body);
						break;
					case Object::SHIP:
						LuaObject<Ship>::PushToLua(dynamic_cast<Ship*>(body));
						break;
					case Object::SPACESTATION:
						LuaObject<SpaceStation>::PushToLua(dynamic_cast<SpaceStation*>(body));
						break;
					case Object::PLANET:
						LuaObject<Planet>::PushToLua(dynamic_cast<Planet*>(body));
						break;
					case Object::STAR:
						LuaObject<Star>::PushToLua(dynamic_cast<Star*>(body));
						break;
					case Object::PLAYER:
						LuaObject<Player>::PushToLua(dynamic_cast<Player*>(body));
						break;
					default:
						throw SavedGameCorruptException();
				}

				break;
			}

			throw SavedGameCorruptException();
		}

		case 'o': {
			const char *end = strchr(pos, '\n');
			if (!end) throw SavedGameCorruptException();
			int len = end - pos;
			end++; // skip newline

			const char *cl = pos;

			// unpickle the object, and insert it beneath the method table value
			pos = unpickle(l, end);

			// get _G[typename]
			lua_rawgeti(l, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
			lua_pushlstring(l, cl, len);
			lua_gettable(l, -2);
			lua_remove(l, -2);

			if (lua_isnil(l, -1)) {
				lua_pop(l, 2);
				break;
			}

			lua_getfield(l, -1, "Unserialize");
			if (lua_isnil(l, -1)) {
				lua_pushlstring(l, cl, len);
				luaL_error(l, "No Unserialize method found for class '%s'\n", lua_tostring(l, -1));
			}

			lua_insert(l, -3);
			lua_pop(l, 1);

			pi_lua_protected_call(l, 1, 1);

			break;
		}

		default:
			throw SavedGameCorruptException();
	}

	LUA_DEBUG_END(l, 1);

	return pos;
}
*/

SaveLoad::Object LuaSerializer::Save(SaveLoad::SaveContext *sc) const
{
	lua_State *l = Lua::manager->GetLuaState();

	LUA_DEBUG_START(l);

	lua_newtable(l);
	int savetable = lua_gettop(l);

	lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	if (lua_isnil(l, -1)) {
		lua_pop(l, 1);
		lua_newtable(l);
		lua_pushvalue(l, -1);
		lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	}

	lua_pushnil(l);
	while (lua_next(l, -2) != 0) {
		lua_pushinteger(l, 1);
		lua_gettable(l, -2);
		pi_lua_protected_call(l, 0, 1);
		lua_pushvalue(l, -3);
		lua_insert(l, -2);
		lua_settable(l, savetable);
		lua_pop(l, 1);
	}

	lua_pop(l, 1);

	lua_newtable(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");

	SaveLoad::Object so;

	lua_pushnil(l);
	while (lua_next(l, savetable)) {
		const std::string name(lua_tostring(l, -2));
		so.Set(name, pickle(sc, l, -1, name));
		lua_pop(l, 1);
	}

	lua_pop(l, 1);

	lua_pushnil(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");

	LUA_DEBUG_END(l, 0);

	return so;
}

/* XXX DESERIALIZER
void LuaSerializer::Unserialize(SaveLoad::Reader &rd)
{
	lua_State *l = Lua::manager->GetLuaState();

	LUA_DEBUG_START(l);

	lua_newtable(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");

	std::string pickled = rd.String();
	const char *start = pickled.c_str();
	const char *end = unpickle(l, start);
	if (size_t(end - start) != pickled.length()) throw SavedGameCorruptException();
	if (!lua_istable(l, -1)) throw SavedGameCorruptException();
	int savetable = lua_gettop(l);

	lua_pushnil(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerTableRefs");

	lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	if (lua_isnil(l, -1)) {
		lua_pop(l, 1);
		lua_newtable(l);
		lua_pushvalue(l, -1);
		lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	}

	lua_pushnil(l);
	while (lua_next(l, -2) != 0) {
		lua_pushvalue(l, -2);
		lua_pushinteger(l, 2);
		lua_gettable(l, -3);
		lua_getfield(l, savetable, lua_tostring(l, -2));
		if (lua_isnil(l, -1)) {
			lua_pop(l, 1);
			lua_newtable(l);
		}
		pi_lua_protected_call(l, 1, 0);
		lua_pop(l, 2);
	}

	lua_pop(l, 2);

	LUA_DEBUG_END(l, 0);
}
*/

int LuaSerializer::l_register(lua_State *l)
{
	LUA_DEBUG_START(l);

	std::string key = luaL_checkstring(l, 2);

	luaL_checktype(l, 3, LUA_TFUNCTION); // any type of function
	luaL_checktype(l, 4, LUA_TFUNCTION); // any type of function

	lua_getfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	if (lua_isnil(l, -1)) {
		lua_pop(l, 1);
		lua_newtable(l);
		lua_pushvalue(l, -1);
		lua_setfield(l, LUA_REGISTRYINDEX, "PiSerializerCallbacks");
	}

	lua_getfield(l, -1, key.c_str());
	if(!(lua_isnil(l, -1)))
		luaL_error(l, "Lua serializer functions for '%s' are already registered\n", key.c_str());
	lua_pop(l, 1);

	lua_newtable(l);

	lua_pushinteger(l, 1);
	lua_pushvalue(l, 3);
	lua_rawset(l, -3);
	lua_pushinteger(l, 2);
	lua_pushvalue(l, 4);
	lua_rawset(l, -3);

	lua_pushstring(l, key.c_str());
	lua_pushvalue(l, -2);
	lua_rawset(l, -4);

	lua_pop(l, 2);

	LUA_DEBUG_END(l, 0);

	return 0;
}

template <> const char *LuaObject<LuaSerializer>::s_type = "Serializer";

template <> void LuaObject<LuaSerializer>::RegisterClass()
{
	static const luaL_Reg l_methods[] = {
		{ "Register", LuaSerializer::l_register },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, 0, l_methods, 0, 0);
}
