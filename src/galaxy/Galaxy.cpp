#include "libs.h"
#include "Galaxy.h"
#include "Pi.h"
#include "Sector.h"
#include "Lang.h"
#include "FileSystem.h"

// lightyears
const float Galaxy::GALAXY_RADIUS = 50000.0;
const float Galaxy::SOL_OFFSET_X = 25000.0;
const float Galaxy::SOL_OFFSET_Y = 0.0;

const Galaxy::FeatureLabel Galaxy::GALACTIC_FEATURES[] = {
	{ Lang::NORMA_ARM            , Galaxy::GALAXY_RADIUS * vector3d( 0.0 , -0.3 , 0.0) },
	{ Lang::PERSEUS_ARM          , Galaxy::GALAXY_RADIUS * vector3d( 0.57,  0.0 , 0.0) },
	{ Lang::OUTER_ARM            , Galaxy::GALAXY_RADIUS * vector3d( 0.65,  0.4 , 0.0) },
	{ Lang::SAGITTARIUS_ARM      , Galaxy::GALAXY_RADIUS * vector3d(-0.3 ,  0.2 , 0.0) },
	{ Lang::SCUTUM_CENTAURUS_ARM , Galaxy::GALAXY_RADIUS * vector3d(-0.45, -0.45, 0.0) },
	{ 0, vector3d(0.0, 0.0, 0.0) }
};

static const std::string galaxyBitmapFilename("galaxy.bmp");

Galaxy::Galaxy() {
	RefCountedPtr<FileSystem::FileData> filedata = FileSystem::gameDataFiles.ReadFile("galaxy.bmp");
	if (!filedata) {
		Error("Galaxy: couldn't load '%s'\n", galaxyBitmapFilename.c_str());
		Pi::Quit();
	}

	SDL_RWops *datastream = SDL_RWFromConstMem(filedata->GetData(), filedata->GetSize());
	m_densityMap.Reset(SDL_LoadBMP_RW(datastream, 1));
	if (!m_densityMap) {
		Error("Galaxy: couldn't load: %s (%s)\n", galaxyBitmapFilename.c_str(), SDL_GetError());
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
