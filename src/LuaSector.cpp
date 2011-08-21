#include "LuaObject.h"
#include "LuaSector.h"

static int l_sector_new(lua_State *l)
{
	int x = luaL_checkinteger(l, 1);
	int y = luaL_checkinteger(l, 2);
	int z = luaL_checkinteger(l, 3);
	Sector *s = new Sector(x, y, z);
	LuaSector::PushToLuaGC(s);
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
		{ "New", l_sector_new },
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
