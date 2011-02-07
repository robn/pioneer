#ifndef _CUSTOMSYSTEH
#define _CUSTOMSYSTEH

#include "StarSystem.h"
#include "Polit.h"
#include "vector3.h"
#include "fixed.h"

class CustomSBody {
public:
	std::string     name;
	SBody::BodyType type;
	int             primaryIdx;  // -1 for primary
	fixed           radius; // in earth radii for planets, sol radii for stars
	fixed           mass; // earth masses or sol masses
	int             averageTemp; // kelvin
	fixed           semiMajorAxis; // in AUs
	fixed           eccentricity;
	// for orbiting things, latitude = inclination
	float           latitude, longitude; // radians
	fixed           rotationPeriod; // in days
	fixed           axialTilt; // in radians
	int             econType; // StarSystem.cpp enum ECON_XXX
	std::string     heightMapFilename;
};

class CustomSystem {
public:
    static void Init();
	static const std::list<CustomSystem*> GetCustomSystemsForSector(int sectorX, int sectorY);

	std::string            name;
	std::list<CustomSBody> sBodies;
	SBody::BodyType        primaryType[4];
	int                    sectorX, sectorY;
	vector3f               pos;
	Uint32                 seed;
	Polit::GovType         govType;
	std::string            shortDesc;
	std::string            longDesc;

	bool IsRandom() const { return sBodies.size() > 0; };
};

#endif /* _CUSTOMSYSTEH */
