// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "TabGroup.h"
#include "Lua.h"
#include "LuaConstants.h"
#include "LuaSignal.h"

namespace UI {

class LuaTabGroup {

public:

	static int l_new_tab(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		auto w = Lua::CheckWidget(tg->GetContext(), l, 2);
		auto tab = tg->NewTab(w);
		if (!lua_isnoneornil(l, 3)) {
			auto inner = UI::Lua::GetWidget(tg->GetContext(), l, 3);
			if (inner)
				tab->SetInnerWidget(inner);
		}
		LuaObject<UI::TabGroup::Tab>::PushToLua(tab);
		return 1;
	}

	static int l_remove_tab(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		auto tab = LuaObject<UI::TabGroup::Tab>::CheckFromLua(2);
		tg->RemoveTab(tab);
		return 0;
	}

	static int l_select_tab(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		auto tab = LuaObject<UI::TabGroup::Tab>::CheckFromLua(2);
		tg->SelectTab(tab);
		return 0;
	}

	static int l_set_header_collapsible(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		bool collapsible = lua_toboolean(l, 2);
		tg->SetHeaderCollapsible(collapsible);
		lua_pushvalue(l, 1);
		return 1;
	}

	static int l_set_transparent(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		bool transparent = lua_toboolean(l, 2);
		tg->SetTransparent(transparent);
		lua_pushvalue(l, 1);
		return 1;
	}

	static int l_set_collapsed(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		bool collapsed = lua_toboolean(l, 2);
		tg->SetCollapsed(collapsed);
		return 0;
	}

	static int l_toggle_collapsed(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		tg->ToggleCollapsed();
		return 0;
	}

	static int l_attr_collapsed(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		lua_pushboolean(l, tg->IsCollapsed());
		return 1;
	}

	static int l_attr_on_collapse(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		LuaSignal<>().Wrap(l, tg->onCollapse);
		return 1;
	}

	static int l_attr_on_expand(lua_State *l) {
		auto tg = LuaObject<UI::TabGroup>::CheckFromLua(1);
		LuaSignal<>().Wrap(l, tg->onExpand);
		return 1;
	}


class Tab {
public:

	static int l_attr_header_widget(lua_State *l) {
		auto tab = LuaObject<UI::TabGroup::Tab>::CheckFromLua(1);
		auto w = tab->GetHeaderWidget();
		LuaObject<UI::Widget>::PushToLua(w);
		return 1;
	}

	static int l_attr_on_select(lua_State *l) {
		auto tab = LuaObject<UI::TabGroup::Tab>::CheckFromLua(1);
		LuaSignal<>().Wrap(l, tab->onSelect);
		return 1;
	}

};

};

}

using namespace UI;

template <> const char *LuaObject<UI::TabGroup>::s_type = "UI.TabGroup";

template <> void LuaObject<UI::TabGroup>::RegisterClass()
{
	static const char *l_parent = "UI.Container";

	static const luaL_Reg l_methods[] = {
		{ "NewTab",               UI::LuaTabGroup::l_new_tab                },
		{ "RemoveTab",            UI::LuaTabGroup::l_remove_tab             },
		{ "SelectTab",            UI::LuaTabGroup::l_select_tab             },
		{ "SetTransparent",       UI::LuaTabGroup::l_set_transparent        },
		{ "SetHeaderCollapsible", UI::LuaTabGroup::l_set_header_collapsible },
		{ "SetCollapsed",         UI::LuaTabGroup::l_set_collapsed          },
		{ "ToggleCollapsed",      UI::LuaTabGroup::l_toggle_collapsed       },
		{ 0, 0 }
	};

	static const luaL_Reg l_attrs[] = {
		{ "collapsed",            UI::LuaTabGroup::l_attr_collapsed         },
		{ "onCollapse",           UI::LuaTabGroup::l_attr_on_collapse       },
		{ "onExpand",             UI::LuaTabGroup::l_attr_on_expand         },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, l_parent, l_methods, l_attrs, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, LuaObject<UI::TabGroup>::DynamicCastPromotionTest);
}

template <> const char *LuaObject<UI::TabGroup::Tab>::s_type = "UI.TabGroup.Tab";

template <> void LuaObject<UI::TabGroup::Tab>::RegisterClass()
{
	static const char *l_parent = "UI.Single";

	static const luaL_Reg l_attrs[] = {
		{ "headerWidget", UI::LuaTabGroup::Tab::l_attr_header_widget },
		{ "onSelect",     UI::LuaTabGroup::Tab::l_attr_on_select     },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, l_parent, 0, l_attrs, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, LuaObject<UI::TabGroup::Tab>::DynamicCastPromotionTest);
}
