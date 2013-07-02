// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef HYPERSPACEMODEL_H
#define HYPERSPACEMODEL_H

#include "EquipType.h"
#include "libs.h"

class HyperspaceModel {
public:
	// drive equipment type, mass (t)
	HyperspaceModel(Equip::Type driveType, Uint32 mass);

	// maximum possible jump distance in ly
	// CalcHyperspaceRangeMax
	float GetMaxRange() const;
	
	// maximum jump distance possible (ly) given this amount of fuel (t)
	// CalcHyperspaceRange
	float GetRange(Uint32 fuelTonnes);

	// time required (s) for jump of given distance (ly)
	// CalcHyperspaceDuration
	float GetDuration(float dist) const;

	// fuel required (t) for jump of given distance (ly)
	// CalcHyperspaceFuelOut
	Uint32 GetFuelRequired(float dist) const;

private:
	Uint32 m_driveClass;
	Uint32 m_mass;
};

#endif
