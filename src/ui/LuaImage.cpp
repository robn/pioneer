// Copyright © 2008-2015 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Image.h"
#include "LuaObject.h"
#include "Color.h"

namespace UI {

class LuaImage {
public:

	static int l_set_tint(lua_State *l) {
		UI::Image *image = LuaObject<UI::Image>::CheckFromLua(1);
		Color c = Color::FromLuaTable(l, 2);
		image->SetTint(c);
		lua_pushvalue(l, 1);
		return 1;
	}

	static int l_set_height_lines(lua_State *l) {
		Image *img = LuaObject<UI::Image>::CheckFromLua(1);
		Uint32 lines = luaL_checkinteger(l, 2);
		img->SetHeightLines(lines);
		lua_pushvalue(l, 1);
		return 1;
	}

};

}

using namespace UI;

template <> const char *LuaObject<UI::Image>::s_type = "UI.Image";

template <> void LuaObject<UI::Image>::RegisterClass()
{
	static const char *l_parent = "UI.Widget";

	static const luaL_Reg l_methods[] = {
		{ "SetTint",        LuaImage::l_set_tint         },
		{ "SetHeightLines", LuaImage::l_set_height_lines },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, l_parent, l_methods, 0, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, LuaObject<UI::Image>::DynamicCastPromotionTest);
}
