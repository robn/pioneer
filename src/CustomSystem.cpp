#include "CustomSystem.h"

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
)
{
	m_name = name;
	m_type = type;
	m_primaryIdx = primaryIdx;
	m_radius = radius;
	m_mass = mass;
	m_averageTemp = averageTemp;
	m_semiMajorAxis = semiMajorAxis;
	m_eccentricity = eccentricity;
	m_latitude = latitude;
	m_longitude = longitude;
	m_rotationPeriod = rotationPeriod;
	m_axialTilt = axialTilt;
	m_econType = econType;
	m_heightMapFilename = heightMapFilename;
}

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
)
{
	m_name = name;
	m_primaryType = primaryType;
	m_sectorX = sectoryX;
	m_sectorY = sectoryY;
	m_pos = pos;
	m_seed = seed;
	m_govType = govType;
	m_shortDesc = shortDesc;
	m_longDesc = longDesc;
}
