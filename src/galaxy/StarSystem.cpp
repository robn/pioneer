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

// minimum moon mass a little under Europa's
static const fixed MIN_MOON_MASS = fixed(1,30000); // earth masses
static const fixed MIN_MOON_DIST = fixed(15,10000); // AUs
static const fixed MAX_MOON_DIST = fixed(2, 100); // AUs
static const fixed PLANET_MIN_SEPARATION = fixed(135,100);

// very crudely
static const fixed AU_SOL_RADIUS = fixed(305,65536);
static const fixed AU_EARTH_RADIUS = fixed(3, 65536);

SystemBody *StarSystem::GetBodyByPath(const SystemPath &path) const
{
	assert(desc.path.IsSameSystem(path));
	assert(path.IsBodyPath());
	assert(path.bodyIndex < m_bodies.size());

	return m_bodies[path.bodyIndex];
}

SystemPath StarSystem::GetPathOf(const SystemBody *sbody) const
{
	return sbody->path;
}

// XXX this is stupid
SystemBody StarSystem::MakeStarOfTypeLighterThan(SystemBody::BodyType type, fixed maxMass, MTRand &rand)
{
	SystemBody sbody;
	int tries = 16;
	do {
		sbody = SystemBody::NewStar(type, rand);
	} while ((sbody.mass > maxMass) && (--tries));
	return sbody;
}

void StarSystem::MakeShortDescription(MTRand &rand)
{
	m_econType = 0;
	if ((m_industrial > m_metallicity) && (m_industrial > m_agricultural)) {
		m_econType = ECON_INDUSTRY;
	} else if (m_metallicity > m_agricultural) {
		m_econType = ECON_MINING;
	} else {
		m_econType = ECON_AGRICULTURE;
	}

	if (m_unexplored) {
		m_shortDesc = Lang::UNEXPLORED_SYSTEM_NO_DATA;
	}

	/* Total population is in billions */
	else if(m_totalPop == 0) {
		m_shortDesc = Lang::SMALL_SCALE_PROSPECTING_NO_SETTLEMENTS;
	} else if (m_totalPop < fixed(1,10)) {
		switch (m_econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::SMALL_INDUSTRIAL_OUTPOST; break;
			case ECON_MINING: m_shortDesc = Lang::SOME_ESTABLISHED_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::YOUNG_FARMING_COLONY; break;
		}
	} else if (m_totalPop < fixed(1,2)) {
		switch (m_econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_COLONY; break;
			case ECON_MINING: m_shortDesc = Lang::MINING_COLONY; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::OUTDOOR_AGRICULTURAL_WORLD; break;
		}
	} else if (m_totalPop < fixed(5,1)) {
		switch (m_econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::HEAVY_INDUSTRY; break;
			case ECON_MINING: m_shortDesc = Lang::EXTENSIVE_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::THRIVING_OUTDOOR_WORLD; break;
		}
	} else {
		switch (m_econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_HUB_SYSTEM; break;
			case ECON_MINING: m_shortDesc = Lang::VAST_STRIP_MINE; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::HIGH_POPULATION_OUTDOOR_WORLD; break;
		}
	}
}

/* percent */
#define MAX_COMMODITY_BASE_PRICE_ADJUSTMENT 25

void StarSystem::Populate()
{
	unsigned long _init[5] = { desc.path.systemIndex, Uint32(desc.path.sectorX), Uint32(desc.path.sectorY), Uint32(desc.path.sectorZ), UNIVERSE_SEED };
	MTRand rand;
	rand.seed(_init, 5);

	/* Various system-wide characteristics */
	// This is 1 in sector (0,0,0) and approaches 0 farther out
	// (1,0,0) ~ .688, (1,1,0) ~ .557, (1,1,1) ~ .48
	m_humanProx = fixed(3,1) / isqrt(9 + 10*(desc.path.sectorX*desc.path.sectorX + desc.path.sectorY*desc.path.sectorY + desc.path.sectorZ*desc.path.sectorZ));
	m_econType = ECON_INDUSTRY;
	m_industrial = rand.Fixed();
	m_agricultural = 0;

	/* system attributes */
	m_totalPop = fixed(0);
	rootBody->PopulateStage1(this, m_totalPop);

//	printf("Trading rates:\n");
	// So now we have balances of trade of various commodities.
	// Lets use black magic to turn these into percentage base price
	// alterations
	int maximum = 0;
	for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		maximum = std::max(abs(m_tradeLevel[i]), maximum);
	}
	if (maximum) for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		m_tradeLevel[i] = (m_tradeLevel[i] * MAX_COMMODITY_BASE_PRICE_ADJUSTMENT) / maximum;
		m_tradeLevel[i] += rand.Int32(-5, 5);
	}

// Unused?
//	for (int i=(int)Equip::FIRST_COMMODITY; i<=(int)Equip::LAST_COMMODITY; i++) {
//		Equip::Type t = (Equip::Type)i;
//		const EquipType &type = Equip::types[t];
//		printf("%s: %d%%\n", type.name, m_tradeLevel[t]);
//	}
//	printf("System total population %.3f billion\n", m_totalPop.ToFloat());
	Polit::GetSysPolitStarSystem(this, m_totalPop, m_polit);

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

