#ifndef _CUSTOMSYSTEH
#define _CUSTOMSYSTEH

#include "StarSystem.h"
#include "Polit.h"
#include "vector3.h"
#include "fixed.h"

class CustomSBody {
public:
	std::string            name;
	SBody::BodyType        type;
	fixed                  radius; // in earth radii for planets, sol radii for stars
	fixed                  mass; // earth masses or sol masses
	int                    averageTemp; // kelvin
	fixed                  semiMajorAxis; // in AUs
	fixed                  eccentricity;
	// for orbiting things, latitude = inclination
	float                  latitude, longitude; // radians
	fixed                  rotationPeriod; // in days
	fixed                  axialTilt; // in radians
	int                    econType; // StarSystem.cpp enum ECON_XXX
	std::string            heightMapFilename;
	std::list<CustomSBody> children;
};

class CustomSystem {
public:
	static void Init();
	static const std::list<const CustomSystem*> GetCustomSystemsForSector(int sectorX, int sectorY);
	static const CustomSystem* GetCustomSystem(const char* name);
	static const SBodyPath GetSBodyPathForCustomSystem(const CustomSystem* cs);
	static const SBodyPath GetSBodyPathForCustomSystem(const char* name);

	std::string            name;
    CustomSBody            sBody;
	SBody::BodyType        primaryType[4];
	int                    sectorX, sectorY;
	vector3f               pos;
	Uint32                 seed;
	Polit::GovType         govType;
	std::string            shortDesc;
	std::string            longDesc;

	bool IsRandom() const { return sBody.type == SBody::TYPE_GRAVPOINT; };
};

#endif /* _CUSTOMSYSTEH */
