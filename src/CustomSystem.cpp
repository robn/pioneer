#include "CustomSystem.h"

static std::list<CustomSystem> custom_systems;

void CustomSystem::AddCustomSystem(const CustomSystem *cs)
{
	custom_systems.push_back(*cs);
}

const std::list<CustomSystem*> GetCustomSystemsForSector(int sectorX, int sectorY)
{
	std::list<CustomSystem*> sector_systems;

	int n=0;
	for (std::list<CustomSystem>::iterator i = custom_systems.begin(); i != custom_systems.end(); i++) {
		const CustomSystem *cs = *i;
		if (cs->m_sectorX == sectorX && cs->m_sectorY == sectorY) {
			n++;
			sector_systems.push_back(cs);
		}
	}

    return sector_systems;
}
