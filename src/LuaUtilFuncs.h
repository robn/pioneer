#ifndef _LUAUTILFUNCS_H
#define _LUAUTILFUNCS_H

#include "oolua/oolua.h"

class pi_utils {
};

OOLUA_CLASS_NO_BASES(pi_utils)
	OOLUA_TYPEDEFS
		No_public_constructors
	OOLUA_END_TYPES
OOLUA_CLASS_END

namespace LuaUtilFuncs {
    void RegisterClasses(lua_State*);
}

#endif /* _LUAUTILFUNCS_H */
