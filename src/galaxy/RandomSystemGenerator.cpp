#include "RandomSystemGenerator.h"
#include "StarSystem.h"
#include "SystemConstants.h"
#include "Pi.h"
#include "LuaNameGen.h"
#include "Lang.h"
#include <cassert>

// very crudely
static const fixed AU_SOL_RADIUS = fixed(305,65536);
static const fixed AU_EARTH_RADIUS = fixed(3, 65536);

// if binary stars have separation s, planets can have stable
// orbits at (0.5 * s * SAFE_DIST_FROM_BINARY)
static const fixed SAFE_DIST_FROM_BINARY = fixed(5,1);

void RandomSystemGenerator::GenerateSystemSingle(MTRand &rand)
{
	SystemBody *star = NewStar(m_desc.starType[0], rand);

	star->name = m_desc.name;

	m_bodies.push_back(star);

	MakePlanetsAround(star, rand);
}

void RandomSystemGenerator::GenerateSystemDouble(MTRand &rand)
{
	SystemBody *star0 = NewStar(m_desc.starType[0], rand);
	SystemBody *star1 = NewStar(m_desc.starType[1], rand);
	SystemBody *grav = MakeBinaryPair(star0, star1, rand);

	grav->name = m_desc.name + " A,B";
	star0->name = m_desc.name + " A";
	star1->name = m_desc.name + " B";

	m_bodies.push_back(grav);
	m_bodies.push_back(star0);
	m_bodies.push_back(star1);

	MakePlanetsAround(star0, rand);
	MakePlanetsAround(star1, rand);
	MakePlanetsAround(grav, rand);
}

void RandomSystemGenerator::GenerateSystemTriple(MTRand &rand)
{
	SystemBody *star0 = NewStar(m_desc.starType[0], rand);
	SystemBody *star1 = NewStar(m_desc.starType[1], rand);
	SystemBody *star2 = NewStar(m_desc.starType[2], rand);

	SystemBody *grav = MakeBinaryPair(star0, star1, rand);

	SystemBody *supergrav = MakeBinaryPair(grav, star2, rand);

	supergrav->name = m_desc.name;
	grav->name = m_desc.name + " A,B";
	star0->name = m_desc.name + " A";
	star1->name = m_desc.name + " B";
	star2->name = m_desc.name + " C";

	m_bodies.push_back(supergrav);
	m_bodies.push_back(grav);
	m_bodies.push_back(star0);
	m_bodies.push_back(star1);
	m_bodies.push_back(star2);

	MakePlanetsAround(star0, rand);
	MakePlanetsAround(star1, rand);
	MakePlanetsAround(star2, rand);
	MakePlanetsAround(grav, rand);
}

void RandomSystemGenerator::GenerateSystemQuadruple(MTRand &rand)
{
	SystemBody *star0 = NewStar(m_desc.starType[0], rand);
	SystemBody *star1 = NewStar(m_desc.starType[1], rand);
	SystemBody *star2 = NewStar(m_desc.starType[2], rand);
	SystemBody *star3 = NewStar(m_desc.starType[3], rand);

	SystemBody *grav1 = MakeBinaryPair(star0, star1, rand);
	SystemBody *grav2 = MakeBinaryPair(star2, star3, rand);

	SystemBody *supergrav = MakeBinaryPair(grav1, grav2, rand);

	supergrav->name = m_desc.name;
	grav1->name = m_desc.name + " A,B";
	grav2->name = m_desc.name + " C,D";
	star0->name = m_desc.name + " A";
	star1->name = m_desc.name + " B";
	star2->name = m_desc.name + " C";
	star3->name = m_desc.name + " D";

	m_bodies.push_back(supergrav);
	m_bodies.push_back(grav1);
	m_bodies.push_back(star0);
	m_bodies.push_back(star1);
	m_bodies.push_back(grav2);
	m_bodies.push_back(star2);
	m_bodies.push_back(star3);

	MakePlanetsAround(star0, rand);
	MakePlanetsAround(star1, rand);
	MakePlanetsAround(star2, rand);
	MakePlanetsAround(star3, rand);
	MakePlanetsAround(grav1, rand);
	MakePlanetsAround(grav2, rand);
}

RefCountedPtr<StarSystem> RandomSystemGenerator::GenerateSystem()
{
	m_bodies.empty();

	const SystemPath &path = m_desc.path;

	unsigned long _init[6] = { path.systemIndex, Uint32(path.sectorX), Uint32(path.sectorY), Uint32(path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };
	MTRand rand(_init, 6);

	// 0 - ~500ly from sol: explored
	// ~500ly - ~700ly (65-90 sectors): gradual
	// ~700ly+: unexplored
	int dist = isqrt(1 + path.sectorX*path.sectorX + path.sectorY*path.sectorY + path.sectorZ*path.sectorZ);
	m_unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

	switch (m_desc.numStars) {
		case 1:
			GenerateSystemSingle(rand);
			break;

		case 2:
			GenerateSystemDouble(rand);
			break;

		case 3:
			GenerateSystemTriple(rand);
			break;

		case 4:
			GenerateSystemQuadruple(rand);
			break;

		default:
			abort();
	}

	// used in MakeshortDescription
	// XXX except this does not reflect the actual mining happening in this system
	m_econ.metallicity = SystemConstants::starMetallicities[m_bodies[0]->type]; // XXX broken for gravpoints

	Populate();
	PopulateAddStations(m_bodies[0]);

	RefCountedPtr<StarSystem> s(new StarSystem(m_desc, m_bodies, m_econ, m_unexplored, m_shortDesc));

	return s;
}

SystemBody *RandomSystemGenerator::NewStar(SystemBody::BodyType type, MTRand &rand)
{
	Uint32 seed = rand.Int32();

	SystemBody::PhysicalData phys;
	phys.radius = fixed(rand.Int32(SystemConstants::starTypeInfo[type].radius[0], SystemConstants::starTypeInfo[type].radius[1]), 100);
	phys.mass = fixed(rand.Int32(SystemConstants::starTypeInfo[type].mass[0], SystemConstants::starTypeInfo[type].mass[1]), 100);
	phys.averageTemp = rand.Int32(SystemConstants::starTypeInfo[type].tempMin, SystemConstants::starTypeInfo[type].tempMax);

	SystemBody *star = new SystemBody(type, phys);
	star->seed = seed;

	return star;
}

SystemBody *RandomSystemGenerator::MakeBinaryPair(SystemBody *a, SystemBody *b, MTRand &rand)
{
	SystemBody::OrbitalData orbit;

	orbit.eccentricity = rand.NFixed(3);

	const fixed minDist = (a->phys.radius + a->phys.radius) * AU_SOL_RADIUS;
	int mul = 1;
	do {
		switch (rand.Int32(3)) {
			case 2: orbit.semiMajorAxis = fixed(rand.Int32(100,10000), 100); break;
			case 1: orbit.semiMajorAxis = fixed(rand.Int32(10,1000), 100); break;
			default:
			case 0: orbit.semiMajorAxis = fixed(rand.Int32(1,100), 100); break;
		}
		orbit.semiMajorAxis *= mul;
		mul *= 2;
	} while (orbit.semiMajorAxis < minDist);

	orbit.orbMin = orbit.semiMajorAxis - orbit.eccentricity*orbit.semiMajorAxis;
	orbit.orbMax = 2*orbit.semiMajorAxis - orbit.orbMin;

	a->orbit = b->orbit = orbit;

	const float rotX = -0.5f*float(M_PI);//(float)(rand.Double()*M_PI/2.0);
	const float rotY = static_cast<float>(rand.Double(M_PI));
	a->orbit.position = matrix4x4d::RotateYMatrix(rotY) * matrix4x4d::RotateXMatrix(rotX);
	b->orbit.position = matrix4x4d::RotateYMatrix(rotY-M_PI) * matrix4x4d::RotateXMatrix(rotX);

	SystemBody::PhysicalData gravpointPhys;
	gravpointPhys.mass = a->phys.mass + b->phys.mass;
	gravpointPhys.radius = 4 * (a->orbit.orbMax + b->orbit.orbMax) / AU_SOL_RADIUS;
	SystemBody *gravpoint = new SystemBody(SystemBody::TYPE_GRAVPOINT, gravpointPhys);

	a->parent = b->parent = gravpoint;
	gravpoint->children.push_back(a);
	gravpoint->children.push_back(b);

	return gravpoint;
}

static fixed mass_from_disk_area(fixed a, fixed b, fixed max)
{
	// so, density of the disk with distance from star goes like so: 1 - x/discMax
	//
	// ---
	//    ---
	//       --- <- zero at discMax
	//
	// Which turned into a disc becomes 2*pi*x - (2*pi*x*x)/discMax
	// Integral of which is: pi*x*x - (2/(3*discMax))*pi*x*x*x
	//
	// Because get_disc_density divides total_mass by
	// mass_from_disk_area(0, discMax, discMax) to find density, the
	// constant factors (pi) in this equation drop out.
	//
	b = (b > max ? max : b);
	assert(b>=a);
	assert(a<=max);
	assert(b<=max);
	assert(a>=0);
	fixed one_over_3max = fixed(2,1)/(3*max);
	return (b*b - one_over_3max*b*b*b) -
		(a*a - one_over_3max*a*a*a);
}

static fixed get_disc_density(SystemBody *primary, fixed discMin, fixed discMax, fixed percentOfPrimaryMass)
{
	discMax = std::max(discMax, discMin);
	fixed total = mass_from_disk_area(discMin, discMax, discMax);
	return primary->GetMassInEarths() * percentOfPrimaryMass / total;
}

// http://en.wikipedia.org/wiki/Hill_sphere
static fixed calc_hill_radius(const SystemBody *body)
{
	if (body->GetSuperType() <= SystemBody::SUPERTYPE_STAR) {
		return fixed(0);
	} else {
		// playing with precision since these numbers get small
		// masses in earth masses
		fixedf<32> mprimary = body->parent->GetMassInEarths();

		fixedf<48> a = body->orbit.semiMajorAxis;
		fixedf<48> e = body->orbit.eccentricity;

		return fixed(a * (fixedf<48>(1,1)-e) *
				fixedf<48>::CubeRootOf(fixedf<48>(
						body->phys.mass / (fixedf<32>(3,1)*mprimary))));

		//fixed hr = semiMajorAxis*(fixed(1,1) - eccentricity) *
		//  fixedcuberoot(mass / (3*mprimary));
	}
}

void RandomSystemGenerator::MakePlanetsAround(SystemBody *primary, MTRand &rand)
{
	fixed discMin = fixed(0);
	fixed discMax = fixed(5000,1);
	fixed discDensity;

	SystemBody::BodySuperType superType = primary->GetSuperType();

	if (superType <= SystemBody::SUPERTYPE_STAR) {
		if (primary->type == SystemBody::TYPE_GRAVPOINT) {
			// around a binary
			discMin = primary->children[0]->orbit.orbMax * SAFE_DIST_FROM_BINARY;
		} else {
			// correct thing is roche limit, but lets ignore that because
			// it depends on body densities and gives some strange results
			discMin = 4 * primary->phys.radius * AU_SOL_RADIUS;
		}
		if (primary->type == SystemBody::TYPE_WHITE_DWARF) {
			// white dwarfs will have started as stars < 8 solar
			// masses or so, so pick discMax according to that
			// We give it a larger discMin because it used to be a much larger star
			discMin = 1000 * primary->phys.radius * AU_SOL_RADIUS;
			discMax = 100 * rand.NFixed(2);		// rand-splitting again
			discMax *= fixed::SqrtOf(fixed(1,2) + fixed(8,1)*rand.Fixed());
		} else {
			discMax = 100 * rand.NFixed(2)*fixed::SqrtOf(primary->phys.mass);
		}
		// having limited discMin by bin-separation/fake roche, and
		// discMax by some relation to star mass, we can now compute
		// disc density
		discDensity = rand.Fixed() * get_disc_density(primary, discMin, discMax, fixed(2,100));

		if ((superType == SystemBody::SUPERTYPE_STAR) && (primary->parent)) {
			// limit planets out to 10% distance to star's binary companion
			discMax = std::min(discMax, primary->orbit.orbMin * fixed(1,10));
		}

		// in trinary and quaternary systems don't bump into other pair...
		if (m_desc.numStars >= 3) {
			discMax = std::min(discMax, fixed(5,100)*m_bodies[0]->children[0]->orbit.orbMin);
		}
	} else {
		fixed primary_rad = primary->phys.radius * AU_EARTH_RADIUS;
		discMin = 4 * primary_rad;
		// use hill radius to find max size of moon system. for stars botch it.
		// And use planets orbit around its primary as a scaler to a moon's orbit
		discMax = std::min(discMax, fixed(1,20)*
			calc_hill_radius(primary)*primary->orbit.orbMin*fixed(1,10));

		discDensity = rand.Fixed() * get_disc_density(primary, discMin, discMax, fixed(1,500));
	}

	fixed initialJump = rand.NFixed(5);
	fixed pos = (fixed(1,1) - initialJump)*discMin + (initialJump*discMax);

	while (pos < discMax) {
		// periapsis, apoapsis = closest, farthest distance in orbit
		fixed periapsis = pos + pos*fixed(1,2)*rand.NFixed(2);/* + jump */;
		fixed ecc = rand.NFixed(3);
		fixed semiMajorAxis = periapsis / (fixed(1,1) - ecc);
		fixed apoapsis = 2*semiMajorAxis - periapsis;
		if (apoapsis > discMax) break;

		fixed mass;
		{
			const fixed a = pos;
			const fixed b = fixed(135,100)*apoapsis;
			mass = mass_from_disk_area(a, b, discMax);
			mass *= rand.Fixed() * discDensity;
		}
		if (mass < 0) {// hack around overflow
			fprintf(stderr, "WARNING: planetary mass has overflowed! (child of %s)\n", primary->name.c_str());
			mass = fixed(Sint64(0x7fFFffFFffFFffFFull));
		}
		assert(mass >= 0);

		Uint32 seed = rand.Int32();

		SystemBody::PhysicalData phys;
		phys.axialTilt = fixed(100,157)*rand.NFixed(2);
		phys.mass = mass;
		phys.rotationPeriod = fixed(rand.Int32(1,200), 24);

		SystemBody *planet = new SystemBody(SystemBody::TYPE_PLANET_TERRESTRIAL, phys);
		planet->seed = seed;
		planet->orbit.eccentricity = ecc;
		planet->orbit.semiMajorAxis = semiMajorAxis;
		planet->parent = primary;

		double r1 = rand.Double(2*M_PI);		// function parameter evaluation order is implementation-dependent
		double r2 = rand.NDouble(5);			// can't put two rands in the same expression
		planet->orbit.position = matrix4x4d::RotateYMatrix(r1) * matrix4x4d::RotateXMatrix(-0.5*M_PI + r2*M_PI/2.0);

		planet->orbit.orbMin = periapsis;
		planet->orbit.orbMax = apoapsis;

		m_bodies.push_back(planet);

		primary->children.push_back(planet);

		// minimum separation between planets of 1.35
		pos = apoapsis * fixed(135,100);
	}

	int idx=0;
	bool make_moons = superType <= SystemBody::SUPERTYPE_STAR;

	for (std::vector<const SystemBody*>::iterator i = primary->children.begin(); i != primary->children.end(); ++i) {
		// XXX SYSGEN CONST
		SystemBody *sbody = const_cast<SystemBody*>(*i);

		// planets around a binary pair [gravpoint] -- ignore the stars...
		if (sbody->GetSuperType() == SystemBody::SUPERTYPE_STAR) continue;
		// Turn them into something!!!!!!!
		char buf[8];
		if (superType <= SystemBody::SUPERTYPE_STAR) {
			// planet naming scheme
			snprintf(buf, sizeof(buf), " %c", 'a'+idx);
		} else {
			// moon naming scheme
			snprintf(buf, sizeof(buf), " %d", 1+idx);
		}
		sbody->name = primary->name+buf;
		sbody->PickPlanetType(rand);
		if (make_moons) MakePlanetsAround(sbody, rand);
		idx++;
	}
}

// Position a surface starport anywhere. Space.cpp::MakeFrameFor() ensures it
// is on dry land (discarding this position if necessary)
// XXX horror reuse of Orbit
static void position_settlement_on_planet(SystemBody *b)
{
	MTRand r(b->seed);
	// used for orientation on planet surface
	double r2 = r.Double(); 	// function parameter evaluation order is implementation-dependent
	double r1 = r.Double();		// can't put two rands in the same expression
	b->orbit.position = matrix4x4d::RotateZMatrix(2*M_PI*r1) * matrix4x4d::RotateYMatrix(2*M_PI*r2);
}

void RandomSystemGenerator::PopulateAddStations(SystemBody *body)
{
	for (unsigned int i=0; i<body->children.size(); i++) {
		PopulateAddStations(const_cast<SystemBody*>(body->children[i])); // XXX SYSGEN CONST
	}

	unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX),
			Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), body->seed, UNIVERSE_SEED };

	MTRand rand, namerand;
	rand.seed(_init, 6);
	namerand.seed(_init, 6);

	if (body->econ.population < fixed(1,1000)) return;

	fixed pop = body->econ.population + rand.Fixed();

	fixed orbMaxS = fixed(1,4)*calc_hill_radius(body);
	fixed orbMinS = 4 * body->phys.radius * AU_EARTH_RADIUS;
	if (body->children.size()) orbMaxS = std::min(orbMaxS, fixed(1,2) * body->children[0]->orbit.orbMin);

	// starports - orbital
	pop -= rand.Fixed();
	if ((orbMinS < orbMaxS) && (pop >= 0)) {

		SystemBody::PhysicalData phys;
		phys.rotationPeriod = fixed(1,3600);
		phys.averageTemp = body->phys.averageTemp;

		SystemBody *sp = new SystemBody(SystemBody::TYPE_STARPORT_ORBITAL, phys);
		sp->seed = rand.Int32();
		sp->parent = body;
		// just always plonk starports in near orbit
		sp->orbit.semiMajorAxis = orbMinS;
		sp->orbit.eccentricity = fixed(0);
		sp->orbit.position = matrix4x4d::Identity();
		sp->orbit.orbMin = sp->orbit.semiMajorAxis;
		sp->orbit.orbMax = sp->orbit.semiMajorAxis;

		sp->name = Pi::luaNameGen->BodyName(sp, namerand);

		m_bodies.push_back(sp);
		body->children.insert(body->children.begin(), sp);

		pop -= rand.Fixed();
		if (pop > 0) {
			// XXX horrid
			SystemBody *sp2 = new SystemBody(sp->type, phys);
			SystemPath path2 = sp2->path;
			*sp2 = *sp;
			sp2->path = path2;
			sp2->orbit.position = matrix4x4d::RotateZMatrix(M_PI);
			sp2->name = Pi::luaNameGen->BodyName(sp2, namerand);

			m_bodies.push_back(sp2);
			body->children.insert(body->children.begin(), sp2);
		}
	}

	// starports - surface
	pop = body->econ.population + rand.Fixed();
	int max = 6;
	while (max-- > 0) {
		pop -= rand.Fixed();
		if (pop < 0) break;

		SystemBody::PhysicalData phys;
		phys.averageTemp = body->phys.averageTemp;

		SystemBody *sp = new SystemBody(SystemBody::TYPE_STARPORT_SURFACE, phys);
		sp->seed = rand.Int32();
		sp->parent = body;
		sp->name = Pi::luaNameGen->BodyName(sp, namerand);

		position_settlement_on_planet(sp);

		m_bodies.push_back(sp);
		body->children.insert(body->children.begin(), sp);
	}
}

// percent
static const int MAX_COMMODITY_BASE_PRICE_ADJUSTMENT = 25;

void RandomSystemGenerator::Populate()
{
	unsigned long _init[5] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX), Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), UNIVERSE_SEED };
	MTRand rand;
	rand.seed(_init, 5);

	// Various system-wide characteristics
	// This is 1 in sector (0,0,0) and approaches 0 farther out
	// (1,0,0) ~ .688, (1,1,0) ~ .557, (1,1,1) ~ .48
	m_econ.humanProx = fixed(3,1) / isqrt(9 + 10*(m_desc.path.sectorX*m_desc.path.sectorX + m_desc.path.sectorY*m_desc.path.sectorY + m_desc.path.sectorZ*m_desc.path.sectorZ));
	m_econ.econType = ECON_INDUSTRY;
	m_econ.industrial = rand.Fixed();
	m_econ.agricultural = 0;

	// system attributes
	m_econ.totalPop = fixed(0);
	PopulateStage1(m_bodies[0], m_econ.totalPop);

	// So now we have balances of trade of various commodities.
	// Lets use black magic to turn these into percentage base price
	// alterations
	int maximum = 0;
	for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		maximum = std::max(abs(m_econ.tradeLevel[i]), maximum);
	}
	if (maximum) for (int i=Equip::FIRST_COMMODITY; i<=Equip::LAST_COMMODITY; i++) {
		m_econ.tradeLevel[i] = (m_econ.tradeLevel[i] * MAX_COMMODITY_BASE_PRICE_ADJUSTMENT) / maximum;
		m_econ.tradeLevel[i] += rand.Int32(-5, 5);
	}

	if (!m_shortDesc.size())
		MakeShortDescription(rand);
}

static const int CELSIUS = 273;

// Set natural resources, tech level, industry strengths and population levels
void RandomSystemGenerator::PopulateStage1(SystemBody *sbody, fixed &outTotalPop)
{
	for (unsigned int i=0; i<sbody->children.size(); i++) {
		PopulateStage1(const_cast<SystemBody*>(sbody->children[i]), outTotalPop); // XXX SYSGEN CONST
	}

	// unexplored systems have no population (that we know about)
	if (m_unexplored) {
		sbody->econ.population = outTotalPop = fixed(0);
		return;
	}

	unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX), Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), UNIVERSE_SEED, Uint32(m_desc.seed) };

	MTRand rand, namerand;
	rand.seed(_init, 6);
	namerand.seed(_init, 6);

	sbody->econ.population = fixed(0);

	// Bad type of planet for settlement
	if ((sbody->phys.averageTemp > CELSIUS+100) || (sbody->phys.averageTemp < 100) ||
	    (sbody->type != SystemBody::TYPE_PLANET_TERRESTRIAL && sbody->type != SystemBody::TYPE_PLANET_ASTEROID)) {

		// orbital starports should carry a small amount of population
		if (sbody->type == SystemBody::TYPE_STARPORT_ORBITAL) {
			sbody->econ.population = fixed(1,100000);
			outTotalPop += sbody->econ.population;
		}

		return;
	}

	sbody->econ.agricultural = fixed(0);

	if (sbody->composition.life > fixed(9,10)) {
		sbody->econ.agricultural = Clamp(fixed(1,1) - fixed(CELSIUS+25-sbody->phys.averageTemp, 40), fixed(0), fixed(1,1));
		m_econ.agricultural += 2*sbody->econ.agricultural;
	} else if (sbody->composition.life > fixed(1,2)) {
		sbody->econ.agricultural = Clamp(fixed(1,1) - fixed(CELSIUS+30-sbody->phys.averageTemp, 50), fixed(0), fixed(1,1));
		m_econ.agricultural += 1*sbody->econ.agricultural;
	} else {
		// don't bother populating crap planets
		if (sbody->composition.metallicity < fixed(5,10) &&
			sbody->composition.metallicity < (fixed(1,1) - m_econ.humanProx)) return;
	}

	const int NUM_CONSUMABLES = 10;
	const Equip::Type consumables[NUM_CONSUMABLES] = {
		Equip::AIR_PROCESSORS,
		Equip::GRAIN,
		Equip::FRUIT_AND_VEG,
		Equip::ANIMAL_MEAT,
		Equip::LIQUOR,
		Equip::CONSUMER_GOODS,
		Equip::MEDICINES,
		Equip::HAND_WEAPONS,
		Equip::NARCOTICS,
		Equip::LIQUID_OXYGEN
	};

	// Commodities we produce (mining and agriculture)
	for (int i=Equip::FIRST_COMMODITY; i<Equip::LAST_COMMODITY; i++) {
		Equip::Type t = Equip::Type(i);
		const EquipType &itype = Equip::types[t];

		fixed affinity = fixed(1,1);
		if (itype.econType & ECON_AGRICULTURE) {
			affinity *= 2*sbody->econ.agricultural;
		}
		if (itype.econType & ECON_INDUSTRY) affinity *= m_econ.industrial;
		// make industry after we see if agriculture and mining are viable
		if (itype.econType & ECON_MINING) {
			affinity *= sbody->composition.metallicity;
		}
		affinity *= rand.Fixed();
		// producing consumables is wise
		for (int j=0; j<NUM_CONSUMABLES; j++) {
			if (i == consumables[j]) affinity *= 2; break;
		}
		assert(affinity >= 0);
		// workforce...
		sbody->econ.population += affinity * m_econ.humanProx;

		int howmuch = (affinity * 256).ToInt32();

		m_econ.tradeLevel[t] += -2*howmuch;
		for (int j=0; j<EQUIP_INPUTS; j++) {
			if (!itype.inputs[j]) continue;
			m_econ.tradeLevel[itype.inputs[j]] += howmuch;
		}
	}

#if 0
XXX SYSGEN
	if (!system->m_hasCustomBodies && sbody->econ.population > 0)
		name = Pi::luaNameGen->BodyName(this, namerand);
#endif

	// Add a bunch of things people consume
	for (int i=0; i<NUM_CONSUMABLES; i++) {
		Equip::Type t = consumables[i];
		if (sbody->composition.life > fixed(1,2)) {
			// life planets can make this jizz probably
			if ((t == Equip::AIR_PROCESSORS) ||
			    (t == Equip::LIQUID_OXYGEN) ||
			    (t == Equip::GRAIN) ||
			    (t == Equip::FRUIT_AND_VEG) ||
			    (t == Equip::ANIMAL_MEAT)) {
				continue;
			}
		}
		m_econ.tradeLevel[t] += rand.Int32(32,128);
	}
	// well, outdoor worlds should have way more people
	sbody->econ.population = fixed(1,10)*sbody->econ.population + sbody->econ.population*sbody->econ.agricultural;

	outTotalPop += sbody->econ.population;
}

void RandomSystemGenerator::MakeShortDescription(MTRand &rand)
{
	m_econ.econType = 0;
	if ((m_econ.industrial > m_econ.metallicity) && (m_econ.industrial > m_econ.agricultural)) {
		m_econ.econType = ECON_INDUSTRY;
	} else if (m_econ.metallicity > m_econ.agricultural) {
		m_econ.econType = ECON_MINING;
	} else {
		m_econ.econType = ECON_AGRICULTURE;
	}

	if (m_unexplored) {
		m_shortDesc = Lang::UNEXPLORED_SYSTEM_NO_DATA;
	}

	/* Total population is in billions */
	else if(m_econ.totalPop == 0) {
		m_shortDesc = Lang::SMALL_SCALE_PROSPECTING_NO_SETTLEMENTS;
	} else if (m_econ.totalPop < fixed(1,10)) {
		switch (m_econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::SMALL_INDUSTRIAL_OUTPOST; break;
			case ECON_MINING: m_shortDesc = Lang::SOME_ESTABLISHED_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::YOUNG_FARMING_COLONY; break;
		}
	} else if (m_econ.totalPop < fixed(1,2)) {
		switch (m_econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_COLONY; break;
			case ECON_MINING: m_shortDesc = Lang::MINING_COLONY; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::OUTDOOR_AGRICULTURAL_WORLD; break;
		}
	} else if (m_econ.totalPop < fixed(5,1)) {
		switch (m_econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::HEAVY_INDUSTRY; break;
			case ECON_MINING: m_shortDesc = Lang::EXTENSIVE_MINING; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::THRIVING_OUTDOOR_WORLD; break;
		}
	} else {
		switch (m_econ.econType) {
			case ECON_INDUSTRY: m_shortDesc = Lang::INDUSTRIAL_HUB_SYSTEM; break;
			case ECON_MINING: m_shortDesc = Lang::VAST_STRIP_MINE; break;
			case ECON_AGRICULTURE: m_shortDesc = Lang::HIGH_POPULATION_OUTDOOR_WORLD; break;
		}
	}
}

