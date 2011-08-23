#ifndef _LUAFIXED_H
#define _LUAFIXED_H

#include "LuaObject.h"
#include "fixed.h"

typedef LuaObjectUncopyable<fixed,LuaUncopyable<fixed> > LuaFixed;

#endif
