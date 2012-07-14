#include "RandomSystemGenerator.h"
#include "StarSystem.h"
#include "SystemConstants.h"
#include "CustomSystem.h"
#include <cassert>

// very crudely
static const fixed AU_SOL_RADIUS = fixed(305,65536);
static const fixed AU_EARTH_RADIUS = fixed(3, 65536);

RefCountedPtr<StarSystem> RandomSystemGenerator::GenerateSystem() const
{
	const SystemPath &path = m_desc.path;

	RefCountedPtr<StarSystem> s(new StarSystem(m_desc));

	unsigned long _init[6] = { path.systemIndex, Uint32(path.sectorX), Uint32(path.sectorY), Uint32(path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };
	MTRand rand(_init, 6);

	/*
	 * 0 - ~500ly from sol: explored
	 * ~500ly - ~700ly (65-90 sectors): gradual
	 * ~700ly+: unexplored
	 */
	int dist = isqrt(1 + path.sectorX*path.sectorX + path.sectorY*path.sectorY + path.sectorZ*path.sectorZ);
	s->m_unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

	s->m_isCustom = s->m_hasCustomBodies = false;
	if (m_desc.customSys) {
		s->m_isCustom = true;
		const CustomSystem *custom = m_desc.customSys;
		if (custom->shortDesc.length() > 0) s->m_shortDesc = custom->shortDesc;
		if (custom->longDesc.length() > 0) s->m_longDesc = custom->longDesc;
		if (!custom->want_rand_explored) s->m_unexplored = !custom->explored;
		if (!custom->IsRandom()) {
			s->m_hasCustomBodies = true;
			s->GenerateFromCustom(m_desc.customSys, rand);
			return s;
		}
	}

	SystemBody *star[4];
	SystemBody *centGrav1(NULL), *centGrav2(NULL);

	const int numStars = m_desc.numStars;
	assert((numStars >= 1) && (numStars <= 4));

	if (numStars == 1) {
		star[0] = new SystemBody(SystemBody::NewStar(m_desc.starType[0], rand));
		star[0]->name = m_desc.name;
		s->AddBody(star[0]);
		s->rootBody = star[0];
	} else {
		centGrav1 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
		centGrav1->name = m_desc.name + " A,B";
		s->AddBody(centGrav1);
		s->rootBody = centGrav1;

		star[0] = new SystemBody(SystemBody::NewStar(m_desc.starType[0], rand));
		star[0]->name = m_desc.name + " A";
		star[0]->parent = centGrav1;
		s->AddBody(star[0]);

		star[1] = new SystemBody(s->MakeStarOfTypeLighterThan(m_desc.starType[1], star[0]->mass, rand));
		star[1]->name = m_desc.name + " B";
		star[1]->parent = centGrav1;
		s->AddBody(star[1]);

		centGrav1->mass = star[0]->mass + star[1]->mass;
		centGrav1->children.push_back(star[0]);
		centGrav1->children.push_back(star[1]);
		const fixed minDist1 = (star[0]->radius + star[1]->radius) * AU_SOL_RADIUS;
try_that_again_guvnah:
		MakeBinaryPair(star[0], star[1], minDist1, rand);

		if (numStars > 2) {
			if (star[0]->orbMax > fixed(100,1)) {
				// reduce to < 100 AU...
				goto try_that_again_guvnah;
			}
			// 3rd and maybe 4th star
			if (numStars == 3) {
				star[2] = new SystemBody(s->MakeStarOfTypeLighterThan(m_desc.starType[2], star[0]->mass, rand));
				star[2]->name = m_desc.name + " C";
				s->AddBody(star[2]);
				centGrav2 = star[2];
			} else {
				centGrav2 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
				centGrav2->name = m_desc.name + " C,D";
				s->AddBody(centGrav2);

				star[2] = new SystemBody(s->MakeStarOfTypeLighterThan(m_desc.starType[2], star[0]->mass, rand));
				star[2]->name = m_desc.name + " C";
				s->AddBody(star[2]);
				star[2]->parent = centGrav2;

				star[3] = new SystemBody(s->MakeStarOfTypeLighterThan(m_desc.starType[3], star[2]->mass, rand));
				star[3]->name = m_desc.name + " D";
				s->AddBody(star[3]);
				star[3]->parent = centGrav2;

				const fixed minDist2 = (star[2]->radius + star[3]->radius) * AU_SOL_RADIUS;
				MakeBinaryPair(star[2], star[3], minDist2, rand);
				centGrav2->mass = star[2]->mass + star[3]->mass;
				centGrav2->children.push_back(star[2]);
				centGrav2->children.push_back(star[3]);
			}

			SystemBody *superCentGrav = new SystemBody(SystemBody::TYPE_GRAVPOINT);
			superCentGrav->name = m_desc.name;
			s->AddBody(superCentGrav);
			s->rootBody = superCentGrav;

			centGrav1->parent = superCentGrav;
			centGrav2->parent = superCentGrav;
			const fixed minDistSuper = star[0]->orbMax + star[2]->orbMax;
			MakeBinaryPair(centGrav1, centGrav2, 4*minDistSuper, rand);

			superCentGrav->children.push_back(centGrav1);
			superCentGrav->children.push_back(centGrav2);

		}
	}

	// used in MakeshortDescription
	// XXX except this does not reflect the actual mining happening in this system
	s->m_metallicity = SystemConstants::starMetallicities[s->rootBody->type];

	for (std::size_t i=0; i<m_desc.numStars; i++) s->MakePlanetsAround(star[i], rand);

	if (m_desc.numStars > 1) s->MakePlanetsAround(centGrav1, rand);
	if (m_desc.numStars == 4) s->MakePlanetsAround(centGrav2, rand);

	s->Populate(true);

	return s;
}

void RandomSystemGenerator::MakeBinaryPair(SystemBody *a, SystemBody *b, fixed minDist, MTRand &rand)
{
	fixed m = a->mass + b->mass;
	fixed a0 = b->mass / m;
	fixed a1 = a->mass / m;
	a->eccentricity = rand.NFixed(3);
	int mul = 1;

	do {
		switch (rand.Int32(3)) {
			case 2: a->semiMajorAxis = fixed(rand.Int32(100,10000), 100); break;
			case 1: a->semiMajorAxis = fixed(rand.Int32(10,1000), 100); break;
			default:
			case 0: a->semiMajorAxis = fixed(rand.Int32(1,100), 100); break;
		}
		a->semiMajorAxis *= mul;
		mul *= 2;
	} while (a->semiMajorAxis < minDist);

	a->orbit.eccentricity = a->eccentricity.ToDouble();
	a->orbit.semiMajorAxis = AU * (a->semiMajorAxis * a0).ToDouble();
	a->orbit.period = 60*60*24*365* a->semiMajorAxis.ToDouble() * sqrt(a->semiMajorAxis.ToDouble() / m.ToDouble());

	const float rotX = -0.5f*float(M_PI);//(float)(rand.Double()*M_PI/2.0);
	const float rotY = static_cast<float>(rand.Double(M_PI));
	a->orbit.rotMatrix = matrix4x4d::RotateYMatrix(rotY) * matrix4x4d::RotateXMatrix(rotX);
	b->orbit.rotMatrix = matrix4x4d::RotateYMatrix(rotY-M_PI) * matrix4x4d::RotateXMatrix(rotX);

	b->orbit.eccentricity = a->eccentricity.ToDouble();
	b->orbit.semiMajorAxis = AU * (a->semiMajorAxis * a1).ToDouble();
	b->orbit.period = a->orbit.period;

	fixed orbMin = a->semiMajorAxis - a->eccentricity*a->semiMajorAxis;
	fixed orbMax = 2*a->semiMajorAxis - orbMin;
	a->orbMin = orbMin;
	b->orbMin = orbMin;
	a->orbMax = orbMax;
	b->orbMax = orbMax;
}

