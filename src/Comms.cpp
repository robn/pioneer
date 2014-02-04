// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Comms.h"
#include "LuaUtils.h"
#include "LuaTable.h"
#include "LuaEvent.h"

namespace Comms {

static void AddMessage(const std::string &priority, const std::string &message, const std::string &from)
{
	lua_State *l = Lua::manager->GetLuaState();
	LuaTable t(l);
	t.Set("priority", priority);
	t.Set("message", message);
	if (!from.empty())
		t.Set("from", from);

	LuaRef ref(l, t.GetIndex());
	LuaEvent::Queue("onCommsMessage", &ref);
}

void Message(const std::string &message, const std::string &from)
{
	AddMessage("normal", message, from);
}

void ImportantMessage(const std::string &message, const std::string &from)
{
	AddMessage("important", message, from);
}

}
