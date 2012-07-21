#ifndef _SYSTEMDESCRIPTOR_H
#define _SYSTEMDESCRIPTOR_H

// high level description of a star system. loosely its what you could expect
// to learn if you looked at it with a telescope in your backyard :)

#include "libs.h"
#include "SystemPath.h"
#include "SystemBody.h"
#include "Array.h"
#include <cassert>

class CustomSystem;

class SystemDescriptor {
public:
	enum { MAX_STARS = 4 };

	SystemDescriptor(const SystemPath &_path, const vector3f &_pos, const std::string &_name, unsigned int _numStars, const Array<SystemBody::BodyType,MAX_STARS> &_starType) :
		path(_path),
		pos(_pos),
		name(_name),
		numStars(_numStars),
		starType(_starType),
		seed(0),
		customSys(0)
	{
		assert(numStars > 0 && numStars <= MAX_STARS);
	}

	SystemPath path;                                // sector and system index
	vector3f pos;                                   // position within sector
	std::string name;                               // name
	std::size_t numStars;                           // number of stars
	Array<SystemBody::BodyType,MAX_STARS> starType; // star type

	// XXX sysgen-specific, move these elsewhere
	Uint32 seed;                                    // rng seed for sysgen
	const CustomSystem *customSys;                  // pointer to custom system definition
};

#endif
