// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _LUASERIALIZER_H
#define _LUASERIALIZER_H

#include "LuaManager.h"
#include "LuaObject.h"
#include "DeleteEmitter.h"
#include "Serializer.h"

class LuaSerializer : public DeleteEmitter {
	friend class LuaObject<LuaSerializer>;

public:
	Serializer::Object Serialize(Serializer::GameSerializer *gs) const;
/* XXX DESERIALIZER
	void Unserialize(Serializer::Reader &rd);
*/

private:
	static int l_register(lua_State *l);

	static Json::Value pickle(Serializer::GameSerializer *gs, lua_State *l, int idx, const std::string &name);
/* XXX DESERIALIZER
	static const char *unpickle(lua_State *l, const char *pos);
*/
};

#endif
