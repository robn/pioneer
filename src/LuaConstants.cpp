#include "LuaConstants.h"
#include "StarSystem.h"
#include "Polit.h"

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
}
