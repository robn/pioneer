#ifndef _LUA_SUPPORT_H
#define _LUA_SUPPORT_H

#include "lua/lua.h"
#include "mylua.h"
#include "MyLuaMathTypes.h"
#include "vector3.h"
#include "fixed.h"

// support functions for accessing lua from C

static inline bool pi_lua_get_string_attr(lua_State *L, const char *key, std::string &out, const char *def)
{
	bool fail = 0;

	LUA_DEBUG_START(L);
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (lua_isstring(L, -1)) {
		out = lua_tostring(L, -1);
	} else {
		if (def) out = def;
		fail = 1;
	}
	lua_pop(L, 1);
	LUA_DEBUG_END(L, 0);

	return !fail;
}

static inline bool pi_lua_get_int_attr(lua_State *L, const char *key, int &out, int def)
{
	bool fail = 0;

	LUA_DEBUG_START(L);
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (lua_isnumber(L, -1)) {
		out = lua_tointeger(L, -1);
	} else {
		out = def;
		fail = 1;
	}
	lua_pop(L, 1);
	LUA_DEBUG_END(L, 0);

	return !fail;
}

static inline bool pi_lua_get_float_attr(lua_State *L, const char *key, float &out, float def)
{
	bool fail = 0;

	LUA_DEBUG_START(L);
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (lua_isnumber(L, -1)) {
		out = lua_tonumber(L, -1);
	} else {
		out = def;
		fail = 1;
	}
	lua_pop(L, 1);
	LUA_DEBUG_END(L, 0);

	return !fail;
}

static inline bool pi_lua_get_vector_attr(lua_State *L, const char *key, vector3f &out, vector3f def)
{
	bool fail = 0;

	LUA_DEBUG_START(L);
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (MyLuaVec::isVec(L, -1)) {
		out = *MyLuaVec::checkVec(L, -1);
	} else {
		out = def;
		fail = 1;
	}
	lua_pop(L, 1);
	LUA_DEBUG_END(L, 0);

	return !fail;
}

static inline bool pi_lua_get_fixed_attr(lua_State *L, const char *key, fixed &out, fixed def)
{
	bool fail = 0;

	LUA_DEBUG_START(L);
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (MyLuaFixed::isFixed(L, -1)) {
		out = *MyLuaFixed::checkFixed(L, -1);
	} else {
		out = def;
		fail = 1;
	}
	lua_pop(L, 1);
	LUA_DEBUG_END(L, 0);

	return !fail;
}

#endif /* _LUA_SUPPORT_H */
