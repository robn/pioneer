#ifndef _SYSTEMGENERATOR_H
#define _SYSTEMGENERATOR_H

#include "RefCounted.h"

class StarSystem;

class SystemGenerator {
public:
	virtual ~SystemGenerator() {}

	virtual RefCountedPtr<StarSystem> GenerateSystem() const = 0;

protected:
	SystemGenerator() {}
};

#endif
