#include "LuaConstants.h"
#include "StarSystem.h"
#include "Polit.h"
#include "EquipType.h"

#define _setfield(v,k) do { lua_pushinteger(L, v); lua_setfield(L, -2, k); } while(0)

void LuaConstants::RegisterConstants(lua_State *L)
{
	lua_newtable(L); // SBody

	lua_newtable(L); // BodyType
	_setfield(SBody::TYPE_GRAVPOINT,                    "TYPE_GRAVPOINT");
	_setfield(SBody::TYPE_BROWN_DWARF,                  "TYPE_BROWN_DWARF");
	_setfield(SBody::TYPE_STAR_M,                       "TYPE_STAR_M");
	_setfield(SBody::TYPE_STAR_K,                       "TYPE_STAR_K");
	_setfield(SBody::TYPE_STAR_G,                       "TYPE_STAR_G");
	_setfield(SBody::TYPE_STAR_F,                       "TYPE_STAR_F");
	_setfield(SBody::TYPE_STAR_A,                       "TYPE_STAR_A");
	_setfield(SBody::TYPE_STAR_B,                       "TYPE_STAR_B");
	_setfield(SBody::TYPE_STAR_O,                       "TYPE_STAR_O");
	_setfield(SBody::TYPE_STAR_M_GIANT,                 "TYPE_STAR_M_GIANT");
	_setfield(SBody::TYPE_WHITE_DWARF,                  "TYPE_WHITE_DWARF");
	_setfield(SBody::TYPE_PLANET_SMALL_GAS_GIANT,       "TYPE_PLANET_SMALL_GAS_GIANT");
	_setfield(SBody::TYPE_PLANET_MEDIUM_GAS_GIANT,      "TYPE_PLANET_MEDIUM_GAS_GIANT");
	_setfield(SBody::TYPE_PLANET_LARGE_GAS_GIANT,       "TYPE_PLANET_LARGE_GAS_GIANT");
	_setfield(SBody::TYPE_PLANET_VERY_LARGE_GAS_GIANT,  "TYPE_PLANET_VERY_LARGE_GAS_GIANT");
	_setfield(SBody::TYPE_PLANET_ASTEROID,              "TYPE_PLANET_ASTEROID");
	_setfield(SBody::TYPE_PLANET_LARGE_ASTEROID,        "TYPE_PLANET_LARGE_ASTEROID");
	_setfield(SBody::TYPE_PLANET_DWARF,                 "TYPE_PLANET_DWARF");
	_setfield(SBody::TYPE_PLANET_SMALL,                 "TYPE_PLANET_SMALL");
	_setfield(SBody::TYPE_PLANET_WATER,                 "TYPE_PLANET_WATER");
	_setfield(SBody::TYPE_PLANET_DESERT,                "TYPE_PLANET_DESERT");
	_setfield(SBody::TYPE_PLANET_CO2,                   "TYPE_PLANET_CO2");
	_setfield(SBody::TYPE_PLANET_METHANE,               "TYPE_PLANET_METHANE");
	_setfield(SBody::TYPE_PLANET_WATER_THICK_ATMOS,     "TYPE_PLANET_WATER_THICK_ATMOS");
	_setfield(SBody::TYPE_PLANET_CO2_THICK_ATMOS,       "TYPE_PLANET_CO2_THICK_ATMOS");
	_setfield(SBody::TYPE_PLANET_METHANE_THICK_ATMOS,   "TYPE_PLANET_METHANE_THICK_ATMOS");
	_setfield(SBody::TYPE_PLANET_HIGHLY_VOLCANIC,       "TYPE_PLANET_HIGHLY_VOLCANIC");
	_setfield(SBody::TYPE_PLANET_INDIGENOUS_LIFE,       "TYPE_PLANET_INDIGENOUS_LIFE");
	_setfield(SBody::TYPE_PLANET_TERRAFORMED_POOR,      "TYPE_PLANET_TERRAFORMED_POOR");
	_setfield(SBody::TYPE_PLANET_TERRAFORMED_GOOD,      "TYPE_PLANET_TERRAFORMED_GOOD");
	_setfield(SBody::TYPE_STARPORT_ORBITAL,             "TYPE_STARPORT_ORBITAL");
	_setfield(SBody::TYPE_STARPORT_SURFACE,             "TYPE_STARPORT_SURFACE");
	_setfield(SBody::TYPE_MAX,                          "TYPE_MAX");
	_setfield(SBody::TYPE_STAR_MIN,                     "TYPE_STAR_MIN");
	_setfield(SBody::TYPE_STAR_MAX,                     "TYPE_STAR_MAX");
	lua_setfield(L, -2, "BodyType");

	lua_newtable(L); // BodySuperType
	_setfield(SBody::SUPERTYPE_NONE,         "SUPERTYPE_NONE");
	_setfield(SBody::SUPERTYPE_STAR,         "SUPERTYPE_STAR");
	_setfield(SBody::SUPERTYPE_ROCKY_PLANET, "SUPERTYPE_ROCKY_PLANET");
	_setfield(SBody::SUPERTYPE_GAS_GIANT,    "SUPERTYPE_GAS_GIANT");
	_setfield(SBody::SUPERTYPE_STARPORT,     "SUPERTYPE_STARPORT");
	lua_setfield(L, -2, "BodySuperType");

	lua_setfield(L, LUA_GLOBALSINDEX, "SBody");


	lua_newtable(L); // Polit

	lua_newtable(L); // Crime
	_setfield(Polit::CRIME_TRADING_ILLEGAL_GOODS, "CRIME_TRADING_ILLEGAL_GOODS");
	_setfield(Polit::CRIME_WEAPON_DISCHARGE,      "CRIME_WEAPON_DISCHARGE");
	_setfield(Polit::CRIME_PIRACY,                "CRIME_PIRACY");
	_setfield(Polit::CRIME_MURDER,                "CRIME_MURDER");
	lua_setfield(L, -2, "Crime");

	lua_newtable(L); // Bloc
	_setfield(Polit::BLOC_NONE,     "BLOC_NONE");
	_setfield(Polit::BLOC_EARTHFED, "BLOC_EARTHFED");
	_setfield(Polit::BLOC_CIS,      "BLOC_CIS");
	_setfield(Polit::BLOC_EMPIRE,   "BLOC_EMPIRE");
	_setfield(Polit::BLOC_EMPIRE,   "BLOC_MAX");
	lua_setfield(L, -2, "Bloc");

	lua_newtable(L); // EconType
	_setfield(Polit::ECON_NONE,            "ECON_NONE");
	_setfield(Polit::ECON_VERY_CAPITALIST, "ECON_VERY_CAPITALIST");
	_setfield(Polit::ECON_CAPITALIST,      "ECON_CAPITALIST");
	_setfield(Polit::ECON_MIXED,           "ECON_MIXED");
	_setfield(Polit::ECON_PLANNED,         "ECON_PLANNED");
	_setfield(Polit::ECON_MAX,             "ECON_MAX");
	lua_setfield(L, -2, "EconType");

	lua_newtable(L); // GovType
	_setfield(Polit::GOV_INVALID,       "GOV_INVALID,");
	_setfield(Polit::GOV_NONE,          "GOV_NONE,");
	_setfield(Polit::GOV_EARTHCOLONIAL, "GOV_EARTHCOLONIAL,");
	_setfield(Polit::GOV_EARTHDEMOC,    "GOV_EARTHDEMOC,");
	_setfield(Polit::GOV_EMPIRERULE,    "GOV_EMPIRERULE,");
	_setfield(Polit::GOV_CISLIBDEM,     "GOV_CISLIBDEM,");
	_setfield(Polit::GOV_CISSOCDEM,     "GOV_CISSOCDEM,");
	_setfield(Polit::GOV_LIBDEM,        "GOV_LIBDEM,");
	_setfield(Polit::GOV_CORPORATE,     "GOV_CORPORATE,");
	_setfield(Polit::GOV_SOCDEM,        "GOV_SOCDEM,");
	_setfield(Polit::GOV_EARTHMILDICT,  "GOV_EARTHMILDICT,");
	_setfield(Polit::GOV_MILDICT1,      "GOV_MILDICT1,");
	_setfield(Polit::GOV_MILDICT2,      "GOV_MILDICT2,");
	_setfield(Polit::GOV_EMPIREMILDICT, "GOV_EMPIREMILDICT,");
	_setfield(Polit::GOV_COMMUNIST,     "GOV_COMMUNIST,");
	_setfield(Polit::GOV_PLUTOCRATIC,   "GOV_PLUTOCRATIC,");
	_setfield(Polit::GOV_DISORDER,      "GOV_DISORDER,");
	_setfield(Polit::GOV_MAX,           "GOV_MAX,");
	_setfield(Polit::GOV_RAND_MIN,      "GOV_RAND_MIN");
	_setfield(Polit::GOV_RAND_MAX,      "GOV_RAND_MAX");
	lua_setfield(L, -2, "GovType");

	lua_setfield(L, LUA_GLOBALSINDEX, "Polit");


	lua_newtable(L); // EquipType

	lua_newtable(L); // Slot
	_setfield(Equip::SLOT_CARGO,            "SLOT_CARGO");
	_setfield(Equip::SLOT_ENGINE,           "SLOT_ENGINE");
	_setfield(Equip::SLOT_LASER,            "SLOT_LASER");
	_setfield(Equip::SLOT_MISSILE,          "SLOT_MISSILE");
	_setfield(Equip::SLOT_ECM,              "SLOT_ECM");
	_setfield(Equip::SLOT_SCANNER,          "SLOT_SCANNER");
	_setfield(Equip::SLOT_RADARMAPPER,      "SLOT_RADARMAPPER");
	_setfield(Equip::SLOT_HYPERCLOUD,       "SLOT_HYPERCLOUD");
	_setfield(Equip::SLOT_HULLAUTOREPAIR,   "SLOT_HULLAUTOREPAIR");
	_setfield(Equip::SLOT_ENERGYBOOSTER,    "SLOT_ENERGYBOOSTER");
	_setfield(Equip::SLOT_ATMOSHIELD,       "SLOT_ATMOSHIELD");
	_setfield(Equip::SLOT_FUELSCOOP,        "SLOT_FUELSCOOP");
	_setfield(Equip::SLOT_LASERCOOLER,      "SLOT_LASERCOOLER");
	_setfield(Equip::SLOT_CARGOLIFESUPPORT, "SLOT_CARGOLIFESUPPORT");
	_setfield(Equip::SLOT_AUTOPILOT,        "SLOT_AUTOPILOT");
	lua_setfield(L, -2, "Slot");

	lua_newtable(L); // Type
	_setfield(Equip::NONE,                  "NONE");
	_setfield(Equip::HYDROGEN,              "HYDROGEN");
	_setfield(Equip::LIQUID_OXYGEN,         "LIQUID_OXYGEN");
	_setfield(Equip::METAL_ORE,             "METAL_ORE");
	_setfield(Equip::CARBON_ORE,            "CARBON_ORE");
	_setfield(Equip::METAL_ALLOYS,          "METAL_ALLOYS");
	_setfield(Equip::PLASTICS,              "PLASTICS");
	_setfield(Equip::FRUIT_AND_VEG,         "FRUIT_AND_VEG");
	_setfield(Equip::ANIMAL_MEAT,           "ANIMAL_MEAT");
	_setfield(Equip::LIVE_ANIMALS,          "LIVE_ANIMALS");
	_setfield(Equip::LIQUOR,                "LIQUOR");
	_setfield(Equip::GRAIN,                 "GRAIN");
	_setfield(Equip::TEXTILES,              "TEXTILES");
	_setfield(Equip::FERTILIZER,            "FERTILIZER");
	_setfield(Equip::WATER,                 "WATER");
	_setfield(Equip::MEDICINES,             "MEDICINES");
	_setfield(Equip::CONSUMER_GOODS,        "CONSUMER_GOODS");
	_setfield(Equip::COMPUTERS,             "COMPUTERS");
	_setfield(Equip::ROBOTS,                "ROBOTS");
	_setfield(Equip::PRECIOUS_METALS,       "PRECIOUS_METALS");
	_setfield(Equip::INDUSTRIAL_MACHINERY,  "INDUSTRIAL_MACHINERY");
	_setfield(Equip::FARM_MACHINERY,        "FARM_MACHINERY");
	_setfield(Equip::MINING_MACHINERY,      "MINING_MACHINERY");
	_setfield(Equip::AIR_PROCESSORS,        "AIR_PROCESSORS");
	_setfield(Equip::SLAVES,                "SLAVES");
	_setfield(Equip::HAND_WEAPONS,          "HAND_WEAPONS");
	_setfield(Equip::BATTLE_WEAPONS,        "BATTLE_WEAPONS");
	_setfield(Equip::NERVE_GAS,             "NERVE_GAS");
	_setfield(Equip::NARCOTICS,             "NARCOTICS");
	_setfield(Equip::MILITARY_FUEL,         "MILITARY_FUEL");
	_setfield(Equip::RUBBISH,               "RUBBISH");
	_setfield(Equip::RADIOACTIVES,          "RADIOACTIVES");

	_setfield(Equip::MISSILE_UNGUIDED,      "MISSILE_UNGUIDED");
	_setfield(Equip::MISSILE_GUIDED,        "MISSILE_GUIDED");
	_setfield(Equip::MISSILE_SMART,         "MISSILE_SMART");
	_setfield(Equip::MISSILE_NAVAL,         "MISSILE_NAVAL");
	_setfield(Equip::ATMOSPHERIC_SHIELDING, "ATMOSPHERIC_SHIELDING");
	_setfield(Equip::ECM_BASIC,             "ECM_BASIC");
	_setfield(Equip::SCANNER,               "SCANNER");
	_setfield(Equip::ECM_ADVANCED,          "ECM_ADVANCED");
	_setfield(Equip::SHIELD_GENERATOR,      "SHIELD_GENERATOR");
	_setfield(Equip::LASER_COOLING_BOOSTER, "LASER_COOLING_BOOSTER");
	_setfield(Equip::CARGO_LIFE_SUPPORT,    "CARGO_LIFE_SUPPORT");
	_setfield(Equip::AUTOPILOT,             "AUTOPILOT");
	_setfield(Equip::RADAR_MAPPER,          "RADAR_MAPPER");
	_setfield(Equip::FUEL_SCOOP,            "FUEL_SCOOP");
	_setfield(Equip::HYPERCLOUD_ANALYZER,   "HYPERCLOUD_ANALYZER");
	_setfield(Equip::HULL_AUTOREPAIR,       "HULL_AUTOREPAIR");
	_setfield(Equip::SHIELD_ENERGY_BOOSTER, "SHIELD_ENERGY_BOOSTER");

	_setfield(Equip::DRIVE_CLASS1,          "DRIVE_CLASS1");
	_setfield(Equip::DRIVE_CLASS2,          "DRIVE_CLASS2");
	_setfield(Equip::DRIVE_CLASS3,          "DRIVE_CLASS3");
	_setfield(Equip::DRIVE_CLASS4,          "DRIVE_CLASS4");
	_setfield(Equip::DRIVE_CLASS5,          "DRIVE_CLASS5");
	_setfield(Equip::DRIVE_CLASS6,          "DRIVE_CLASS6");
	_setfield(Equip::DRIVE_CLASS7,          "DRIVE_CLASS7");
	_setfield(Equip::DRIVE_CLASS8,          "DRIVE_CLASS8");
	_setfield(Equip::DRIVE_CLASS9,          "DRIVE_CLASS9");
	_setfield(Equip::DRIVE_MIL1,            "DRIVE_MIL1");
	_setfield(Equip::DRIVE_MIL2,            "DRIVE_MIL2");
	_setfield(Equip::DRIVE_MIL3,            "DRIVE_MIL3");
	_setfield(Equip::DRIVE_MIL4,            "DRIVE_MIL4");

	_setfield(Equip::PULSECANNON_1MW,       "PULSECANNON_1MW");
	_setfield(Equip::PULSECANNON_DUAL_1MW,  "PULSECANNON_DUAL_1MW");
	_setfield(Equip::PULSECANNON_2MW,       "PULSECANNON_2MW");
	_setfield(Equip::PULSECANNON_RAPID_2MW, "PULSECANNON_RAPID_2MW");
	_setfield(Equip::PULSECANNON_4MW,       "PULSECANNON_4MW");
	_setfield(Equip::PULSECANNON_10MW,      "PULSECANNON_10MW");
	_setfield(Equip::PULSECANNON_20MW,      "PULSECANNON_20MW");
	_setfield(Equip::MININGCANNON_17MW,     "MININGCANNON_17MW");
	_setfield(Equip::SMALL_PLASMA_ACCEL,    "SMALL_PLASMA_ACCEL");
	_setfield(Equip::LARGE_PLASMA_ACCEL,    "LARGE_PLASMA_ACCEL");

	_setfield(Equip::TYPE_MAX,              "TYPE_MAX");
	_setfield(Equip::FIRST_COMMODITY,       "FIRST_COMMODITY");
	_setfield(Equip::LAST_COMMODITY,        "LAST_COMMODITY");
	_setfield(Equip::FIRST_SHIPEQUIP,       "FIRST_SHIPEQUIP");
	_setfield(Equip::LAST_SHIPEQUIP,        "LAST_SHIPEQUIP");
	lua_setfield(L, -2, "Type");

	lua_setfield(L, LUA_GLOBALSINDEX, "EquipType");
}
