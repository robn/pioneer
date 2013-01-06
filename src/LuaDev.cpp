// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "LuaDev.h"
#include "LuaObject.h"
#include "Pi.h"
#include "WorldView.h"
#include "Game.h"
#include "Space.h"

/*
 * Lua commands used in development & debugging
 * Everything here is subject to rapid changes
 */

/*
 * Set current camera offset to vector,
 * it will also make the camera body visible
 * (the offset will reset when switching cameras)
 *
 * Dev.SetCameraOffset(x, y, z)
 */
static int l_dev_set_camera_offset(lua_State *l)
{
	if (!Pi::worldView)
		return luaL_error(l, "Dev.SetCameraOffset only works when there is a game running");
	Camera *cam = Pi::worldView->GetActiveCamera();
	const float x = luaL_checknumber(l, 1);
	const float y = luaL_checknumber(l, 2);
	const float z = luaL_checknumber(l, 3);
	cam->SetPosition(vector3d(x, y, z));
	cam->SetBodyVisible(true);
	return 0;
}

// dump frame structure to stdout (not console)
static int l_dev_dump_frames(lua_State *l)
{
	if (!Pi::game)
		return luaL_error(l, "game not running");
	Pi::game->GetSpace()->DebugDumpFrames();
	return 0;
}

void LuaDev::Register()
{
	lua_State *l = Lua::manager->GetLuaState();

	LUA_DEBUG_START(l);

	static const luaL_Reg methods[]= {
		{ "SetCameraOffset", l_dev_set_camera_offset },
		{ "DumpFrames",      l_dev_dump_frames       },
		{ 0, 0 }
	};

	luaL_newlib(l, methods);
	lua_setglobal(l, "Dev");

	LUA_DEBUG_END(l, 0);
}
