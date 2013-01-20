// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Model.h"
#include "LuaObject.h"
#include "Pi.h"
#include "graphics/TextureBuilder.h"
#include "StringF.h"

namespace SceneGraph {

class LuaModel {
public:

	static int l_set_colors(lua_State *l)
	{
		SceneGraph::Model *m = LuaObject<SceneGraph::Model>::CheckFromLua(1);
		std::vector<Color4ub> colors(m->GetColors());

		luaL_checktype(l, 2, LUA_TTABLE);

		lua_getfield(l, 2, "primary");
		if (lua_istable(l, -1))
			colors[0] = Color4ub(Color4f::FromLuaTable(l, -1));

		lua_getfield(l, 2, "secondary");
		if (lua_istable(l, -1))
			colors[1] = Color4ub(Color4f::FromLuaTable(l, -1));

		lua_getfield(l, 2, "trim");
		if (lua_istable(l, -1))
			colors[2] = Color4ub(Color4f::FromLuaTable(l, -1));

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
		SceneGraph::Model *m = LuaObject<SceneGraph::Model>::CheckFromLua(1);
		const std::string name(luaL_checkstring(l, 2));
		int num = lua_tointeger(l, 3);

		Graphics::Texture *t = Graphics::TextureBuilder(stringf("textures/decals/%0.png", name), Graphics::LINEAR_CLAMP, true, true, false).GetOrCreateTexture(Pi::renderer, "model");
		m->SetDecalTexture(t, num);

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

