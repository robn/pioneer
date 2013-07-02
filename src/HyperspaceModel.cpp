// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "HyperspaceModel.h"

HyperspaceModel::HyperspaceModel(Equip::Type driveType, Uint32 mass)
{
	assert(Equip::types[driveType].slot == Equip::SLOT_ENGINE);
	m_driveClass = Equip::types[driveType].pval;
	m_mass = mass;
}

float HyperspaceModel::GetMaxRange() const
{
	// 625.0f is balancing parameter
	return 625.0f * m_driveClass * m_driveClass / float(m_mass);
}

float HyperspaceModel::GetRange(Uint32 fuel)
{
	const float maxRange = GetMaxRange();
	const Uint32 maxFuelRequired = GetFuelRequired(maxRange);

	if (maxFuelRequired <= fuel)
		return maxRange;

	// range is proportional to fuel - use this as first guess
	float range = maxRange*fuel/maxFuelRequired;

	// if the range is too big due to rounding error, lower it until is is OK.
	while(range > 0 && GetFuelRequired(range) > fuel)
		range -= 0.05;

	// range is never negative
	range = std::max(range, 0.0f);

	return range;
}

float HyperspaceModel::GetDuration(float dist) const
{
	// 0.36 is balancing parameter
	return ((dist * dist * 0.36) / (GetMaxRange() * m_driveClass)) *
			(60.0 * 60.0 * 24.0 * sqrtf(m_mass));
}

Uint32 HyperspaceModel::GetFuelRequired(float dist) const
{
	const Uint32 sqDriveClass = m_driveClass*m_driveClass;
	Uint32 fuel = int(ceil(sqDriveClass*dist / GetMaxRange()));
	if (fuel > sqDriveClass) fuel = sqDriveClass;
	if (fuel < 1) fuel = 1;
	return fuel;
}
