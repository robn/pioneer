#include "StarSystem.h"
#include "Sector.h"

#include "Serializer.h"
#include "Pi.h"
#include "LuaNameGen.h"
#include <map>
#include "utils.h"
#include "Lang.h"
#include "StringF.h"
#include <iostream>
#include "enum_table.h"
#include "SystemConstants.h"
#include "SystemCache.h"

StarSystem::StarSystem(
	const SystemDescriptor &desc, const std::vector<SystemBody*> &bodies, const EconomicData &econ,
	bool unexplored, const std::string &shortDesc, const std::string &longDesc) :
		m_desc(desc), m_econ(econ), m_unexplored(unexplored), m_shortDesc(shortDesc), m_longDesc(longDesc)
{
	for (std::vector<SystemBody*>::const_iterator i = bodies.begin(); i != bodies.end(); ++i) {
		(*i)->path = desc.path;
		(*i)->path.bodyIndex = m_bodies.size();
		m_bodies.push_back(*i);

		if ((*i)->GetSuperType() == SystemBody::SUPERTYPE_STARPORT)
			m_spaceStations.push_back(*i);
	}
	rootBody = m_bodies[0];
}

SystemBody *StarSystem::GetBodyByPath(const SystemPath &path) const
{
	assert(m_desc.path.IsSameSystem(path));
	assert(path.IsBodyPath());
	assert(path.bodyIndex < m_bodies.size());

	return m_bodies[path.bodyIndex];
}

StarSystem::~StarSystem()
{
	if (rootBody) delete rootBody;
}

void StarSystem::Serialize(Serializer::Writer &wr, StarSystem *s)
{
	if (s) {
		wr.Byte(1);
		wr.Int32(s->m_desc.path.sectorX);
		wr.Int32(s->m_desc.path.sectorY);
		wr.Int32(s->m_desc.path.sectorZ);
		wr.Int32(s->m_desc.path.systemIndex);
	} else {
		wr.Byte(0);
	}
}

RefCountedPtr<StarSystem> StarSystem::Unserialize(Serializer::Reader &rd)
{
	if (rd.Byte()) {
		int sec_x = rd.Int32();
		int sec_y = rd.Int32();
		int sec_z = rd.Int32();
		int sys_idx = rd.Int32();
		return Pi::systemCache->GetSystem(SystemPath(sec_x, sec_y, sec_z, sys_idx));
	} else {
		return RefCountedPtr<StarSystem>(0);
	}
}

