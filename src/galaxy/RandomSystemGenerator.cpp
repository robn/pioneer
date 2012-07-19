#include "RandomSystemGenerator.h"
#include "StarSystem.h"
#include "SystemConstants.h"
#include "Pi.h"
#include "LuaNameGen.h"
#include <cassert>

// very crudely
static const fixed AU_SOL_RADIUS = fixed(305,65536);
static const fixed AU_EARTH_RADIUS = fixed(3, 65536);

// if binary stars have separation s, planets can have stable
// orbits at (0.5 * s * SAFE_DIST_FROM_BINARY)
static const fixed SAFE_DIST_FROM_BINARY = fixed(5,1);

static SystemBody new_star_lighter_than(SystemBody::BodyType type, const SystemBody &orig, MTRand &rand)
{
	// try for a while until we get one
	int tries = 16;
	while (tries--) {
		SystemBody body(SystemBody::NewStar(type, rand));
		if (body.mass <= orig.mass)
			return body;
	}

	// just make one the same size as the current one, sigh
	SystemBody body(type);
	body.mass = orig.mass;

	return body;
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
	bool unexplored = (dist > 90) || (dist > 65 && rand.Int32(dist) > 40);

	SystemBody *star[4];
	SystemBody *centGrav1(NULL), *centGrav2(NULL);

	const int numStars = m_desc.numStars;
	assert((numStars >= 1) && (numStars <= 4));

	if (numStars == 1) {
		star[0] = new SystemBody(SystemBody::NewStar(m_desc.starType[0], rand));
		star[0]->name = m_desc.name;
		m_bodies.push_back(star[0]);
	} else {
		centGrav1 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
		centGrav1->name = m_desc.name + " A,B";
		m_bodies.push_back(centGrav1);

		star[0] = new SystemBody(SystemBody::NewStar(m_desc.starType[0], rand));
		star[0]->name = m_desc.name + " A";
		star[0]->parent = centGrav1;
		m_bodies.push_back(star[0]);

		star[1] = new SystemBody(new_star_lighter_than(m_desc.starType[1], *star[0], rand));
		star[1]->name = m_desc.name + " B";
		star[1]->parent = centGrav1;
		m_bodies.push_back(star[1]);

		centGrav1->mass = star[0]->mass + star[1]->mass;
		centGrav1->children.push_back(star[0]);
		centGrav1->children.push_back(star[1]);
		const fixed minDist1 = (star[0]->radius + star[1]->radius) * AU_SOL_RADIUS;
try_that_again_guvnah:
		MakeBinaryPair(star[0], star[1], minDist1, rand);

		if (numStars > 2) {
			if (star[0]->orbit.orbMax > fixed(100,1)) {
				// reduce to < 100 AU...
				goto try_that_again_guvnah;
			}
			// 3rd and maybe 4th star
			if (numStars == 3) {
				star[2] = new SystemBody(new_star_lighter_than(m_desc.starType[2], *star[0], rand));
				star[2]->name = m_desc.name + " C";
				m_bodies.push_back(star[2]);
				centGrav2 = star[2];
			} else {
				centGrav2 = new SystemBody(SystemBody::TYPE_GRAVPOINT);
				centGrav2->name = m_desc.name + " C,D";
				m_bodies.push_back(centGrav2);

				star[2] = new SystemBody(new_star_lighter_than(m_desc.starType[2], *star[0], rand));
				star[2]->name = m_desc.name + " C";
				star[2]->parent = centGrav2;
				m_bodies.push_back(star[2]);

				star[3] = new SystemBody(new_star_lighter_than(m_desc.starType[3], *star[2], rand));
				star[3]->name = m_desc.name + " D";
				star[3]->parent = centGrav2;
				m_bodies.push_back(star[3]);

				const fixed minDist2 = (star[2]->radius + star[3]->radius) * AU_SOL_RADIUS;
				MakeBinaryPair(star[2], star[3], minDist2, rand);
				centGrav2->mass = star[2]->mass + star[3]->mass;
				centGrav2->children.push_back(star[2]);
				centGrav2->children.push_back(star[3]);
			}

			SystemBody *superCentGrav = new SystemBody(SystemBody::TYPE_GRAVPOINT);
			superCentGrav->name = m_desc.name;
			m_bodies.push_back(superCentGrav);

			centGrav1->parent = superCentGrav;
			centGrav2->parent = superCentGrav;
			const fixed minDistSuper = star[0]->orbit.orbMax + star[2]->orbit.orbMax;
			MakeBinaryPair(centGrav1, centGrav2, 4*minDistSuper, rand);

			superCentGrav->children.push_back(centGrav1);
			superCentGrav->children.push_back(centGrav2);

		}
	}

	for (std::size_t i=0; i<m_desc.numStars; i++) MakePlanetsAround(star[i], rand);

	if (m_desc.numStars > 1) MakePlanetsAround(centGrav1, rand);
	if (m_desc.numStars == 4) MakePlanetsAround(centGrav2, rand);

	RefCountedPtr<StarSystem> s(new StarSystem(m_desc, m_bodies));

	// used in MakeshortDescription
	// XXX except this does not reflect the actual mining happening in this system
	s->m_metallicity = SystemConstants::starMetallicities[s->rootBody->type];

	s->m_unexplored = unexplored;

	s->Populate();
	PopulateAddStations(m_bodies[0]);

	return s;
}

void RandomSystemGenerator::MakeBinaryPair(SystemBody *a, SystemBody *b, fixed minDist, MTRand &rand)
{
	fixed m = a->mass + b->mass;
	fixed a0 = b->mass / m;
	fixed a1 = a->mass / m;
	a->orbit.eccentricity = rand.NFixed(3);
	int mul = 1;

	do {
		switch (rand.Int32(3)) {
			case 2: a->orbit.semiMajorAxis = fixed(rand.Int32(100,10000), 100); break;
			case 1: a->orbit.semiMajorAxis = fixed(rand.Int32(10,1000), 100); break;
			default:
			case 0: a->orbit.semiMajorAxis = fixed(rand.Int32(1,100), 100); break;
		}
		a->orbit.semiMajorAxis *= mul;
		mul *= 2;
	} while (a->orbit.semiMajorAxis < minDist);

	double period = 60*60*24*365* a->orbit.semiMajorAxis.ToDouble() * sqrt(a->orbit.semiMajorAxis.ToDouble() / m.ToDouble());

	const float rotX = -0.5f*float(M_PI);//(float)(rand.Double()*M_PI/2.0);
	const float rotY = static_cast<float>(rand.Double(M_PI));
	const matrix4x4d rotA = matrix4x4d::RotateYMatrix(rotY) * matrix4x4d::RotateXMatrix(rotX);
	const matrix4x4d rotB = matrix4x4d::RotateYMatrix(rotY-M_PI) * matrix4x4d::RotateXMatrix(rotX);

	a->m_orbit = Orbit(a->orbit.eccentricity.ToDouble(), (a->orbit.semiMajorAxis * a0).ToDouble()*AU, period, rotA);
	b->m_orbit = Orbit(a->orbit.eccentricity.ToDouble(), (a->orbit.semiMajorAxis * a1).ToDouble()*AU, period, rotB);

	fixed orbMin = a->orbit.semiMajorAxis - a->orbit.eccentricity*a->orbit.semiMajorAxis;
	fixed orbMax = 2*a->orbit.semiMajorAxis - orbMin;
	a->orbit.orbMin = orbMin;
	b->orbit.orbMin = orbMin;
	a->orbit.orbMax = orbMax;
	b->orbit.orbMax = orbMax;
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

static double calc_orbital_period(double semiMajorAxis, double centralMass)
{
	return 2.0*M_PI*sqrt((semiMajorAxis*semiMajorAxis*semiMajorAxis)/(G*centralMass));
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
						body->mass / (fixedf<32>(3,1)*mprimary))));

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
			discMin = 4 * primary->radius * AU_SOL_RADIUS;
		}
		if (primary->type == SystemBody::TYPE_WHITE_DWARF) {
			// white dwarfs will have started as stars < 8 solar
			// masses or so, so pick discMax according to that
			// We give it a larger discMin because it used to be a much larger star
			discMin = 1000 * primary->radius * AU_SOL_RADIUS;
			discMax = 100 * rand.NFixed(2);		// rand-splitting again
			discMax *= fixed::SqrtOf(fixed(1,2) + fixed(8,1)*rand.Fixed());
		} else {
			discMax = 100 * rand.NFixed(2)*fixed::SqrtOf(primary->mass);
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
		fixed primary_rad = primary->radius * AU_EARTH_RADIUS;
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

		SystemBody *planet = new SystemBody(SystemBody::TYPE_PLANET_TERRESTRIAL);
		planet->seed = rand.Int32();
		planet->orbit.eccentricity = ecc;
		planet->axialTilt = fixed(100,157)*rand.NFixed(2);
		planet->orbit.semiMajorAxis = semiMajorAxis;
		planet->tmp = 0;
		planet->parent = primary;
		planet->mass = mass;
		planet->rotationPeriod = fixed(rand.Int32(1,200), 24);

		double r1 = rand.Double(2*M_PI);		// function parameter evaluation order is implementation-dependent
		double r2 = rand.NDouble(5);			// can't put two rands in the same expression
		const matrix4x4d rotMatrix = matrix4x4d::RotateYMatrix(r1) * matrix4x4d::RotateXMatrix(-0.5*M_PI + r2*M_PI/2.0);
		planet->m_orbit = Orbit(ecc.ToDouble(), semiMajorAxis.ToDouble()*AU, calc_orbital_period(semiMajorAxis.ToDouble()*AU, primary->GetMass()), rotMatrix);

		planet->orbit.orbMin = periapsis;
		planet->orbit.orbMax = apoapsis;

		m_bodies.push_back(planet);

		primary->children.push_back(planet);

		// minimum separation between planets of 1.35
		pos = apoapsis * fixed(135,100);
	}

	int idx=0;
	bool make_moons = superType <= SystemBody::SUPERTYPE_STAR;

	for (std::vector<SystemBody*>::iterator i = primary->children.begin(); i != primary->children.end(); ++i) {
		// planets around a binary pair [gravpoint] -- ignore the stars...
		if ((*i)->GetSuperType() == SystemBody::SUPERTYPE_STAR) continue;
		// Turn them into something!!!!!!!
		char buf[8];
		if (superType <= SystemBody::SUPERTYPE_STAR) {
			// planet naming scheme
			snprintf(buf, sizeof(buf), " %c", 'a'+idx);
		} else {
			// moon naming scheme
			snprintf(buf, sizeof(buf), " %d", 1+idx);
		}
		(*i)->name = primary->name+buf;
		(*i)->PickPlanetType(rand);
		if (make_moons) MakePlanetsAround(*i, rand);
		idx++;
	}
}

// Position a surface starport anywhere. Space.cpp::MakeFrameFor() ensures it
// is on dry land (discarding this position if necessary)
// XXX horror reuse of Orbit
static Orbit position_settlement_on_planet(SystemBody *b)
{
	MTRand r(b->seed);
	// used for orientation on planet surface
	double r2 = r.Double(); 	// function parameter evaluation order is implementation-dependent
	double r1 = r.Double();		// can't put two rands in the same expression

	return Orbit(0.0, 0.0, 0.0, matrix4x4d::RotateZMatrix(2*M_PI*r1) * matrix4x4d::RotateYMatrix(2*M_PI*r2));
}

void RandomSystemGenerator::PopulateAddStations(SystemBody *body)
{
	for (unsigned int i=0; i<body->children.size(); i++) {
		PopulateAddStations(body->children[i]);
	}

	unsigned long _init[6] = { m_desc.path.systemIndex, Uint32(m_desc.path.sectorX),
			Uint32(m_desc.path.sectorY), Uint32(m_desc.path.sectorZ), body->seed, UNIVERSE_SEED };

	MTRand rand, namerand;
	rand.seed(_init, 6);
	namerand.seed(_init, 6);

	if (body->m_population < fixed(1,1000)) return;

	fixed pop = body->m_population + rand.Fixed();

	fixed orbMaxS = fixed(1,4)*calc_hill_radius(body);
	fixed orbMinS = 4 * body->radius * AU_EARTH_RADIUS;
	if (body->children.size()) orbMaxS = std::min(orbMaxS, fixed(1,2) * body->children[0]->orbit.orbMin);

	// starports - orbital
	pop -= rand.Fixed();
	if ((orbMinS < orbMaxS) && (pop >= 0)) {

		SystemBody *sp = new SystemBody(SystemBody::TYPE_STARPORT_ORBITAL);
		sp->seed = rand.Int32();
		sp->tmp = 0;
		sp->parent = body;
		sp->rotationPeriod = fixed(1,3600);
		sp->averageTemp = body->averageTemp;
		sp->mass = 0;
		// just always plonk starports in near orbit
		sp->orbit.semiMajorAxis = orbMinS;
		sp->orbit.eccentricity = fixed(0);
		sp->axialTilt = fixed(0);

		const double eccentricity = 0.0;
		const double semiMajorAxis = sp->orbit.semiMajorAxis.ToDouble()*AU;
		const double period = calc_orbital_period(semiMajorAxis, body->mass.ToDouble() * EARTH_MASS);

		sp->m_orbit = Orbit(eccentricity, semiMajorAxis, period, matrix4x4d::Identity());

		sp->orbit.orbMin = sp->orbit.semiMajorAxis;
		sp->orbit.orbMax = sp->orbit.semiMajorAxis;

		sp->name = Pi::luaNameGen->BodyName(sp, namerand);

		m_bodies.push_back(sp);
		body->children.insert(body->children.begin(), sp);

		pop -= rand.Fixed();
		if (pop > 0) {
			// XXX horrid
			SystemBody *sp2 = new SystemBody(sp->type);
			SystemPath path2 = sp2->path;
			*sp2 = *sp;
			sp2->path = path2;
			sp2->m_orbit = Orbit(eccentricity, semiMajorAxis, period, matrix4x4d::RotateZMatrix(M_PI));
			sp2->name = Pi::luaNameGen->BodyName(sp2, namerand);

			m_bodies.push_back(sp2);
			body->children.insert(body->children.begin(), sp2);
		}
	}

	// starports - surface
	pop = body->m_population + rand.Fixed();
	int max = 6;
	while (max-- > 0) {
		pop -= rand.Fixed();
		if (pop < 0) break;

		SystemBody *sp = new SystemBody(SystemBody::TYPE_STARPORT_SURFACE);
		sp->seed = rand.Int32();
		sp->tmp = 0;
		sp->parent = body;
		sp->averageTemp = body->averageTemp;
		sp->mass = 0;
		sp->name = Pi::luaNameGen->BodyName(sp, namerand);

		// XXX horror reuse of Orbit
		sp->m_orbit = position_settlement_on_planet(sp);

		m_bodies.push_back(sp);
		body->children.insert(body->children.begin(), sp);
	}
}
