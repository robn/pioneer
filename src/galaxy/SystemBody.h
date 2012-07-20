#ifndef _SYSTEMBODY_H
#define _SYSTEMBODY_H

#include "SystemPath.h"
#include "Orbit.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "mtrand.h"
#include <vector>
#include <string>

class StarSystem;

class SystemBody {
public:

	enum BodyType { // <enum scope='SystemBody' prefix=TYPE_>
		TYPE_GRAVPOINT = 0,
		TYPE_BROWN_DWARF = 1, //  L+T Class Brown Dwarfs
		TYPE_WHITE_DWARF = 2,
		TYPE_STAR_M = 3, //red
		TYPE_STAR_K = 4, //orange
		TYPE_STAR_G = 5, //yellow
		TYPE_STAR_F = 6, //white
		TYPE_STAR_A = 7, //blue/white
		TYPE_STAR_B = 8, //blue
		TYPE_STAR_O = 9,  //blue/purple/white
		TYPE_STAR_M_GIANT = 10,
		TYPE_STAR_K_GIANT = 11,
		TYPE_STAR_G_GIANT = 12,
		TYPE_STAR_F_GIANT = 13,
		TYPE_STAR_A_GIANT = 14,
		TYPE_STAR_B_GIANT = 15,
		TYPE_STAR_O_GIANT = 16,
		TYPE_STAR_M_SUPER_GIANT = 17,
		TYPE_STAR_K_SUPER_GIANT = 18,
		TYPE_STAR_G_SUPER_GIANT = 19,
		TYPE_STAR_F_SUPER_GIANT = 20,
		TYPE_STAR_A_SUPER_GIANT = 21,
		TYPE_STAR_B_SUPER_GIANT = 22,
		TYPE_STAR_O_SUPER_GIANT = 23,
		TYPE_STAR_M_HYPER_GIANT = 24,
		TYPE_STAR_K_HYPER_GIANT = 25,
		TYPE_STAR_G_HYPER_GIANT = 26,
		TYPE_STAR_F_HYPER_GIANT = 27,
		TYPE_STAR_A_HYPER_GIANT = 28,
		TYPE_STAR_B_HYPER_GIANT = 29,
		TYPE_STAR_O_HYPER_GIANT = 30, // these various stars do exist = they are transitional states and are rare
		TYPE_STAR_M_WF = 31,  //Wolf-Rayet star
		TYPE_STAR_B_WF = 32,  // while you do not specifically get class M,B or O WF stars,
		TYPE_STAR_O_WF = 33, //  you do get red = blue and purple from the colour of the gasses = so spectral class is an easy way to define them.
		TYPE_STAR_S_BH = 34, //stellar blackhole
		TYPE_STAR_IM_BH = 35, //Intermediate-mass blackhole
		TYPE_STAR_SM_BH = 36, //Supermassive blackhole
		TYPE_PLANET_GAS_GIANT = 37,
		TYPE_PLANET_ASTEROID = 38,
		TYPE_PLANET_TERRESTRIAL = 39,
		TYPE_STARPORT_ORBITAL = 40,
		TYPE_STARPORT_SURFACE = 41,

        TYPE_LAST, // <enum skip>

		TYPE_MIN = TYPE_BROWN_DWARF, // <enum skip>
		TYPE_MAX = TYPE_STARPORT_SURFACE, // <enum skip>

		// supertype ranges
		TYPE_STAR_MIN = TYPE_BROWN_DWARF, // <enum skip>
		TYPE_STAR_MAX = TYPE_STAR_SM_BH, // <enum skip>

		TYPE_ROCKY_PLANET_MIN = TYPE_PLANET_ASTEROID, // <enum skip>
		TYPE_ROCKY_PLANET_MAX = TYPE_PLANET_TERRESTRIAL, // <enum skip>

		TYPE_GAS_GIANT_MIN = TYPE_PLANET_GAS_GIANT, // <enum skip>
		TYPE_GAS_GIANT_MAX = TYPE_PLANET_GAS_GIANT, // <enum skip>

		TYPE_STARPORT_MIN = TYPE_STARPORT_ORBITAL, // <enum skip>
		TYPE_STARPORT_MAX = TYPE_STARPORT_SURFACE // <enum skip>

		// XXX need larger atmosphereless thing
	};

	enum BodySuperType { // <enum scope='SystemBody' prefix=SUPERTYPE_>
		SUPERTYPE_NONE = 0,
		SUPERTYPE_STAR = 1,
		SUPERTYPE_ROCKY_PLANET = 2,
		SUPERTYPE_GAS_GIANT = 3,
		SUPERTYPE_STARPORT = 4,
	};

	SystemBody(BodyType type);

	static SystemBody NewStar(BodyType type, MTRand &rand);

	// XXX old interface to be checked
	~SystemBody();
	void PickPlanetType(MTRand &rand);
	const SystemBody *FindStarAndTrueOrbitalRange(fixed &orbMin, fixed &orbMax);

	std::string GetAstroDescription() const;
	const char *GetIcon() const;
	BodySuperType GetSuperType() const;
	double GetRadius() const {
		if (GetSuperType() <= SUPERTYPE_STAR)
			return phys.radius.ToDouble() * SOL_RADIUS;
		else
			return phys.radius.ToDouble() * EARTH_RADIUS;
	}
	double GetMass() const {
		if (GetSuperType() <= SUPERTYPE_STAR)
			return phys.mass.ToDouble() * SOL_MASS;
		else
			return phys.mass.ToDouble() * EARTH_MASS;
	}
	fixed GetMassInEarths() const {
		if (GetSuperType() <= SUPERTYPE_STAR)
			return phys.mass * 332998;
		else
			return phys.mass;
	}
	// returned in seconds
	double GetRotationPeriod() const {
		return phys.rotationPeriod.ToDouble()*60*60*24;
	}
	double CalcSurfaceGravity() const;

	double GetMaxChildOrbitalDistance() const;
	void PopulateStage1(StarSystem *system, fixed &outTotalPop);

	bool HasAtmosphere() const;
	void PickAtmosphere();
	void GetAtmosphereFlavor(Color *outColor, double *outDensity) const {
		*outColor = m_atmosColor;
		*outDensity = m_atmosDensity;
	}

	bool IsScoopable() const;

	SystemBody *parent;
	std::vector<SystemBody*> children;

	BodyType type;
	Uint32 id; // index into starsystem->m_bodies
	SystemPath path;
	Uint32 seed; // Planet.cpp can use to generate terrain
	std::string name;

	struct PhysicalData {
		fixed radius;
		fixed mass;           // earth masses if planet, solar masses if star
		fixed rotationPeriod; // in days
		fixed axialTilt;      // in radians
		int averageTemp;      // celcius
	};
	PhysicalData phys;

	struct OrbitalData {
		fixed      orbMin, orbMax; // periapsis, apoapsis in AUs
		fixed      semiMajorAxis;  // in AUs
		fixed      eccentricity;   // 0.0 - 1.0
		fixed      orbitalOffset;  // radians
		matrix4x4d position;       // initial position
	};
	OrbitalData orbit;

	struct Composition {
		fixed metallicity;    // (crust) 0.0 = light (Al, SiO2, etc), 1.0 = heavy (Fe, heavy metals)
		fixed volatileGas;    // 1.0 = earth atmosphere density
		fixed volatileLiquid; // 1.0 = 100% ocean cover (earth = 70%)
		fixed volatileIces;   // 1.0 = 100% ice cover (earth = 3%)
		fixed volcanicity;    // 0 = none, 1.0 = fucking volcanic
		fixed atmosOxidizing; // 0.0 = reducing (H2, NH3, etc), 1.0 = oxidising (CO2, O2, etc)
		fixed life;           // 0.0 = dead, 1.0 = teeming
	};
	Composition composition;

	/* economy type stuff */
	fixed m_population;
	fixed m_agricultural;

	const char *heightMapFilename;
	unsigned int heightMapFractal;

	Orbit GetOrbit() const;

private:
	Color m_atmosColor;
	double m_atmosDensity;
};

#endif
