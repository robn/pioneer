// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _LUASERIALIZER_H
#define _LUASERIALIZER_H

#include "LuaManager.h"
#include "LuaObject.h"
#include "DeleteEmitter.h"
#include "SaveLoad.h"

class LuaSerializer : public DeleteEmitter {
	friend class LuaObject<LuaSerializer>;

public:
	SaveLoad::Object Save(SaveLoad::SaveContext *sc) const;
/* XXX DESERIALIZER
	void Unserialize(SaveLoad::Reader &rd);
*/

private:
	static int l_register(lua_State *l);

	static Json::Value pickle(SaveLoad::SaveContext *sc, lua_State *l, int idx, const std::string &name);
/* XXX DESERIALIZER
	static const char *unpickle(lua_State *l, const char *pos);
*/
};

#endif
