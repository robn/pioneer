#ifndef _MYLUAMATHTYPES_H
#define _MYLUAMATHTYPES_H

#include "mylua.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "fixed.h"

#define MYLUA_VEC "Vec"
#define MYLUA_MATRIX "Matrix"
#define MYLUA_FIXED "Fixed"

namespace MyLuaVec {
	int Vec_new(lua_State *L);
	int Vec_newNormalized(lua_State *L);
	bool isVec (lua_State *L, int index);
	vector3f *checkVec (lua_State *L, int index);
	vector3f *pushVec(lua_State *L);
	int Vec_register (lua_State *L);
}; /* namespace MyLuaVec */

namespace MyLuaMatrix {
	matrix4x4f *checkMatrix (lua_State *L, int index);
	matrix4x4f *pushMatrix(lua_State *L);
	int Matrix_register (lua_State *L);
}; /* namespace MyLuaMatrix */

namespace MyLuaFixed {
	int Fixed_new (lua_State *L);
	bool isFixed (lua_State *L, int index);
	fixed *checkFixed (lua_State *L, int index);
	fixed *pushFixed(lua_State *L);
	int Fixed_register (lua_State *L);
}; /* namespace MuLuaFixed */

// i'll just throw them in here...
static inline const char *luaPi_gettable_checkstring(lua_State *L, int table, int idx)
{
	lua_pushinteger(L, idx);
	lua_gettable(L, (table<0 ? table-1 : table));
	const char *r = luaL_checkstring(L, -1);
	lua_pop(L, 1);
	return r;
}

static inline double luaPi_gettable_checknumber(lua_State *L, int table, int idx)
{
	lua_pushinteger(L, idx);
	lua_gettable(L, (table<0 ? table-1 : table));
	double num = luaL_checknumber(L, -1);
	lua_pop(L, 1);
	return num;
}

static inline int luaPi_gettable_checkinteger(lua_State *L, int table, int idx)
{
	lua_pushinteger(L, idx);
	lua_gettable(L, (table<0 ? table-1 : table));
	int num = luaL_checkinteger(L, -1);
	lua_pop(L, 1);
	return num;
}
#endif /* _MYLUAMATHTYPES_H */
