#include "LuaObject.h"
#include "LuaSector.h"
#include "LuaUtils.h"

static int l_sector_new(lua_State *l)
{
	int x = luaL_checkinteger(l, 1);
	int y = luaL_checkinteger(l, 2);
	int z = luaL_checkinteger(l, 3);
	Sector *s = new Sector(x, y, z);
	LuaSector::PushToLuaGC(s);
	return 1;
}

static int l_sector_get_systems(lua_State *l)
{
	Sector *s = LuaSector::GetFromLua(1);

	LUA_DEBUG_START(l);

	lua_newtable(l);

	for (std::vector<Sector::System>::iterator i = s->m_systems.begin(); i != s->m_systems.end(); i++) {
		lua_pushinteger(l, lua_objlen(l, -1)+1);

		lua_newtable(l);
		pi_lua_settable(l, "name", (*i).name.c_str());

		lua_pushstring(l, "pos");
		lua_newtable(l);
		pi_lua_settable(l, "x", (*i).p.x);
		pi_lua_settable(l, "y", (*i).p.y);
		pi_lua_settable(l, "z", (*i).p.z);
		lua_rawset(l, -3);

		lua_pushstring(l, "stars");
		lua_newtable(l);
		for (int j = 0; j < (*i).numStars; j++) {
			lua_pushinteger(l, lua_objlen(l, -1)+1);
			lua_pushstring(l, LuaConstants::GetConstantString(l, "BodyType", (*i).starType[j]));
			lua_rawset(l, -3);
		}
		lua_rawset(l, -3);

		lua_rawset(l, -3);
	}

	LUA_DEBUG_END(l, 1);

	return 1;
}

static int l_sector_attr_x(lua_State *l)
{
	Sector *s = LuaSector::GetFromLua(1);
	lua_pushinteger(l, s->sx);
	return 1;
}

static int l_sector_attr_y(lua_State *l)
{
	Sector *s = LuaSector::GetFromLua(1);
	lua_pushinteger(l, s->sy);
	return 1;
}

static int l_sector_attr_z(lua_State *l)
{
	Sector *s = LuaSector::GetFromLua(1);
	lua_pushinteger(l, s->sz);
	return 1;
}

template <> const char *LuaObject<Sector>::s_type = "Sector";

template <> void LuaObject<Sector>::RegisterClass()
{
	static luaL_reg l_methods[] = {
		{ "New",        l_sector_new         },
		{ "GetSystems", l_sector_get_systems },
		{ 0, 0 }
	};

	static luaL_reg l_attrs[] = {
		{ "x", l_sector_attr_x },
		{ "y", l_sector_attr_y },
		{ "z", l_sector_attr_z },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, NULL, l_methods, l_attrs, NULL);
}
