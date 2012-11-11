// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "LuaServerAgent.h"
#include "LuaObject.h"
#include "ServerAgent.h"
#include "Pi.h"
#include <json/json.h>

static Json::Value _lua_to_json(lua_State *l, int idx)
{
	int data = lua_absindex(l, idx);

	Json::Value json;

	return json;
}

static void _json_to_lua(lua_State *l, const Json::Value &data)
{
}

static int l_serveragent_call(lua_State *l)
{
	const std::string method(luaL_checkstring(l, 1));
	const Json::Value data(_lua_to_json(l, 2));
		
	// XXX callbacks
	
	Pi::serverAgent->Call(method, data);

	return 0;
}

void LuaServerAgent::Register()
{
	static const luaL_Reg l_methods[] = {
		{ "Call", l_serveragent_call },
		{ 0, 0 }
	};

	LuaObjectBase::CreateObject(l_methods, 0, 0);
	lua_setglobal(Lua::manager->GetLuaState(), "ServerAgent");
}
