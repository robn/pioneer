#include "Tagged.h"
#include "LuaUtils.h"

unsigned int Tagged::AddTagsFromLua(lua_State *l, int stackIndex)
{
	unsigned int count = 0;

	LUA_DEBUG_START(l);

	lua_pushvalue(l, stackIndex);
	lua_pushnil(l);
	while (lua_next(l, -2)) {
		AddTag(luaL_checkstring(l, -1));
		lua_pop(l, 1);
		count++;
	}
	lua_pop(l, 1);

	LUA_DEBUG_END(l, 0);
	
	return count;
}

int Tagged::l_attr_tags(lua_State *l)
{
	lua_newtable(l);
	return 1;
}
