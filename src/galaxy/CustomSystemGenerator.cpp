#include "CustomSystemGenerator.h"
#include "CustomSystem.h"
#include "StarSystem.h"
#include "RandomSystemGenerator.h"

RefCountedPtr<StarSystem> CustomSystemGenerator::GenerateSystem() const
{
	assert(m_desc.customSys);

	const CustomSystem *custom = m_desc.customSys;
	
	RefCountedPtr<StarSystem> s;

	if (custom->IsRandom())
		s = RandomSystemGenerator(m_desc).GenerateSystem();

	else {
		s.Reset(new StarSystem(m_desc));
		s->m_hasCustomBodies = true;

		unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX), Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };
		MTRand rand(_init, 6);

		// XXX explored calc, duplicated
		int dist = isqrt(1 + m_desc.path.sectorX*m_desc.path.sectorX + m_desc.path.sectorY*m_desc.path.sectorY + m_desc.path.sectorZ*m_desc.path.sectorZ);
		s->m_unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

		GenerateFromCustom(s.Get(), m_desc.customSys, rand);
	}

	if (custom->shortDesc.length() > 0) s->m_shortDesc = custom->shortDesc;
	if (custom->longDesc.length() > 0) s->m_longDesc = custom->longDesc;
	if (!custom->want_rand_explored) s->m_unexplored = !custom->explored;

	s->m_isCustom = true;

	return s;
}

void CustomSystemGenerator::GenerateFromCustom(StarSystem *s, const CustomSystem *customSys, MTRand &rand)
{
	const CustomSystemBody *csbody = customSys->sBody;

	SystemBody *sbody = new SystemBody(csbody->type);
	sbody->seed = csbody->want_rand_seed ? rand.Int32() : csbody->seed;
	sbody->radius = csbody->radius;
	sbody->mass = csbody->mass;
	sbody->averageTemp = csbody->averageTemp;
	sbody->name = csbody->name;
	s->AddBody(sbody);
	s->rootBody = sbody;

	int humanInfestedness = 0;
	CustomGetKidsOf(s, sbody, csbody->children, &humanInfestedness, rand);

	s->Populate();
}

static double calc_orbital_period(double semiMajorAxis, double centralMass)
{
	return 2.0*M_PI*sqrt((semiMajorAxis*semiMajorAxis*semiMajorAxis)/(G*centralMass));
}

void CustomSystemGenerator::CustomGetKidsOf(StarSystem *s, SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand)
{
	for (std::vector<CustomSystemBody*>::const_iterator i = children.begin(); i != children.end(); i++) {
		const CustomSystemBody *csbody = *i;

		SystemBody *kid = new SystemBody(csbody->type);
		kid->parent = parent;
		kid->seed = csbody->want_rand_seed ? rand.Int32() : csbody->seed;
		kid->radius = csbody->radius;
		kid->averageTemp = csbody->averageTemp;
		kid->name = csbody->name;

		kid->mass = csbody->mass;
		if (kid->type == SystemBody::TYPE_PLANET_ASTEROID) kid->mass /= 100000;

		kid->m_metallicity    = csbody->metallicity;
		kid->m_volatileGas    = csbody->volatileGas;
		kid->m_volatileLiquid = csbody->volatileLiquid;
		kid->m_volatileIces   = csbody->volatileIces;
		kid->m_volcanicity    = csbody->volcanicity;
		kid->m_atmosOxidizing = csbody->atmosOxidizing;
		kid->m_life           = csbody->life;

		kid->rotationPeriod = csbody->rotationPeriod;
		kid->eccentricity = csbody->eccentricity;
		kid->orbitalOffset = csbody->orbitalOffset;
		kid->axialTilt = csbody->axialTilt;
		kid->semiMajorAxis = csbody->semiMajorAxis;
		kid->orbit.eccentricity = csbody->eccentricity.ToDouble();
		kid->orbit.semiMajorAxis = csbody->semiMajorAxis.ToDouble() * AU;
		kid->orbit.period = calc_orbital_period(kid->orbit.semiMajorAxis, parent->GetMass());
		if (csbody->heightMapFilename.length() > 0) {
			kid->heightMapFilename = csbody->heightMapFilename.c_str();
			kid->heightMapFractal = csbody->heightMapFractal;
		}

		if (kid->type == SystemBody::TYPE_STARPORT_SURFACE) {
			kid->orbit.rotMatrix = matrix4x4d::RotateYMatrix(csbody->longitude) *
				matrix4x4d::RotateXMatrix(-0.5*M_PI + csbody->latitude);
		} else {
			if (kid->orbit.semiMajorAxis < 1.2 * parent->GetRadius()) {
				Error("%s's orbit is too close to its parent", csbody->name.c_str());
			}
			double offset = csbody->want_rand_offset ? rand.Double(2*M_PI) : (csbody->orbitalOffset.ToDouble()*M_PI);
			kid->orbit.rotMatrix = matrix4x4d::RotateYMatrix(offset) * matrix4x4d::RotateXMatrix(-0.5*M_PI + csbody->latitude);
		}

		// perihelion and aphelion (in AUs)
		kid->orbMin = csbody->semiMajorAxis - csbody->eccentricity*csbody->semiMajorAxis;
		kid->orbMax = 2*csbody->semiMajorAxis - kid->orbMin;

		kid->PickAtmosphere();

		s->AddBody(kid);

		if (kid->GetSuperType() == SystemBody::SUPERTYPE_STARPORT) {
			(*outHumanInfestedness)++;
			s->m_spaceStations.push_back(kid);
		}
		parent->children.push_back(kid);

		CustomGetKidsOf(s, kid, csbody->children, outHumanInfestedness, rand);
	}
}
