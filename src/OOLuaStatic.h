#ifndef _OOLUASTATIC_H
#define _OOLUASTATIC_H

#include "oolua/oolua.h"

#define STATIC_DISPATCH_START(cl,name) \
    static int name(lua_State *l) { const char* _cl = #cl; const char* _name = #name; const int _n = lua_gettop(l)-1;
#define STATIC_FUNC(n,def) if (_n == n) { def }
#define STATIC_FUNC_VA(n,def) if (_n >= n) { _static_varargs_table(l,n); def }
#define STATIC_DISPATCH_END _static_dispatch_fail(l,_cl,_name); return 0; }

#define STATIC_FUNC_0(rt1,fn) \
	STATIC_FUNC(0, OOLUA_C_FUNCTION_0(rt1,fn))
#define STATIC_FUNC_1(rt1,fn,t1) \
	STATIC_FUNC(1, OOLUA_C_FUNCTION_1(rt1,fn,t1))
#define STATIC_FUNC_2(rt1,fn,t1,t2) \
	STATIC_FUNC(2, OOLUA_C_FUNCTION_2(rt1,fn,t1,t2))
#define STATIC_FUNC_3(rt1,fn,t1,t2,t3) \
	STATIC_FUNC(3, OOLUA_C_FUNCTION_3(rt1,fn,t1,t2,t3))
#define STATIC_FUNC_4(rt1,fn,t1,t2,t3,t4) \
	STATIC_FUNC(4, OOLUA_C_FUNCTION_4(rt1,fn,t1,t2,t3,t4))
#define STATIC_FUNC_5(rt1,fn,t1,t2,t3,t4,t5) \
	STATIC_FUNC(5, OOLUA_C_FUNCTION_5(rt1,fn,t1,t2,t3,t4,t5))
#define STATIC_FUNC_6(rt1,fn,t1,t2,t3,t4,t5,t6) \
	STATIC_FUNC(6, OOLUA_C_FUNCTION_6(rt1,fn,t1,t2,t3,t4,t5,t6))

#define STATIC_FUNC_0_VA(rt1,fn) \
	STATIC_FUNC_VA(0, OOLUA_C_FUNCTION_2(rt1,fn,OOLUA::Lua_table,int))
#define STATIC_FUNC_1_VA(rt1,fn,t1) \
	STATIC_FUNC_VA(1, OOLUA_C_FUNCTION_3(rt1,fn,t1,OOLUA::Lua_table,int))
#define STATIC_FUNC_2_VA(rt1,fn,t1,t2) \
	STATIC_FUNC_VA(2, OOLUA_C_FUNCTION_4(rt1,fn,t1,t2,OOLUA::Lua_table,int))
#define STATIC_FUNC_3_VA(rt1,fn,t1,t2,t3) \
	STATIC_FUNC_VA(3, OOLUA_C_FUNCTION_5(rt1,fn,t1,t2,t3,OOLUA::Lua_table,int))
#define STATIC_FUNC_4_VA(rt1,fn,t1,t2,t3,t4) \
	STATIC_FUNC_VA(4, OOLUA_C_FUNCTION_6(rt1,fn,t1,t2,t3,t4,OOLUA::Lua_table,int))
#define STATIC_FUNC_5_VA(rt1,fn,t1,t2,t3,t4,t5) \
	STATIC_FUNC_VA(5, OOLUA_C_FUNCTION_7(rt1,fn,t1,t2,t3,t4,t5,OOLUA::Lua_table,int))
#define STATIC_FUNC_6_VA(rt1,fn,t1,t2,t3,t4,t5,t6) \
	STATIC_FUNC_VA(6, OOLUA_C_FUNCTION_8(rt1,fn,t1,t2,t3,t4,t5,t6,OOLUA::Lua_table,int))

static inline void _static_varargs_table(lua_State *l, int n)
{
	int i;
	for (i=n-(lua_gettop(l)-1); i<-1; i++)
		lua_insert(l, i);
	lua_newtable(l);
	for (i=1; lua_gettop(l)-2 > n; i++) {
		lua_insert(l, -2);
		lua_pushinteger(l, i);
		lua_insert(l, -2);
		lua_settable(l, -3);
	}
	lua_pushinteger(l, i-1);
}

static inline void _static_dispatch_fail(lua_State *l, const char* cl, const char *name)
{
	luaL_error(l, "no function found for %s:%s (%d arguments)", cl, name, lua_gettop(l)-1);
}

#endif
