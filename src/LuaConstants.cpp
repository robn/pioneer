#include "mylua.h"
#include "LuaConstants.h"
#include "StarSystem.h"

void LuaConstants::RegisterConstants(lua_State *L)
{
	lua_newtable(L);
	lua_pushinteger(L, SBody::TYPE_GRAVPOINT); lua_setfield(L, -1, "TYPE_GRAVPOINT");
	lua_setfield(L, LUA_GLOBALSINDEX, "SBody");
}
