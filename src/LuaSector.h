#ifndef _LUASECTOR_H
#define _LUASECTOR_H

#include "LuaObject.h"
#include "Sector.h"

// this specialisation releases the sector when lua is done with it, which
// probably causes it to be freed
template<>
class LuaAcquirer<Sector> {
public:
	virtual void Acquire(Sector *o) {}
	virtual void Release(Sector *o) {
		o->Release();
	}
};

typedef LuaObject<Sector> LuaSector;

#endif
