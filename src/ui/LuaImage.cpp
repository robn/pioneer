// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Image.h"
#include "LuaObject.h"

namespace UI {

class LuaImage {
public:

	static int l_set_image_file(lua_State *l) {
		Image *image = LuaObject<UI::Image>::CheckFromLua(1);
		const std::string filename(luaL_checkstring(l, 2));
		image->SetImageFile(filename);
		lua_pushvalue(l, 1);
		return 1;
	}

	static int l_clear_image(lua_State *l) {
		Image *image = LuaObject<UI::Image>::CheckFromLua(1);
		image->ClearImage();
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
		{ "SetImageFile", LuaImage::l_set_image_file },
		{ "ClearImage",   LuaImage::l_clear_image    },

		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, l_parent, l_methods, 0, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, LuaObject<UI::Image>::DynamicCastPromotionTest);
}
