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

StarSystem::StarSystem(const SystemDescriptor &_desc, const std::vector<SystemBody*> &bodies, const EconomicData &_econ, bool _unexplored) :
	desc(_desc), econ(_econ), unexplored(_unexplored)
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
	assert(desc.path.IsSameSystem(path));
	assert(path.IsBodyPath());
	assert(path.bodyIndex < m_bodies.size());

	return m_bodies[path.bodyIndex];
}

void StarSystem::MakeShortDescription(MTRand &rand)
{
	econ.econType = 0;
	if ((econ.industrial > econ.metallicity) && (econ.industrial > econ.agricultural)) {
		econ.econType = ECON_INDUSTRY;
	} else if (econ.metallicity > econ.agricultural) {
		econ.econType = ECON_MINING;
	} else {
		econ.econType = ECON_AGRICULTURE;
	}

	if (unexplored) {
		m_shortDesc = Lang::UNEXPLORED_SYSTEM_NO_DATA;
	}

	/* Total population is in billions */
	else if(econ.totalPop == 0) {
		m_shortDesc = Lang::SMALL_SCALE_PROSPECTING_NO_SETTLEMENTS;
	} else if (econ.totalPop < fixed(1,10)) {
		switch (econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::SMALL_INDUSTRIAL_OUTPOST; break;
			case ECON_MINING: m_shortDesc = Lang::SOME_ESTABLISHED_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::YOUNG_FARMING_COLONY; break;
		}
	} else if (econ.totalPop < fixed(1,2)) {
		switch (econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_COLONY; break;
			case ECON_MINING: m_shortDesc = Lang::MINING_COLONY; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::OUTDOOR_AGRICULTURAL_WORLD; break;
		}
	} else if (econ.totalPop < fixed(5,1)) {
		switch (econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::HEAVY_INDUSTRY; break;
			case ECON_MINING: m_shortDesc = Lang::EXTENSIVE_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::THRIVING_OUTDOOR_WORLD; break;
		}
	} else {
		switch (econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_HUB_SYSTEM; break;
			case ECON_MINING: m_shortDesc = Lang::VAST_STRIP_MINE; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::HIGH_POPULATION_OUTDOOR_WORLD; break;
		}
	}
}

// percent
static const int MAX_COMMODITY_BASE_PRICE_ADJUSTMENT = 25;

void StarSystem::Populate()
{
	unsigned long _init[5] = { desc.path.systemIndex, Uint32(desc.path.sectorX), Uint32(desc.path.sectorY), Uint32(desc.path.sectorZ), UNIVERSE_SEED };
	MTRand rand;
	rand.seed(_init, 5);

	// Various system-wide characteristics
	// This is 1 in sector (0,0,0) and approaches 0 farther out
	// (1,0,0) ~ .688, (1,1,0) ~ .557, (1,1,1) ~ .48
	econ.humanProx = fixed(3,1) / isqrt(9 + 10*(desc.path.sectorX*desc.path.sectorX + desc.path.sectorY*desc.path.sectorY + desc.path.sectorZ*desc.path.sectorZ));
	econ.econType = ECON_INDUSTRY;
	econ.industrial = rand.Fixed();
	econ.agricultural = 0;

	// system attributes
	econ.totalPop = fixed(0);
	rootBody->PopulateStage1(this, econ.totalPop);

	// So now we have balances of trade of various commodities.
	// Lets use black magic to turn these into percentage base price
	// alterations
	int maximum = 0;
	for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		maximum = std::max(abs(econ.tradeLevel[i]), maximum);
	}
	if (maximum) for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		econ.tradeLevel[i] = (econ.tradeLevel[i] * MAX_COMMODITY_BASE_PRICE_ADJUSTMENT) / maximum;
		econ.tradeLevel[i] += rand.Int32(-5, 5);
	}

	Polit::GetSysPolitStarSystem(this, econ.totalPop, m_polit);

	if (!m_shortDesc.size())
		MakeShortDescription(rand);
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

