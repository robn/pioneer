// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Model.h"
#include "LuaObject.h"
#include "Pi.h"

namespace SceneGraph {

class LuaModel {
public:

	static int l_set_colors(lua_State *l)
	{
		SceneGraph::Model *m = LuaObject<SceneGraph::Model>::CheckFromLua(1);
		luaL_checktype(l, 2, LUA_TTABLE);
		std::vector<Color4ub> colors;
		lua_getfield(l, 2, "primary");
		luaL_checktype(l, -1, LUA_TTABLE);
		colors.push_back(Color4ub(Color4f::FromLuaTable(l, -1)));
		lua_getfield(l, 2, "secondary");
		luaL_checktype(l, -1, LUA_TTABLE);
		colors.push_back(Color4ub(Color4f::FromLuaTable(l, -1)));
		lua_getfield(l, 2, "trim");
		luaL_checktype(l, -1, LUA_TTABLE);
		colors.push_back(Color4ub(Color4f::FromLuaTable(l, -1)));
		lua_pop(l, 3);
		m->SetColors(colors);
		return 0;
	}

	static int l_set_pattern(lua_State *l)
	{
		return 0;
	}

	static int l_set_decal(lua_State *l)
	{
		return 0;
	}

	static int l_attr_colors(lua_State *l)
	{
		SceneGraph::Model *m = LuaObject<SceneGraph::Model>::CheckFromLua(1);
		const std::vector<Color4ub> &colors = m->GetColors();
		lua_newtable(l);
		colors[0].ToColor4f().ToLuaTable(l);
		lua_setfield(l, -2, "primary");
		colors[1].ToColor4f().ToLuaTable(l);
		lua_setfield(l, -2, "secondary");
		colors[2].ToColor4f().ToLuaTable(l);
		lua_setfield(l, -2, "trim");
		return 1;
	}

	static int l_attr_pattern(lua_State *l)
	{
		return 0;
	}

	static int l_attr_decal(lua_State *l)
	{
		return 0;
	}

};

}

using namespace SceneGraph;

template <> const char *LuaObject<SceneGraph::Model>::s_type = "SceneGraph.Model";

template <> void LuaObject<SceneGraph::Model>::RegisterClass()
{
	static const luaL_Reg l_methods[] = {
		{ "SetColors",  LuaModel::l_set_colors  },
		{ "SetPattern", LuaModel::l_set_pattern },
		{ "SetDecal",   LuaModel::l_set_decal   },
		{ 0, 0 }
	};

	static const luaL_Reg l_attrs[] = {
		{ "colors",  LuaModel::l_attr_colors  },
		{ "pattern", LuaModel::l_attr_pattern },
		{ "decal",   LuaModel::l_attr_decal   },
		{ 0, 0 }
	};

	LuaObjectBase::CreateClass(s_type, 0, l_methods, l_attrs, 0);
}

