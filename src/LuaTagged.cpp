#include "LuaObject.h"
#include "LuaTagged.h"
#include "LuaUtils.h"
#include "Tagged.h"

static int l_tagged_attr_tags(lua_State *l)
{
	LUA_DEBUG_START(l);

	Tagged *t = LuaTagged::CheckFromLua(1);

	lua_newtable(l);

	for (Tagged::TagIterator i = t->TagsBegin(); i != t->TagsEnd(); ++i) {
		lua_pushinteger(l, lua_rawlen(l, -1) +1);
		lua_pushstring(l, (*i).c_str());
		lua_rawset(l, -3);
	}

	LUA_DEBUG_END(l, 1);

	return 1;
}

template <> const char *LuaObject<LuaUncopyable<Tagged> >::s_type = "Tagged";

template <> void LuaObject<LuaUncopyable<Tagged> >::RegisterClass()
{
	static const luaL_Reg l_attrs[] = {
		{ "tags", l_tagged_attr_tags },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, 0, 0, 0, l_attrs, 0);
}
