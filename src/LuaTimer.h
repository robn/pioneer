#ifndef _LUATIMER_H
#define _LUATIMER_H

#include "DeleteEmitter.h"

class LuaTimer : public DeleteEmitter {
public:
	void Tick();
};

#endif
