#include "SystemBody.h"
#include "StarSystem.h"
#include "SystemConstants.h"
#include "Lang.h"
#include "Pi.h"
#include "LuaNameGen.h"
#include "enum_table.h"
#include <iostream>
#include <cassert>

SystemBody::SystemBody(BodyType _type, const PhysicalData &_phys) :
	parent(0),
	type(_type),
	seed(0),
	phys(_phys),
	heightMapFilename(0),
	heightMapFractal(0),
	m_atmosDensity(0.0)
{
	assert(type >= TYPE_GRAVPOINT && type < TYPE_LAST);
}

static const int CELSIUS = 273;

SystemBody::BodySuperType SystemBody::GetSuperType() const
{
	if (type >= TYPE_STAR_MIN && type <= TYPE_STAR_MAX)
		return SUPERTYPE_STAR;

	if (type >= TYPE_ROCKY_PLANET_MIN && type <= TYPE_ROCKY_PLANET_MAX)
		return SUPERTYPE_ROCKY_PLANET;

	if (type >= TYPE_GAS_GIANT_MIN && type <= TYPE_GAS_GIANT_MAX)
		return SUPERTYPE_GAS_GIANT;

	if (type >= TYPE_STARPORT_MIN && type <= TYPE_STARPORT_MAX)
		return SUPERTYPE_STARPORT;

	return SUPERTYPE_NONE;
}

static const char *body_descriptions[SystemBody::TYPE_LAST] = {
	0, // GRAVPOINT
	Lang::BROWN_DWARF,
	Lang::WHITE_DWARF,
	Lang::STAR_M,
	Lang::STAR_K,
	Lang::STAR_G,
	Lang::STAR_F,
	Lang::STAR_A,
	Lang::STAR_B,
	Lang::STAR_O,
	Lang::STAR_M_GIANT,
	Lang::STAR_K_GIANT,
	Lang::STAR_G_GIANT,
	Lang::STAR_AF_GIANT,
	Lang::STAR_AF_GIANT,
	Lang::STAR_B_GIANT,
	Lang::STAR_O_GIANT,
	Lang::STAR_M_SUPER_GIANT,
	Lang::STAR_K_SUPER_GIANT,
	Lang::STAR_G_SUPER_GIANT,
	Lang::STAR_AF_SUPER_GIANT,
	Lang::STAR_AF_SUPER_GIANT,
	Lang::STAR_B_SUPER_GIANT,
	Lang::STAR_O_SUPER_GIANT,
	Lang::STAR_M_HYPER_GIANT,
	Lang::STAR_K_HYPER_GIANT,
	Lang::STAR_G_HYPER_GIANT,
	Lang::STAR_AF_HYPER_GIANT,
	Lang::STAR_AF_HYPER_GIANT,
	Lang::STAR_B_HYPER_GIANT,
	Lang::STAR_O_HYPER_GIANT,
	Lang::STAR_M_WF,
	Lang::STAR_B_WF,
	Lang::STAR_O_WF,
	Lang::STAR_S_BH,
	Lang::STAR_IM_BH,
	Lang::STAR_SM_BH,
	0, // PLANET_GAS_GIANT
	Lang::ASTEROID,
	0, // PLANET_TERRESTRIAL
	Lang::ORBITAL_STARPORT,
	Lang::STARPORT
};

std::string SystemBody::GetAstroDescription() const
{
	if (body_descriptions[type])
		return body_descriptions[type];

	switch (type) {

		case TYPE_PLANET_GAS_GIANT:
			if (phys.mass > 800) return Lang::VERY_LARGE_GAS_GIANT;
			if (phys.mass > 300) return Lang::LARGE_GAS_GIANT;
			if (phys.mass > 80) return Lang::MEDIUM_GAS_GIANT;
			else return Lang::SMALL_GAS_GIANT;

		case TYPE_PLANET_TERRESTRIAL: {
			std::string s;
			if (phys.mass > fixed(2,1)) s = Lang::MASSIVE;
			else if (phys.mass > fixed(3,2)) s = Lang::LARGE;
			else if (phys.mass < fixed(1,10)) s = Lang::TINY;
			else if (phys.mass < fixed(1,5)) s = Lang::SMALL;

			if (composition.volcanicity > fixed(7,10)) {
				if (s.size()) s += Lang::COMMA_HIGHLY_VOLCANIC;
				else s = Lang::HIGHLY_VOLCANIC;
			}

			if (composition.volatileIces + composition.volatileLiquid > fixed(4,5)) {
				if (composition.volatileIces > composition.volatileLiquid) {
					if (phys.averageTemp < fixed(250)) {
						s += Lang::ICE_WORLD;
					} else s += Lang::ROCKY_PLANET;
				} else {
					if (phys.averageTemp < fixed(250)) {
						s += Lang::ICE_WORLD;
					} else {
						s += Lang::OCEANICWORLD;
					}
				}
			} else if (composition.volatileLiquid > fixed(2,5)){
				if (phys.averageTemp > fixed(250)) {
					s += Lang::PLANET_CONTAINING_LIQUID_WATER;
				} else {
					s += Lang::PLANET_WITH_SOME_ICE;
				}
			} else if (composition.volatileLiquid > fixed(1,5)){
				s += Lang::ROCKY_PLANET_CONTAINING_COME_LIQUIDS;
			} else {
				s += Lang::ROCKY_PLANET;
			}

			if (composition.volatileGas < fixed(1,100)) {
				s += Lang::WITH_NO_SIGNIFICANT_ATMOSPHERE;
			} else {
				std::string thickness;
				if (composition.volatileGas < fixed(1,10)) thickness = Lang::TENUOUS;
				else if (composition.volatileGas < fixed(1,5)) thickness = Lang::THIN;
				else if (composition.volatileGas < fixed(2,1)) {}
				else if (composition.volatileGas < fixed(4,1)) thickness = Lang::THICK;
				else thickness = Lang::VERY_DENSE;

				if (composition.atmosOxidizing > fixed(95,100)) {
					s += Lang::WITH_A+thickness+Lang::O2_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(7,10)) {
					s += Lang::WITH_A+thickness+Lang::CO2_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(65,100)) {
					s += Lang::WITH_A+thickness+Lang::CO_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(55,100)) {
					s += Lang::WITH_A+thickness+Lang::CH4_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(3,10)) {
					s += Lang::WITH_A+thickness+Lang::H_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(2,10)) {
					s += Lang::WITH_A+thickness+Lang::HE_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(15,100)) {
					s += Lang::WITH_A+thickness+Lang::AR_ATMOSPHERE;
				} else if (composition.atmosOxidizing > fixed(1,10)) {
					s += Lang::WITH_A+thickness+Lang::S_ATMOSPHERE;
				} else {
					s += Lang::WITH_A+thickness+Lang::N_ATMOSPHERE;
				}
			}

			if (composition.life > fixed(1,2)) {
				s += Lang::AND_HIGHLY_COMPLEX_ECOSYSTEM;
			} else if (composition.life > fixed(1,10)) {
				s += Lang::AND_INDIGENOUS_PLANT_LIFE;
			} else if (composition.life > fixed(0)) {
				s += Lang::AND_INDIGENOUS_MICROBIAL_LIFE;
			} else {
				s += ".";
			}

			return s;
		}

		default:
			return Lang::UNKNOWN;
	}
}

static const char *body_icons[SystemBody::TYPE_LAST] = {
	0, // GRAVPOINT
	"icons/object_brown_dwarf.png",
	"icons/object_white_dwarf.png",
	"icons/object_star_m.png",
	"icons/object_star_k.png",
	"icons/object_star_g.png",
	"icons/object_star_f.png",
	"icons/object_star_a.png",
	"icons/object_star_b.png",
	"icons/object_star_b.png", //shares B graphic for now
	"icons/object_star_m_giant.png",
	"icons/object_star_k_giant.png",
	"icons/object_star_g_giant.png",
	"icons/object_star_f_giant.png",
	"icons/object_star_a_giant.png",
	"icons/object_star_b_giant.png",
	"icons/object_star_o.png", // uses old O type graphic
	"icons/object_star_m_super_giant.png",
	"icons/object_star_k_super_giant.png",
	"icons/object_star_g_super_giant.png",
	"icons/object_star_g_super_giant.png", //shares G graphic for now
	"icons/object_star_a_super_giant.png",
	"icons/object_star_b_super_giant.png",
	"icons/object_star_b_super_giant.png",// uses B type graphic for now
	"icons/object_star_m_hyper_giant.png",
	"icons/object_star_k_hyper_giant.png",
	"icons/object_star_g_hyper_giant.png",
	"icons/object_star_f_hyper_giant.png",
	"icons/object_star_a_hyper_giant.png",
	"icons/object_star_b_hyper_giant.png",
	"icons/object_star_b_hyper_giant.png",// uses B type graphic for now
	"icons/object_star_m_wf.png",
	"icons/object_star_b_wf.png",
	"icons/object_star_o_wf.png",
	"icons/object_star_bh.png",
	"icons/object_star_smbh.png",
	"icons/object_star_smbh.png",
	0, // PLANET_GAS_GIANT
	"icons/object_planet_asteroid.png",
	0, // PLANET_TERRESTRIAL
	"icons/object_orbital_starport.png",
	0
};

const char *SystemBody::GetIcon() const
{
	if (body_icons[type])
		return body_icons[type];

	switch (type) {
		case TYPE_PLANET_GAS_GIANT:
			if (phys.mass > 800) {
				if (phys.averageTemp > 1000) return "icons/object_planet_large_gas_giant_hot.png";
				else return "icons/object_planet_large_gas_giant.png";
			}
			if (phys.mass > 300) {
				if (phys.averageTemp > 1000) return "icons/object_planet_large_gas_giant_hot.png";
				else return "icons/object_planet_large_gas_giant.png";
			}
			if (phys.mass > 80) {
				if (phys.averageTemp > 1000) return "icons/object_planet_medium_gas_giant_hot.png";
				else return "icons/object_planet_medium_gas_giant.png";
			}
			else {
				if (phys.averageTemp > 1000) return "icons/object_planet_small_gas_giant_hot.png";
				else return "icons/object_planet_small_gas_giant.png";
			}

		case TYPE_PLANET_TERRESTRIAL:
			if (composition.volatileLiquid > fixed(7,10)) {
				if (phys.averageTemp > 250) return "icons/object_planet_water.png";
				else return "icons/object_planet_ice.png";
			}
			if ((composition.life > fixed(9,10)) &&
				(composition.volatileGas > fixed(6,10))) return "icons/object_planet_life.png";
			if ((composition.life > fixed(8,10)) &&
				(composition.volatileGas > fixed(5,10))) return "icons/object_planet_life6.png";
			if ((composition.life > fixed(7,10)) &&
				(composition.volatileGas > fixed(45,100))) return "icons/object_planet_life7.png";
			if ((composition.life > fixed(6,10)) &&
				(composition.volatileGas > fixed(4,10))) return "icons/object_planet_life8.png";
			if ((composition.life > fixed(5,10)) &&
				(composition.volatileGas > fixed(3,10))) return "icons/object_planet_life4.png";
			if ((composition.life > fixed(4,10)) &&
				(composition.volatileGas > fixed(2,10))) return "icons/object_planet_life5.png";
			if ((composition.life > fixed(1,10)) &&
				(composition.volatileGas > fixed(2,10))) return "icons/object_planet_life2.png";
			if (composition.life > fixed(1,10)) return "icons/object_planet_life3.png";
			if (phys.mass < fixed(1,100)) return "icons/object_planet_dwarf.png";
			if (phys.mass < fixed(1,10)) return "icons/object_planet_small.png";
			if ((composition.volatileLiquid < fixed(1,10)) &&
				(composition.volatileGas > fixed(1,5))) return "icons/object_planet_desert.png";

			if (composition.volatileIces + composition.volatileLiquid > fixed(3,5)) {
				if (composition.volatileIces > composition.volatileLiquid) {
					if (phys.averageTemp < 250)	return "icons/object_planet_ice.png";
				} else {
					if (phys.averageTemp > 250) {
						return "icons/object_planet_water.png";
					} else return "icons/object_planet_ice.png";
				}
			}

			if (composition.volatileGas > fixed(1,2)) {
				if (composition.atmosOxidizing < fixed(1,2)) {
					if (phys.averageTemp > 300) return "icons/object_planet_methane3.png";
					else if (phys.averageTemp > 250) return "icons/object_planet_methane2.png";
					else return "icons/object_planet_methane.png";
				} else {
					if (phys.averageTemp > 300) return "icons/object_planet_co2_2.png";
					else if (phys.averageTemp > 250) {
						if ((composition.volatileLiquid > fixed(3,10)) && (composition.volatileGas > fixed(2,10)))
							return "icons/object_planet_co2_4.png";
						else return "icons/object_planet_co2_3.png";
					} else return "icons/object_planet_co2.png";
				}
			}

			if ((composition.volatileLiquid > fixed(1,10)) &&
			(composition.volatileGas < fixed(1,10))) return "icons/object_planet_ice.png";
			if (composition.volcanicity > fixed(7,10)) return "icons/object_planet_volcanic.png";
			return "icons/object_planet_small.png";
			/*
			"icons/object_planet_water_n1.png"
			"icons/object_planet_life3.png"
			"icons/object_planet_life2.png"
			*/

		default:
			return 0;
	}
}

double SystemBody::GetMaxChildOrbitalDistance() const
{
	double max = 0;
	for (unsigned int i=0; i<children.size(); i++) {
		if (children[i]->orbit.orbMax.ToDouble() > max) {
			max = children[i]->orbit.orbMax.ToDouble();
		}
	}
	return AU * max;
}

double SystemBody::CalcSurfaceGravity() const
{
	double r = GetRadius();
	if (r > 0.0) {
		return G * GetMass() / pow(r, 2);
	} else {
		return 0.0;
	}
}

bool SystemBody::HasAtmosphere() const
{
	return (composition.volatileGas > fixed(1,100));
}

bool SystemBody::IsScoopable() const
{
	return (GetSuperType() == SUPERTYPE_GAS_GIANT);
}

void SystemBody::PickAtmosphere()
{
	// Alpha value isn't real alpha. in the shader fog depth is determined
	// by density*alpha, so that we can have very dense atmospheres
	// without having them a big stinking solid color obscuring everything
	//
	// These are our atmosphere colours, for terrestrial planets we use m_atmosOxidizing
	// for some variation to atmosphere colours
	switch (type) {
		case SystemBody::TYPE_PLANET_GAS_GIANT:
			m_atmosColor = Color(1.0f, 1.0f, 1.0f, 0.0005f);
			m_atmosDensity = 14.0;
			break;
		case SystemBody::SUPERTYPE_STAR:
		case SystemBody::TYPE_PLANET_ASTEROID:
			m_atmosColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
			m_atmosDensity = 0.0;
			break;
		default:
		case SystemBody::TYPE_PLANET_TERRESTRIAL:
			double r = 0, g = 0, b = 0;
			double atmo = composition.atmosOxidizing.ToDouble();
			if (composition.volatileGas.ToDouble() > 0.001) {
				if (atmo > 0.95) {
					// o2
					r = 1.0f + ((0.95f-atmo)*15.0f);
					g = 0.95f + ((0.95f-atmo)*10.0f);
					b = atmo*atmo*atmo*atmo*atmo;
					m_atmosColor = Color(r, g, b, 1.0);
				} else if (atmo > 0.7) {
					// co2
					r = atmo+0.05f;
					g = 1.0f + (0.7f-atmo);
					b = 0.8f;
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.65) {
					// co
					r = 1.0f + (0.65f-atmo);
					g = 0.8f;
					b = atmo + 0.25f;
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.55) {
					// ch4
					r = 1.0f + ((0.55f-atmo)*5.0);
					g = 0.35f - ((0.55f-atmo)*5.0);
					b = 0.4f;
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.3) {
					// h
					r = 1.0f;
					g = 1.0f;
					b = 1.0f;
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.2) {
					// he
					r = 1.0f;
					g = 1.0f;
					b = 1.0f;
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.15) {
					// ar
					r = 0.5f - ((0.15f-atmo)*5.0);
					g = 0.0f;
					b = 0.5f + ((0.15f-atmo)*5.0);
					m_atmosColor = Color(r, g, b, 1.0f);
				} else if (atmo > 0.1) {
					// s
					r = 0.8f - ((0.1f-atmo)*4.0);
					g = 1.0f;
					b = 0.5f - ((0.1f-atmo)*10.0);
					m_atmosColor = Color(r, g, b, 1.0f);
				} else {
					// n
					r = 1.0f;
					g = 1.0f;
					b = 1.0f;
					m_atmosColor = Color(r, g, b, 1.0f);
				}
			} else {
				m_atmosColor = Color(0.0, 0.0, 0.0, 0.0f);
			}
			m_atmosDensity = composition.volatileGas.ToDouble();
			break;

		/*default:
			m_atmosColor = Color(0.6f, 0.6f, 0.6f, 1.0f);
			m_atmosDensity = composition.body->composition.volatileGas.ToDouble();
			break;*/
	}
}

// For moons distance from star is not orbMin, orbMax.
const SystemBody *SystemBody::FindStarAndTrueOrbitalRange(fixed &orbMin_, fixed &orbMax_)
{
	const SystemBody *planet = this;
	const SystemBody *star = this->parent;

	assert(star);

	// while not found star yet..
	while (star->GetSuperType() > SystemBody::SUPERTYPE_STAR) {
		planet = star;
		star = star->parent;
	}

	orbMin_ = planet->orbit.orbMin;
	orbMax_ = planet->orbit.orbMax;
	return star;
}

// star_phys.radius in sol radii
// star_temp in kelvin,
// object_dist in AU
// return Watts/m^2
static fixed calcEnergyPerUnitAreaAtDist(fixed star_radius, int star_temp, fixed object_dist)
{
	fixed temp = star_temp * fixed(1,10000);
	const fixed total_solar_emission =
		temp*temp*temp*temp*star_radius*star_radius;

	return fixed(1744665451,100000)*(total_solar_emission / (object_dist*object_dist));
}

static int CalcSurfaceTemp(const SystemBody *primary, fixed distToPrimary, fixed albedo, fixed greenhouse)
{
	fixed energy_per_meter2;
	if (primary->type == SystemBody::TYPE_GRAVPOINT) {
		// binary. take energies of both stars
		energy_per_meter2 = calcEnergyPerUnitAreaAtDist(primary->children[0]->phys.radius,
			primary->children[0]->phys.averageTemp, distToPrimary);
		energy_per_meter2 += calcEnergyPerUnitAreaAtDist(primary->children[1]->phys.radius,
			primary->children[1]->phys.averageTemp, distToPrimary);
	} else {
		energy_per_meter2 = calcEnergyPerUnitAreaAtDist(primary->phys.radius, primary->phys.averageTemp, distToPrimary);
	}
	const fixed surface_temp_pow4 = energy_per_meter2*(1-albedo)/(1-greenhouse);
	return int(isqrt(isqrt((surface_temp_pow4.v>>fixed::FRAC)*4409673)));
}

void SystemBody::PickPlanetType(MTRand &rand)
{
	fixed albedo = fixed(0);
	fixed greenhouse = fixed(0);

	fixed minDistToStar, maxDistToStar, averageDistToStar;
	const SystemBody *star = FindStarAndTrueOrbitalRange(minDistToStar, maxDistToStar);
	averageDistToStar = (minDistToStar+maxDistToStar)>>1;

	/* first calculate blackbody temp (no greenhouse effect, zero albedo) */
	int bbody_temp = CalcSurfaceTemp(star, averageDistToStar, albedo, greenhouse);

	phys.averageTemp = bbody_temp;

	// phys.radius is just the cube root of the phys.mass. we get some more fractional
	// bits for small bodies otherwise we can easily end up with 0 phys.radius
	// which breaks stuff elsewhere
	if (phys.mass <= fixed(1,1))
		phys.radius = fixed(fixedf<48>::CubeRootOf(fixedf<48>(phys.mass)));
	else
		phys.radius = fixed::CubeRootOf(phys.mass);
	// enforce minimum size of 10km
	phys.radius = std::max(phys.radius, fixed(1,630));

	if (parent->type <= TYPE_STAR_MAX)
		// get it from the table now rather than setting it on stars/gravpoints as
		// currently nothing else needs them to have metallicity
		composition.metallicity = SystemConstants::starMetallicities[parent->type] * rand.Fixed();
	else
		// this assumes the parent's parent is a star/gravpoint, which is currently always true
		composition.metallicity = SystemConstants::starMetallicities[parent->parent->type] * rand.Fixed();
	// harder to be volcanic when you are tiny (you cool down)
	composition.volcanicity = std::min(fixed(1,1), phys.mass) * rand.Fixed();
	composition.atmosOxidizing = rand.Fixed();
	composition.life = fixed(0);
	composition.volatileGas = fixed(0);
	composition.volatileLiquid = fixed(0);
	composition.volatileIces = fixed(0);

	// pick body type
	if (phys.mass > 317*13) {
		// more than 13 jupiter phys.masses can fuse deuterium - is a brown dwarf
		type = SystemBody::TYPE_BROWN_DWARF;
		phys.averageTemp = phys.averageTemp + rand.Int32(SystemConstants::starTypeInfo[type].tempMin,
					SystemConstants::starTypeInfo[type].tempMax);
		// prevent phys.mass exceeding 65 jupiter phys.masses or so, when it becomes a star
		// XXX since TYPE_BROWN_DWARF is supertype star, phys.mass is now in
		// solar phys.masses. what a fucking mess
		phys.mass = std::min(phys.mass, fixed(317*65, 1)) / 332998;
		//phys.radius is too high as it now uses the planetary calculations to work out phys.radius (Cube root of phys.mass)
		// So tell it to use the star data instead:
		phys.radius = fixed(rand.Int32(SystemConstants::starTypeInfo[type].radius[0],
				SystemConstants::starTypeInfo[type].radius[1]), 100);
	} else if (phys.mass > 6) {
		type = SystemBody::TYPE_PLANET_GAS_GIANT;
	} else if (phys.mass > fixed(1, 15000)) {
		type = SystemBody::TYPE_PLANET_TERRESTRIAL;

		fixed amount_volatiles = fixed(2,1)*rand.Fixed();
		if (rand.Int32(3)) amount_volatiles *= phys.mass;
		// total atmosphere loss
		if (rand.Fixed() > phys.mass) amount_volatiles = fixed(0);

		// fudge how much of the volatiles are in which state
		greenhouse = fixed(0);
		albedo = fixed(0);
		// CO2 sublimation
		if (phys.averageTemp > 195) greenhouse += amount_volatiles * fixed(1,3);
		else albedo += fixed(2,6);
		// H2O liquid
		if (phys.averageTemp > 273) greenhouse += amount_volatiles * fixed(1,5);
		else albedo += fixed(3,6);
		// H2O boils
		if (phys.averageTemp > 373) greenhouse += amount_volatiles * fixed(1,3);

		phys.averageTemp = CalcSurfaceTemp(star, averageDistToStar, albedo, greenhouse);

		const fixed proportion_gas = phys.averageTemp / (fixed(100,1) + phys.averageTemp);
		composition.volatileGas = proportion_gas * amount_volatiles;

		const fixed proportion_liquid = (fixed(1,1)-proportion_gas) * (phys.averageTemp / (fixed(50,1) + phys.averageTemp));
		composition.volatileLiquid = proportion_liquid * amount_volatiles;

		const fixed proportion_ices = fixed(1,1) - (proportion_gas + proportion_liquid);
		composition.volatileIces = proportion_ices * amount_volatiles;

		if ((composition.volatileLiquid > fixed(0)) &&
		    (phys.averageTemp > CELSIUS-60) &&
		    (phys.averageTemp < CELSIUS+200)) {
			// try for life
			int minTemp = CalcSurfaceTemp(star, maxDistToStar, albedo, greenhouse);
			int maxTemp = CalcSurfaceTemp(star, minDistToStar, albedo, greenhouse);

			if ((star->type != TYPE_BROWN_DWARF) &&
			    (star->type != TYPE_WHITE_DWARF) &&
			    (star->type != TYPE_STAR_O) &&
			    (minTemp > CELSIUS-10) && (minTemp < CELSIUS+90) &&
			    (maxTemp > CELSIUS-10) && (maxTemp < CELSIUS+90)) {
				composition.life = rand.Fixed();
			}
		}
	} else {
		type = SystemBody::TYPE_PLANET_ASTEROID;
	}

    PickAtmosphere();
}

static const unsigned char RANDOM_RING_COLORS[][4] = {
	{ 156, 122,  98, 217 }, // jupiter-like
	{ 156, 122,  98, 217 }, // saturn-like
	{ 181, 173, 174, 217 }, // neptune-like
	{ 130, 122,  98, 217 }, // uranus-like
	{ 207, 122,  98, 217 }  // brown dwarf-like
};

void SystemBody::PickRings(bool forceRings)
{
	m_rings.minRadius = fixed(0);
	m_rings.maxRadius = fixed(0);
	m_rings.baseColor = Color4ub(255,255,255,255);

	if (type == SystemBody::TYPE_PLANET_GAS_GIANT) {
		MTRand ringRng(seed + 965467);

		// today's forecast: 50% chance of rings
		double rings_die = ringRng.Double();
		if (forceRings || (rings_die < 0.5)) {
			const unsigned char * const baseCol
				= RANDOM_RING_COLORS[ringRng.Int32(COUNTOF(RANDOM_RING_COLORS))];
			m_rings.baseColor.r = Clamp(baseCol[0] + ringRng.Int32(-20,20), 0, 255);
			m_rings.baseColor.g = Clamp(baseCol[1] + ringRng.Int32(-20,20), 0, 255);
			m_rings.baseColor.b = Clamp(baseCol[2] + ringRng.Int32(-20,10), 0, 255);
			m_rings.baseColor.a = Clamp(baseCol[3] + ringRng.Int32(-5,5), 0, 255);

			// from wikipedia: http://en.wikipedia.org/wiki/Roche_limit
			// basic Roche limit calculation assuming a rigid satellite
			// d = R (2 p_M / p_m)^{1/3}
			// 
			// where R is the radius of the primary, p_M is the density of
			// the primary and p_m is the density of the satellite
			//
			// I assume a satellite density of 500 kg/m^3
			// (which Wikipedia says is an average comet density)
			//
			// also, I can't be bothered to think about unit conversions right now,
			// so I'm going to ignore the real density of the primary and take it as 1100 kg/m^3
			// (note: density of Saturn is ~687, Jupiter ~1,326, Neptune ~1,638, Uranus ~1,318)
			//
			// This gives: d = 1.638642 * R
			fixed innerMin = fixed(110, 100);
			fixed innerMax = fixed(145, 100);
			fixed outerMin = fixed(150, 100);
			fixed outerMax = fixed(168642, 100000);

			m_rings.minRadius = innerMin + (innerMax - innerMin)*ringRng.Fixed();
			m_rings.maxRadius = outerMin + (outerMax - outerMin)*ringRng.Fixed();
		}
	}
}

// Calculate parameters used in the atmospheric model for shaders
// used by both LmrModels and Geosphere
SystemBody::AtmosphereParameters SystemBody::CalcAtmosphereParams() const
{
	AtmosphereParameters params;

	double atmosDensity;

	GetAtmosphereFlavor(&params.atmosCol, &atmosDensity);
	// adjust global atmosphere opacity
	atmosDensity *= 1e-5;

	params.atmosDensity = static_cast<float>(atmosDensity);

	// Calculate parameters used in the atmospheric model for shaders
	// Isothermal atmospheric model
	// See http://en.wikipedia.org/wiki/Atmospheric_pressure#Altitude_atmospheric_pressure_variation
	// This model features an exponential decrease in pressure and density with altitude.
	// The scale height is 1/the exponential coefficient.

	// The equation for pressure is:
	// Pressure at height h = Pressure surface * e^((-Mg/RT)*h)

	// calculate (inverse) atmosphere scale height		
	// The formula for scale height is:
	// h = RT / Mg
	// h is height above the surface in meters
	// R is the universal gas constant
	// T is the surface temperature in Kelvin
	// g is the gravity in m/s^2
	// M is the molar mass of air in kg/mol

	// calculate gravity
	// radius of the planet
	const double radiusPlanet_in_m = (phys.radius.ToDouble()*EARTH_RADIUS);
	const double massPlanet_in_kg = (phys.mass.ToDouble()*EARTH_MASS);
	const double g = G*massPlanet_in_kg/(radiusPlanet_in_m*radiusPlanet_in_m);

	const double T = static_cast<double>(phys.averageTemp);

	// XXX just use earth's composition for now
	const double M = 0.02897f; // in kg/mol

	const float atmosScaleHeight = static_cast<float>(GAS_CONSTANT_R*T/(M*g));

	// min of 2.0 corresponds to a scale height of 1/20 of the planet's radius,
	params.atmosInvScaleHeight = std::max(20.0f, static_cast<float>(GetRadius() / atmosScaleHeight));
	// integrate atmospheric density between surface and this radius. this is 10x the scale
	// height, which should be a height at which the atmospheric density is negligible
	params.atmosRadius = 1.0f + static_cast<float>(10.0f * atmosScaleHeight) / GetRadius();

	params.planetRadius = static_cast<float>(radiusPlanet_in_m);

	return params;
}


SystemBody::~SystemBody()
{
	for (std::vector<SystemBody*>::iterator i = children.begin(); i != children.end(); ++i) {
		delete (*i);
	}
}

Orbit SystemBody::GetOrbit() const
{
	if (!parent) return Orbit();

	if (parent->type == TYPE_GRAVPOINT)
		return Orbit(orbit.eccentricity.ToDouble(), orbit.semiMajorAxis.ToDouble()*AU, parent->children[0]->GetMass()+parent->children[1]->GetMass(), orbit.position);
	
	return Orbit(orbit.eccentricity.ToDouble(), orbit.semiMajorAxis.ToDouble()*AU, parent->GetMass()+GetMass(), orbit.position);
}
