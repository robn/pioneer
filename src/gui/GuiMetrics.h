#ifndef _GUIMETRICS_H
#define _GUIMETRICS_H

#include "vector2.h"

namespace Gui {

class Metrics {
public:
	Metrics() : ideal(vector2f(DONTCARE,DONTCARE)), minimum(vector2f(DONTCARE,DONTCARE)), maximum(vector2f(DONTCARE,DONTCARE)) {}
	Metrics(const vector2f &_ideal) : ideal(_ideal), minimum(vector2f(DONTCARE,DONTCARE)), maximum(vector2f(DONTCARE,DONTCARE)) {}
	Metrics(const vector2f &_ideal, const vector2f &_minimum) : ideal(_ideal), minimum(_minimum), maximum(vector2f(DONTCARE,DONTCARE)) {}
	Metrics(const vector2f &_ideal, const vector2f &_minimum, const vector2f &_maximum) : ideal(_ideal), minimum(_minimum), maximum(_maximum) {}
	
	// a magic value to indicate we don't care about this particular dimension
	static const float DONTCARE = -1.0f;

	// ideal dimensions   (widget functions optimally in this space)
	vector2f ideal;

	// minimum dimensions (widget needs at least this much to function)
	vector2f minimum;;
	
	// maximum dimensions (please don't give me more than this)
	vector2f maximum;
};

}

#endif
