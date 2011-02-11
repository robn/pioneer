#include "LuaConstants.h"
#include "StarSystem.h"

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

	lua_setfield(L, LUA_GLOBALSINDEX, "SBody");
}
