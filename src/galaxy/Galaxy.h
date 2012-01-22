#ifndef _GALAXY_H
#define _GALAXY_H

#include "SDLWrappers.h"

class Galaxy {
public:
	Galaxy();
	~Galaxy();

	// these values in light-years
	float GetRadius() const { return GALAXY_RADIUS; }
	vector3f GetSolPosition() const { return vector3f(SOL_OFFSET_X, SOL_OFFSET_Y, 0.0f); }

	SDL_Surface *GetGalaxyBitmap() const { return m_densityMap.Get(); }
	Uint8 GetSectorDensity(int sx, int sy, int sz) const;

private:
	static const float GALAXY_RADIUS;
	static const float SOL_OFFSET_X;
	static const float SOL_OFFSET_Y;

	SDLSurfacePtr m_densityMap;
};

#endif /* _GALAXY_H */
