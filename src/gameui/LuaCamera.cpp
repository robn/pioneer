// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "gameui/Camera.h"
#include "LuaObject.h"
#include "LuaBody.h"

namespace GameUI {

class LuaCamera {
public:

	static int l_new(lua_State *l) {
		UI::Context *c = LuaObject<UI::Context>::CheckFromLua(1);
		Body *b = LuaBody::CheckFromLua(2);
		LuaObject<GameUI::Camera>::PushToLua(new GameUI::Camera(c, b));
		return 1;
	}

};

}

using namespace GameUI;

template <> const char *LuaObject<GameUI::Camera>::s_type = "UI.Game.Camera";

template <> void LuaObject<GameUI::Camera>::RegisterClass()
{
	static const char *l_parent = "UI.Single";

	static const luaL_Reg l_methods[] = {
		{ "New", LuaCamera::l_new },
        { 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, l_parent, l_methods, 0, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, LuaObject<GameUI::Camera>::DynamicCastPromotionTest);
}
