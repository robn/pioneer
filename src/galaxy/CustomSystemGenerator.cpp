#include "CustomSystemGenerator.h"
#include "CustomSystem.h"
#include "StarSystem.h"
#include "RandomSystemGenerator.h"

RefCountedPtr<StarSystem> CustomSystemGenerator::GenerateSystem()
{
	assert(m_desc.customSys);

	const CustomSystem *custom = m_desc.customSys;
	
	RefCountedPtr<StarSystem> s;

	if (custom->IsRandom())
		s = RandomSystemGenerator(m_desc).GenerateSystem();

	else {
		unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX), Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };
		MTRand rand(_init, 6);

		// XXX explored calc, duplicated
		int dist = isqrt(1 + m_desc.path.sectorX*m_desc.path.sectorX + m_desc.path.sectorY*m_desc.path.sectorY + m_desc.path.sectorZ*m_desc.path.sectorZ);
		bool unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

		GenerateFromCustom(m_desc.customSys, rand);

		s.Reset(new StarSystem(m_desc, m_bodies));
		s->m_hasCustomBodies = true;

		s->m_unexplored = unexplored;

		s->Populate();
	}

	if (custom->shortDesc.length() > 0) s->m_shortDesc = custom->shortDesc;
	if (custom->longDesc.length() > 0) s->m_longDesc = custom->longDesc;
	if (!custom->want_rand_explored) s->m_unexplored = !custom->explored;

	s->m_isCustom = true;

	return s;
}

void CustomSystemGenerator::GenerateFromCustom(const CustomSystem *customSys, MTRand &rand)
{
	m_bodies.empty();

	const CustomSystemBody *csbody = customSys->sBody;

	SystemBody::PhysicalData phys;
	phys.radius = csbody->radius;
	phys.mass = csbody->mass;
	phys.averageTemp = csbody->averageTemp;

	SystemBody *sbody = new SystemBody(csbody->type, phys);
	sbody->seed = csbody->want_rand_seed ? rand.Int32() : csbody->seed;
	sbody->name = csbody->name;
	m_bodies.push_back(sbody);

	int humanInfestedness = 0;
	CustomGetKidsOf(sbody, csbody->children, &humanInfestedness, rand);
}

void CustomSystemGenerator::CustomGetKidsOf(SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand)
{
	for (std::vector<CustomSystemBody*>::const_iterator i = children.begin(); i != children.end(); i++) {
		const CustomSystemBody *csbody = *i;

		SystemBody::PhysicalData phys;
		phys.radius = csbody->radius;
		phys.averageTemp = csbody->averageTemp;
		phys.mass = csbody->mass;
		if (csbody->type == SystemBody::TYPE_PLANET_ASTEROID) phys.mass /= 100000;
		phys.rotationPeriod = csbody->rotationPeriod;
		phys.axialTilt = csbody->axialTilt;

		SystemBody *kid = new SystemBody(csbody->type, phys);
		kid->parent = parent;
		kid->seed = csbody->want_rand_seed ? rand.Int32() : csbody->seed;
		kid->name = csbody->name;

		kid->composition.metallicity    = csbody->metallicity;
		kid->composition.volatileGas    = csbody->volatileGas;
		kid->composition.volatileLiquid = csbody->volatileLiquid;
		kid->composition.volatileIces   = csbody->volatileIces;
		kid->composition.volcanicity    = csbody->volcanicity;
		kid->composition.atmosOxidizing = csbody->atmosOxidizing;
		kid->composition.life           = csbody->life;

		kid->orbit.eccentricity = csbody->eccentricity;
		kid->orbit.orbitalOffset = csbody->orbitalOffset;
		kid->orbit.semiMajorAxis = csbody->semiMajorAxis;

		if (csbody->heightMapFilename.length() > 0) {
			kid->heightMapFilename = csbody->heightMapFilename.c_str();
			kid->heightMapFractal = csbody->heightMapFractal;
		}

		if (kid->type == SystemBody::TYPE_STARPORT_SURFACE)
			kid->orbit.position = matrix4x4d::RotateYMatrix(csbody->longitude) * matrix4x4d::RotateXMatrix(-0.5*M_PI + csbody->latitude);
		else {
			if (kid->orbit.semiMajorAxis.ToDouble()*AU < 1.2 * parent->GetRadius()) {
				Error("%s's orbit is too close to its parent", csbody->name.c_str());
			}
			double offset = csbody->want_rand_offset ? rand.Double(2*M_PI) : (csbody->orbitalOffset.ToDouble()*M_PI);
			kid->orbit.position = matrix4x4d::RotateYMatrix(offset) * matrix4x4d::RotateXMatrix(-0.5*M_PI + csbody->latitude);
		}

		// perihelion and aphelion (in AUs)
		kid->orbit.orbMin = csbody->semiMajorAxis - csbody->eccentricity*csbody->semiMajorAxis;
		kid->orbit.orbMax = 2*csbody->semiMajorAxis - kid->orbit.orbMin;

		kid->PickAtmosphere();

		// pick or specify rings
		switch (csbody->ringStatus) {
			case CustomSystemBody::WANT_NO_RINGS:
				kid->m_rings.minRadius = fixed(0);
				kid->m_rings.maxRadius = fixed(0);
				break;
			case CustomSystemBody::WANT_RINGS:
				kid->PickRings(true);
				break;
			case CustomSystemBody::WANT_RANDOM_RINGS:
				kid->PickRings(false);
				break;
			case CustomSystemBody::WANT_CUSTOM_RINGS:
				kid->m_rings.minRadius = csbody->ringInnerRadius;
				kid->m_rings.maxRadius = csbody->ringOuterRadius;
				kid->m_rings.baseColor = csbody->ringColor;
				break;
		}

		m_bodies.push_back(kid);

		if (kid->GetSuperType() == SystemBody::SUPERTYPE_STARPORT)
			(*outHumanInfestedness)++;

		parent->children.push_back(kid);

		CustomGetKidsOf(kid, csbody->children, outHumanInfestedness, rand);
	}
}
