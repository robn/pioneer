#ifndef _CUSTOMSYSTEM_H
#define _CUSTOMSYSTEM_H

#include "StarSystem.h"
#include "Polit.h"
#include "vector3.h"
#include "fixed.h"


class CustomSBody {
public:
	friend class CustomSystem;

	CustomSBody::CustomSBody(
		const char      *name,
		SBody::BodyType  type,
		int              primaryIdx,
		fixed            radius,
		fixed            mass,
		int              averageTemp,
		fixed            semiMajorAxis,
		fixed            eccentricity,
		float            latitude,
		float            longitude,
		fixed            rotationPeriod,
		fixed            axialTilt,
		int              econType,
		const char      *heightMapFilename
	);

private:
	std::string     m_name;
	SBody::BodyType m_type;
	int             m_primaryIdx;  // -1 for primary
	fixed           m_radius; // in earth radii for planets, sol radii for stars
	fixed           m_mass; // earth masses or sol masses
	int             m_averageTemp; // kelvin
	fixed           m_semiMajorAxis; // in AUs
	fixed           m_eccentricity;
	// for orbiting things, latitude = inclination
	float           m_latitude, m_longitude; // radians
	fixed           m_rotationPeriod; // in days
	fixed           m_axialTilt; // in radians
	int             m_econType; // StarSystem.cpp enum ECON_XXX
	std::string     m_heightMapFilename;
};

class CustomSystem {
public:
	CustomSystem::CustomSystem(
		const char      *name, 
		SBody::BodyType  primaryType[4], 
		int              sectoryX, 
		int              sectoryY, 
		vector3f         pos, 
		Uint32           seed, 
		Polit::GovType   govType, 
		const char      *shortDesc, 
		const char      *longDesc
	);

	const char *GetName()       { return m_name.c_str(); };
	int GetSectorX()            { return m_sectorX; };
	int GetSectorY()            { return m_sectorY; };
	int GetSeed()               { return m_seed; };
	Polit::GovType GetGovType() { return m_govType; };
	const char *GetShortDesc()  { return m_shortDesc.c_str(); };
	const char *GetLongDesc()   { return m_longDesc.c_str(); };

	void AddCustomSBody(CustomSBody sBody) { m_sBodies.push_back(sBody); };
	std::vector *GetCustomSBodies()        { return &m_sBodies; };
	bool IsRandom()                        { return m_sBodies.size() > 0; };

private:
	std::string              m_name;
	std::vector<CustomSBody> m_sBodies;
	SBody::BodyType          m_primaryType[4];
	int                      m_sectorX, m_sectorY;
	vector3f                 m_pos;
	Uint32                   m_seed;
	Polit::GovType           m_govType;
	std::string              m_shortDesc;
	std::string              m_longDesc;
};

#endif /* _CUSTOMSYSTEM_H */
