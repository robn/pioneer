// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "LuaServerAgent.h"
#include "LuaObject.h"
#include "LuaRef.h"
#include "ServerAgent.h"
#include "Pi.h"
#include <json/json.h>

struct CallbackPair {
	CallbackPair(lua_State *l, int successIndex, int failIndex) :
		lua(l),
		successCallback(l, successIndex),
		failCallback(l, failIndex)
		{}
	lua_State *lua;
	LuaRef successCallback;
	LuaRef failCallback;
};

static Json::Value _lua_to_json(lua_State *l, int idx)
{
	int data = lua_absindex(l, idx);

	switch (lua_type(l, data)) {
		case LUA_TNIL:
			return Json::Value();
			
		case LUA_TNUMBER:
			return Json::Value(lua_tonumber(l, data));

		case LUA_TBOOLEAN:
			return Json::Value(lua_toboolean(l, data));

		case LUA_TSTRING:
			return Json::Value(lua_tostring(l, data));

		case LUA_TTABLE: {

			// XXX handle arrays

			Json::Value object(Json::objectValue);

			lua_pushnil(l);
			while (lua_next(l, data)) {
				const std::string key(luaL_checkstring(l, -2));
				Json::Value value(_lua_to_json(l, -1));
				object[key] = value;
				lua_pop(l, 1);
			}

			return object;
		}

		default:
			luaL_error(l, "can't convert Lua type %s to JSON", lua_typename(l, lua_type(l, idx)));
			return Json::Value();
	}

	// shouldn't get here
	assert(0);

	return Json::Value();
}

static void _json_to_lua(lua_State *l, const Json::Value &data)
{
	lua_newtable(l);

	// XXX do things
}

static void _success_callback(const Json::Value &data, void *userdata)
{
	CallbackPair *cp = reinterpret_cast<CallbackPair*>(userdata);
	if (!cp->successCallback.IsValid()) return;

	cp->successCallback.PushCopyToStack();

	_json_to_lua(cp->lua, data);
	pi_lua_protected_call(cp->lua, 1, 0);

	delete cp;
}

static void _fail_callback(const std::string &error, void *userdata)
{
	CallbackPair *cp = reinterpret_cast<CallbackPair*>(userdata);
	if (!cp->failCallback.IsValid()) return;

	cp->failCallback.PushCopyToStack();

	lua_pushlstring(cp->lua, error.c_str(), error.size());
	pi_lua_protected_call(cp->lua, 1, 0);

	delete cp;
}

static int l_serveragent_call(lua_State *l)
{
	const std::string method(luaL_checkstring(l, 1));
	const Json::Value data(_lua_to_json(l, 2));

	int successIndex = LUA_NOREF, failIndex = LUA_NOREF;

	if (lua_gettop(l) > 2) {
		luaL_checktype(l, 3, LUA_TFUNCTION);
		successIndex = 3;
	}
	if (lua_gettop(l) > 3) {
		luaL_checktype(l, 4, LUA_TFUNCTION);
		failIndex = 4;
	}

	CallbackPair *cp = new CallbackPair(l, successIndex, failIndex);
		
	Pi::serverAgent->Call(method, data, sigc::ptr_fun(_success_callback), sigc::ptr_fun(_fail_callback), cp);

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
