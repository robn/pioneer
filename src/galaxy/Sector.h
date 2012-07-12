#ifndef _SECTOR_H
#define _SECTOR_H

#include "libs.h"
#include <string>
#include <vector>
#include "galaxy/StarSystem.h"
#include "galaxy/CustomSystem.h"
#include "SystemDescriptor.h"

class Sector {
public:
	// lightyears
	static const float SIZE;
	Sector(int x, int y, int z);
	static float DistanceBetween(const Sector *a, int sysIdxA, const Sector *b, int sysIdxB);
	static void Init();
	// Sector is within a bounding rectangle - used for SectorView m_sectorCache pruning.
	bool WithinBox(const int Xmin, const int Xmax, const int Ymin, const int Ymax, const int Zmin, const int Zmax) const;

	std::vector<SystemDescriptor> m_systems;
private:
	void GetCustomSystems();
	std::string GenName(SystemDescriptor &sys, MTRand &rand);
	int sx, sy, sz;
};

#endif /* _SECTOR_H */
