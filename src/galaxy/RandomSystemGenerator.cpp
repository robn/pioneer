#include "RandomSystemGenerator.h"
#include "StarSystem.h"
#include "SystemConstants.h"
#include "Sector.h"
#include <cassert>

// very crudely
static const fixed AU_SOL_RADIUS = fixed(305,65536);
static const fixed AU_EARTH_RADIUS = fixed(3, 65536);

RandomSystemGenerator::RandomSystemGenerator(const SystemPath &path) : SystemGenerator(),
	m_path(path)
{
	assert(path.IsSystemPath());
}

RefCountedPtr<StarSystem> RandomSystemGenerator::GenerateSystem() const
{
	Sector sec = Sector(m_path.sectorX, m_path.sectorY, m_path.sectorZ);
	assert(m_path.systemIndex >= 0 && m_path.systemIndex < sec.m_systems.size());

	const Sector::System &secSys = sec.m_systems[m_path.systemIndex];

	RefCountedPtr<StarSystem> s(new StarSystem(m_path));

	s->m_seed = secSys.seed;
	s->m_name = secSys.name;

	unsigned long _init[6] = { m_path.systemIndex, Uint32(m_path.sectorX), Uint32(m_path.sectorY), Uint32(m_path.sectorZ), UNIVERSE_SEED, Uint32(s->m_seed) };
	MTRand rand(_init, 6);

	/*
	 * 0 - ~500ly from sol: explored
	 * ~500ly - ~700ly (65-90 sectors): gradual
	 * ~700ly+: unexplored
	 */
	int dist = isqrt(1 + m_path.sectorX*m_path.sectorX + m_path.sectorY*m_path.sectorY + m_path.sectorZ*m_path.sectorZ);
	s->m_unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

	s->m_isCustom = s->m_hasCustomBodies = false;
	if (secSys.customSys) {
		s->m_isCustom = true;
		const CustomSystem *custom = secSys.customSys;
		s->m_numStars = custom->numStars;
		if (custom->shortDesc.length() > 0) s->m_shortDesc = custom->shortDesc;
		if (custom->longDesc.length() > 0) s->m_longDesc = custom->longDesc;
		if (!custom->want_rand_explored) s->m_unexplored = !custom->explored;
		if (!custom->IsRandom()) {
			s->m_hasCustomBodies = true;
			s->GenerateFromCustom(secSys.customSys, rand);
			return s;
		}
	}

	SystemBody *star[4];
	SystemBody *centGrav1(NULL), *centGrav2(NULL);

	const int numStars = secSys.numStars;
	assert((numStars >= 1) && (numStars <= 4));

	if (numStars == 1) {
		star[0] = new SystemBody(SystemBody::NewStar(secSys.starType[0], rand));
		star[0]->name = s->m_name;
		s->AddBody(star[0]);
		s->rootBody = star[0];
		s->m_numStars = 1;
	} else {
		centGrav1 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
		centGrav1->name = s->m_name + " A,B";
		s->AddBody(centGrav1);
		s->rootBody = centGrav1;

		star[0] = new SystemBody(SystemBody::NewStar(secSys.starType[0], rand));
		star[0]->name = s->m_name + " A";
		star[0]->parent = centGrav1;
		s->AddBody(star[0]);

		star[1] = new SystemBody(s->MakeStarOfTypeLighterThan(secSys.starType[1], star[0]->mass, rand));
		star[1]->name = s->m_name + " B";
		star[1]->parent = centGrav1;
		s->AddBody(star[1]);

		centGrav1->mass = star[0]->mass + star[1]->mass;
		centGrav1->children.push_back(star[0]);
		centGrav1->children.push_back(star[1]);
		const fixed minDist1 = (star[0]->radius + star[1]->radius) * AU_SOL_RADIUS;
try_that_again_guvnah:
		s->MakeBinaryPair(star[0], star[1], minDist1, rand);

		s->m_numStars = 2;

		if (numStars > 2) {
			if (star[0]->orbMax > fixed(100,1)) {
				// reduce to < 100 AU...
				goto try_that_again_guvnah;
			}
			// 3rd and maybe 4th star
			if (numStars == 3) {
				star[2] = new SystemBody(s->MakeStarOfTypeLighterThan(secSys.starType[2], star[0]->mass, rand));
				star[2]->name = s->m_name + " C";
				s->AddBody(star[2]);
				centGrav2 = star[2];
				s->m_numStars = 3;
			} else {
				centGrav2 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
				centGrav2->name = s->m_name + " C,D";
				s->AddBody(centGrav2);

				star[2] = new SystemBody(s->MakeStarOfTypeLighterThan(secSys.starType[2], star[0]->mass, rand));
				star[2]->name = s->m_name + " C";
				s->AddBody(star[2]);
				star[2]->parent = centGrav2;

				star[3] = new SystemBody(s->MakeStarOfTypeLighterThan(secSys.starType[3], star[2]->mass, rand));
				star[3]->name = s->m_name + " D";
				s->AddBody(star[3]);
				star[3]->parent = centGrav2;

				const fixed minDist2 = (star[2]->radius + star[3]->radius) * AU_SOL_RADIUS;
				s->MakeBinaryPair(star[2], star[3], minDist2, rand);
				centGrav2->mass = star[2]->mass + star[3]->mass;
				centGrav2->children.push_back(star[2]);
				centGrav2->children.push_back(star[3]);
				s->m_numStars = 4;
			}

			SystemBody *superCentGrav = new SystemBody(SystemBody::TYPE_GRAVPOINT);
			superCentGrav->name = s->m_name;
			s->AddBody(superCentGrav);
			s->rootBody = superCentGrav;

			centGrav1->parent = superCentGrav;
			centGrav2->parent = superCentGrav;
			const fixed minDistSuper = star[0]->orbMax + star[2]->orbMax;
			s->MakeBinaryPair(centGrav1, centGrav2, 4*minDistSuper, rand);

			superCentGrav->children.push_back(centGrav1);
			superCentGrav->children.push_back(centGrav2);

		}
	}

	// used in MakeShortDescription
	// XXX except this does not reflect the actual mining happening in this system
	s->m_metallicity = SystemConstants::starMetallicities[s->rootBody->type];

	for (int i=0; i<s->m_numStars; i++) s->MakePlanetsAround(star[i], rand);

	if (s->m_numStars > 1) s->MakePlanetsAround(centGrav1, rand);
	if (s->m_numStars == 4) s->MakePlanetsAround(centGrav2, rand);

	s->Populate(true);

	return s;
}
