#ifndef _LUA_SUPPORT_H
#define _LUA_SUPPORT_H

#include "lua/lua.h"
#include "mylua.h"
#include "MyLuaMathTypes.h"

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

#endif /* _LUA_SUPPORT_H */
