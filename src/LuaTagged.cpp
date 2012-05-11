#include "LuaObject.h"
#include "LuaTagged.h"
#include "LuaUtils.h"
#include "Tagged.h"

static int l_tagged_attr_tags(lua_State *l)
{
	lua_newtable(l);
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
