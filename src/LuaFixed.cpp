#include "LuaFixed.h"

static int l_fixed_new(lua_State *l)
{
	Sint64 num = luaL_checkinteger(l, 1);
	Sint64 denom = luaL_checkinteger(l, 2);
	fixed *f = new fixed(num, denom);
	LuaFixed::PushToLuaGC(f);
	return 1;
}

static int l_fixed_to_number(lua_State *l)
{
	fixed *f = LuaFixed::GetFromLua(1);
	lua_pushnumber(l, f->ToDouble());
	return 1;
}

static int l_fixed_meta_add(lua_State *l)
{
	fixed *r;

	fixed *f1 = LuaFixed::CheckFromLua(1);
	fixed *f2 = LuaFixed::CheckFromLua(2);

	if (f1 && f2)
		r = new fixed(*f1 + *f2);
	
	else if (f1) {
		Sint64 n = luaL_checkinteger(l, 2);
		r = new fixed(*f1 + n);
	}

	else {
		Sint64 n = luaL_checkinteger(l, 1);
		r = new fixed(n + *f2);
	}

	LuaFixed::PushToLuaGC(r);
	return 1;
}

static int l_fixed_meta_sub(lua_State *l)
{
	fixed *r;

	fixed *f1 = LuaFixed::CheckFromLua(1);
	fixed *f2 = LuaFixed::CheckFromLua(2);

	if (f1 && f2)
		r = new fixed(*f1 - *f2);
	
	else if (f1) {
		Sint64 n = luaL_checkinteger(l, 2);
		r = new fixed(*f1 - n);
	}

	else {
		Sint64 n = luaL_checkinteger(l, 1);
		r = new fixed(n - *f2);
	}

	LuaFixed::PushToLuaGC(r);
	return 1;
}

static int l_fixed_meta_mul(lua_State *l)
{
	fixed *r;

	fixed *f1 = LuaFixed::CheckFromLua(1);
	fixed *f2 = LuaFixed::CheckFromLua(2);

	if (f1 && f2)
		r = new fixed(*f1 * *f2);
	
	else if (f1) {
		Sint64 n = luaL_checkinteger(l, 2);
		r = new fixed(*f1 * n);
	}

	else {
		Sint64 n = luaL_checkinteger(l, 1);
		r = new fixed(n * *f2);
	}

	LuaFixed::PushToLuaGC(r);
	return 1;
}

static int l_fixed_meta_div(lua_State *l)
{
	fixed *r;

	fixed *f1 = LuaFixed::CheckFromLua(1);
	fixed *f2 = LuaFixed::CheckFromLua(2);

	if (f1 && f2)
		r = new fixed(*f1 / *f2);
	
	else if (f1) {
		Sint64 n = luaL_checkinteger(l, 2);
		r = new fixed(*f1 / n);
	}

	else {
		Sint64 n = luaL_checkinteger(l, 1);
		r = new fixed(n / *f2);
	}

	LuaFixed::PushToLuaGC(r);
	return 1;
}

static int l_fixed_meta_eq(lua_State *l)
{
	fixed *f1 = LuaFixed::GetFromLua(1);
	fixed *f2 = LuaFixed::GetFromLua(2);
	lua_pushboolean(l, *f1 == *f2);
	return 1;
}

static int l_fixed_meta_le(lua_State *l)
{
	fixed *f1 = LuaFixed::GetFromLua(1);
	fixed *f2 = LuaFixed::GetFromLua(2);
	lua_pushboolean(l, *f1 <= *f2);
	return 1;
}

static int l_fixed_meta_lt(lua_State *l)
{
	fixed *f1 = LuaFixed::GetFromLua(1);
	fixed *f2 = LuaFixed::GetFromLua(2);
	lua_pushboolean(l, *f1 < *f2);
	return 1;
}

template <> const char *LuaObject<LuaUncopyable<fixed> >::s_type = "Fixed";

template <> void LuaObject<LuaUncopyable<fixed> >::RegisterClass()
{
	static const luaL_reg l_methods[] = {
		{ "New",      l_fixed_new       },
		{ "ToNumber", l_fixed_to_number },
		{ 0, 0 }
	};

	static const luaL_reg l_meta[] = {
		{ "__add", l_fixed_meta_add },
		{ "__sub", l_fixed_meta_sub },
		{ "__mul", l_fixed_meta_mul },
		{ "__div", l_fixed_meta_div },
		{ "__eq",  l_fixed_meta_eq  },
		{ "__le",  l_fixed_meta_le  },
		{ "__lt",  l_fixed_meta_lt  },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, NULL, l_methods, NULL, l_meta);
}
