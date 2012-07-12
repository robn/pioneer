#ifndef _SYSTEMDESCRIPTOR_H
#define _SYSTEMDESCRIPTOR_H

// high level description of a star system. loosely its what you could expect
// to learn if you looked at it with a telescope in your backyard :)

#include "libs.h"
#include "SystemPath.h"
#include "SystemBody.h"

class CustomSystem;

class SystemDescriptor {
public:
	enum { MAX_STARS = 4 };

	SystemDescriptor() : numStars(0), seed(0), customSys(0) {}

	SystemPath path;                          // sector and system index
	vector3f pos;                             // position within sector
	std::string name;                         // name
	unsigned int numStars;                    // number of stars
	SystemBody::BodyType starType[MAX_STARS]; // star types

	// XXX sysgen-specific, move these elsewhere
	Uint32 seed;                              // rng seed for sysgen
	const CustomSystem *customSys;            // pointer to custom system definition
};

#endif
