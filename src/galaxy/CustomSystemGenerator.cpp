#include "CustomSystemGenerator.h"
#include "CustomSystem.h"
#include "StarSystem.h"
#include "RandomSystemGenerator.h"

RefCountedPtr<StarSystem> CustomSystemGenerator::GenerateSystem() const
{
	assert(m_desc.customSys);

	const CustomSystem *custom = m_desc.customSys;
	
	RefCountedPtr<StarSystem> s;

	if (custom->IsRandom()) {
		s = RandomSystemGenerator(m_desc).GenerateSystem();

		if (custom->shortDesc.length() > 0) s->m_shortDesc = custom->shortDesc;
		if (custom->longDesc.length() > 0) s->m_longDesc = custom->longDesc;
		if (!custom->want_rand_explored) s->m_unexplored = !custom->explored;
	}
	else {
		s.Reset(new StarSystem(m_desc));
		s->m_hasCustomBodies = true;

		unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX), Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };
		MTRand rand(_init, 6);

		s->GenerateFromCustom(m_desc.customSys, rand);
	}

	s->m_isCustom = true;

	return s;
}
