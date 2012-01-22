#include "libs.h"
#include "Galaxy.h"
#include "Pi.h"
#include "Sector.h"

// lightyears
const float Galaxy::GALAXY_RADIUS = 50000.0;
const float Galaxy::SOL_OFFSET_X = 25000.0;
const float Galaxy::SOL_OFFSET_Y = 0.0;

Galaxy::Galaxy() {
	m_densityMap.Reset(SDL_LoadBMP(PIONEER_DATA_DIR "/galaxy.bmp"));
	if (!m_densityMap) {
		Error("SDL_LoadBMP: %s\n", IMG_GetError());
		Pi::Quit();
	}
}

Galaxy::~Galaxy() {}

Uint8 Galaxy::GetSectorDensity(int sx, int sy, int sz) const
{
	// -1.0 to 1.0
	float offset_x = (sx*Sector::SIZE + SOL_OFFSET_X)/GALAXY_RADIUS;
	float offset_y = (-sy*Sector::SIZE + SOL_OFFSET_Y)/GALAXY_RADIUS;
	// 0.0 to 1.0
	offset_x = Clamp((offset_x + 1.0)*0.5, 0.0, 1.0);
	offset_y = Clamp((offset_y + 1.0)*0.5, 0.0, 1.0);

	int x = int(floor(offset_x * (m_densityMap->w - 1)));
	int y = int(floor(offset_y * (m_densityMap->h - 1)));

	SDL_LockSurface(m_densityMap.Get());
	int val = static_cast<char*>(m_densityMap->pixels)[x + y*m_densityMap->pitch];
	SDL_UnlockSurface(m_densityMap.Get());
	// crappy unrealistic but currently adequate density dropoff with sector z
	val = val * (256 - std::min(abs(sz),256)) >> 8;
	// reduce density somewhat to match real (gliese) density
	val >>= 1;
	return Uint8(val);
}
