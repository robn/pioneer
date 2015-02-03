// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "LuaRPCAgent.h"
#include "LuaObject.h"
#include "LuaRef.h"
#include "RPCAgent.h"
#include "Pi.h"
#include <json/json.h>

struct CallbackPair {
	CallbackPair(lua_State *l, int successIndex, int failIndex) :
		lua(l),
		successCallback(l, successIndex),
		failCallback(l, failIndex)
		{}
	lua_State *lua;
	LuaRef successCallback;
	LuaRef failCallback;
};

static Json::Value _lua_to_json(lua_State *l, int idx)
{
	int data = lua_absindex(l, idx);

	switch (lua_type(l, data)) {
		case LUA_TNIL:
			return Json::Value();
			
		case LUA_TNUMBER:
			return Json::Value(lua_tonumber(l, data));

		case LUA_TBOOLEAN:
			return Json::Value(lua_toboolean(l, data));

		case LUA_TSTRING:
			return Json::Value(lua_tostring(l, data));

		case LUA_TTABLE: {

			// XXX handle arrays

			Json::Value object(Json::objectValue);

			lua_pushnil(l);
			while (lua_next(l, data)) {
				const std::string key(luaL_checkstring(l, -2));
				Json::Value value(_lua_to_json(l, -1));
				object[key] = value;
				lua_pop(l, 1);
			}

			return object;
		}

		default:
			luaL_error(l, "can't convert Lua type %s to JSON", lua_typename(l, lua_type(l, idx)));
			return Json::Value();
	}

	// shouldn't get here
	assert(0);

	return Json::Value();
}

static void _json_to_lua(lua_State *l, const Json::Value &data)
{
	LUA_DEBUG_START(l);

	switch (data.type()) {
		case Json::nullValue:
			lua_pushnil(l);
			break;

		case Json::intValue:
		case Json::uintValue:
		case Json::realValue:
			lua_pushnumber(l, data.asDouble());
			break;

		case Json::stringValue: {
			const std::string &str(data.asString());
			lua_pushlstring(l, str.c_str(), str.size());
			break;
		}

		case Json::booleanValue:
			lua_pushboolean(l, data.asBool());
			break;

		case Json::arrayValue: {
			lua_newtable(l);
			for (int i = 0; i < int(data.size()); i++) {
				lua_pushinteger(l, i+1);
				_json_to_lua(l, data[i]);
				lua_rawset(l, -3);
			}
		}

		case Json::objectValue: {
			lua_newtable(l);
			for (Json::Value::const_iterator i = data.begin(); i != data.end(); ++i) {
				const std::string &key(i.key().asString());
				lua_pushlstring(l, key.c_str(), key.size());
				_json_to_lua(l, *i);
				lua_rawset(l, -3);
			}
		}
	}

	LUA_DEBUG_END(l, 1);
}

static void _success_callback(const Json::Value &data, void *userdata)
{
	CallbackPair *cp = reinterpret_cast<CallbackPair*>(userdata);
	if (!cp->successCallback.IsValid()) return;

	cp->successCallback.PushCopyToStack();

	_json_to_lua(cp->lua, data);
	pi_lua_protected_call(cp->lua, 1, 0);

	delete cp;
}

static void _fail_callback(const std::string &error, void *userdata)
{
	CallbackPair *cp = reinterpret_cast<CallbackPair*>(userdata);
	if (!cp->failCallback.IsValid()) return;

	cp->failCallback.PushCopyToStack();

	lua_pushlstring(cp->lua, error.c_str(), error.size());
	pi_lua_protected_call(cp->lua, 1, 0);

	delete cp;
}

static int l_rpcagent_call(lua_State *l)
{
	const std::string method(luaL_checkstring(l, 1));
	const Json::Value data(_lua_to_json(l, 2));

	int successIndex = LUA_NOREF, failIndex = LUA_NOREF;

	if (lua_gettop(l) > 2) {
		luaL_checktype(l, 3, LUA_TFUNCTION);
		successIndex = 3;
	}
	if (lua_gettop(l) > 3) {
		luaL_checktype(l, 4, LUA_TFUNCTION);
		failIndex = 4;
	}

	CallbackPair *cp = new CallbackPair(l, successIndex, failIndex);
		
	Pi::rpcAgent->Call(method, data, sigc::ptr_fun(_success_callback), sigc::ptr_fun(_fail_callback), cp);

	return 0;
}

void LuaRPCAgent::Register()
{
	static const luaL_Reg l_methods[] = {
		{ "Call", l_rpcagent_call },
		{ 0, 0 }
	};

	LuaObjectBase::CreateObject(l_methods, 0, 0);
	lua_setglobal(Lua::manager->GetLuaState(), "RPCAgent");
}
