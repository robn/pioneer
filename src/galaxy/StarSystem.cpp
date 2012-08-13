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

static std::vector<const SystemBody*> _LinkBodies(const SystemDescriptor &desc, const std::vector<SystemBody*> &bodies)
{
	std::vector<const SystemBody*> linkedBodies;
	for (std::vector<SystemBody*>::const_iterator i = bodies.begin(); i != bodies.end(); ++i) {
		(*i)->path = desc.path;
		(*i)->path.bodyIndex = linkedBodies.size();
		linkedBodies.push_back(*i);
	}
	return linkedBodies;
}

StarSystem::StarSystem(
	const SystemDescriptor &_desc, const std::vector<SystemBody*> &_bodies, const EconomicData &_econ,
	bool _unexplored, const std::string &_shortDescription, const std::string &_longDescription) :
		rootBody(_bodies[0]), bodies(_LinkBodies(_desc, _bodies)),
		desc(_desc), econ(_econ), unexplored(_unexplored), shortDescription(_shortDescription), longDescription(_longDescription)
{
}

const SystemBody *StarSystem::GetBodyByPath(const SystemPath &path) const
{
	assert(desc.path.IsSameSystem(path));
	assert(path.IsBodyPath());
	assert(path.bodyIndex < bodies.size());

	return bodies[path.bodyIndex];
}

StarSystem::~StarSystem()
{
	if (rootBody) delete rootBody;
}

void StarSystem::Serialize(Serializer::Writer &wr, StarSystem *s)
{
	if (s) {
		wr.Byte(1);
		wr.Int32(s->desc.path.sectorX);
		wr.Int32(s->desc.path.sectorY);
		wr.Int32(s->desc.path.sectorZ);
		wr.Int32(s->desc.path.systemIndex);
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

