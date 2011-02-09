#include "CustomSystem.h"
#include "MyLuaMathTypes.h"
#include "LuaUtilFuncs.h"
#include "Polit.h"

static std::list<CustomSystem> custom_systems;

static CustomSBody define_sbody(lua_State *L)
{
	CustomSBody csbody;

	lua_getfield(L, -1, "name");
	if (lua_isnone(L, -1)) {
		luaL_error(L, "define_sbody: required field 'name' not provided");
		return csbody;
	}
	if (!lua_isstring(L, -1)) {
		luaL_error(L, "define_system: value for field 'name' must be a string");
		return csbody;
	}
	csbody.name = luaL_checkstring(L, -1);
	printf("define_sbody: name: %s\n", csbody.name.c_str());
	lua_pop(L, 1);

	lua_getfield(L, -1, "type");
	if (lua_isnone(L, -1)) {
		luaL_error(L, "define_sbody: required field 'type' not provided");
		return csbody;
	}
	if (!lua_isnumber(L, -1)) {
		luaL_error(L, "define_system: value for field 'name' must be an integer");
		return csbody;
	}
	csbody.type = static_cast<SBody::BodyType>(luaL_checkinteger(L, -1));
	printf("define_sbody: type: %d\n", csbody.type);
	lua_pop(L, 1);

	return csbody;
}

static int define_system(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 2) {
		luaL_error(L, "define_system takes 2 arguments");
		return 0;
	}

	const char *system_name = luaL_checkstring(L, -2);

	if (!lua_istable(L, -1)) {
		luaL_error(L, "define_system 2nd argument must be a table");
		return 0;
	}

	printf("define_system: %s\n", system_name);

	CustomSystem cs;

	cs.name = system_name;

	lua_getfield(L, -1, "type");
	if (lua_isnil(L, -1)) {
		luaL_error(L, "define_system: required field 'type' not provided");
		return 0;
	}
	if (lua_istable(L, -1)) {
		int i;
		for (i = 0; i < 4; i++) {
			lua_pushinteger(L, i+1);
			lua_gettable(L, -2);
			if (lua_isnil(L, -1)) {
				lua_pop(L, 1);
				break;
			}
			if (lua_isnumber(L, -1)) {
				printf("define_system: type: %d\n", luaL_checkinteger(L, -1));
				cs.primaryType[i] = static_cast<SBody::BodyType>(luaL_checkinteger(L, 4));
			} else {
				luaL_error(L, "define_system: position %d in field 'type' is not an integer", i);
				return 0;
			}
			lua_pop(L, 1);
		}
		for (; i < 4; i++) {
			cs.primaryType[i] = SBody::TYPE_GRAVPOINT;
		}
	} else {
		luaL_error(L, "define_system: value for field 'type' must be a table");
		return 0;
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "sector");
	if (lua_isnone(L, -1)) {
		luaL_error(L, "define_system: required field 'sector' not provided");
		return 0;
	}
	if (lua_istable(L, -1)) {
		int sector[2];
		for (int i = 0; i < 2; i++) {
			lua_pushinteger(L, i+1);
			lua_gettable(L, -2);
			if (lua_isnumber(L, -1)) {
				sector[i] = luaL_checkinteger(L, -1);
			} else {
				luaL_error(L, "define_system: position %d in field 'sector' is not an integer", i);
				return 0;
			}
			lua_pop(L, 1);
		}
		cs.sectorX = sector[0];
		cs.sectorY = sector[1];
		printf("define_system: sector [%d,%d]\n", cs.sectorX, cs.sectorY);
	} else {
		luaL_error(L, "define_system: value for field 'sector' must be a table");
		return 0;
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "pos");
	if (lua_isnone(L, -1)) {
		luaL_error(L, "define_system: required field 'pos' not provided");
		return 0;
	}
	if (!MyLuaVec::isVec(L, -1)) {
		luaL_error(L, "define_system: value for field 'pos' must be a vector (use v() to make one");
		return 0;
	}
	cs.pos = *MyLuaVec::checkVec(L, -1);
	printf("define_system: pos: (%f,%f,%f)\n", cs.pos.x, cs.pos.y, cs.pos.z);
	lua_pop(L, 1);

	lua_getfield(L, -1, "seed");
	if (lua_isnumber(L, -1))
		cs.seed = luaL_checkinteger(L, -1);
	else
		cs.seed = 0;
	lua_pop(L, 1);
	printf("define_system: seed: %d\n", cs.seed);

	lua_getfield(L, -1, "govtype");
	if (lua_isnumber(L, -1))
		cs.govType = static_cast<Polit::GovType>(luaL_checkinteger(L, -1));
	else
		cs.govType = Polit::GOV_NONE;
	lua_pop(L, 1);
	printf("define_system: govtype: %d\n", cs.govType);

	lua_getfield(L, -1, "short_desc");
	if (lua_isstring(L, -1)) {
		cs.shortDesc = luaL_checkstring(L, -1);
		printf("define_system: short_desc: %s\n", cs.shortDesc.c_str());
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "long_desc");
	if (lua_isstring(L, -1)) {
		cs.longDesc = luaL_checkstring(L, -1);
		printf("define_system: long_desc: %s\n", cs.longDesc.c_str());
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "bodies");
	if (lua_istable(L, -1)) {
		cs.sBody = define_sbody(L);
		if (cs.sBody.type != cs.primaryType[0]) {
			luaL_error(L, "define_system: primary body has different type to first star\n");
			return 0;
		}
	} else if(!lua_isnil(L, -1)) {
		luaL_error(L, "define_system: value for field 'bodies' must be a table");
		return 0;
	}
	lua_pop(L, 1);

	custom_systems.push_back(cs);

	return 0;
}

void CustomSystem::Init()
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	MyLuaVec::Vec_register(L);
	lua_pop(L, 1);

	MyLuaFixed::Fixed_register(L);
	lua_pop(L, 1);

	lua_register(L, "v", MyLuaVec::Vec_new);
	lua_register(L, "fixed", MyLuaFixed::Fixed_new);
	lua_register(L, "define_system", define_system);
	lua_register(L, "load_lua", LuaUtilFuncs::load_lua);
	lua_register(L, "deg2rad", LuaUtilFuncs::deg2rad);
	lua_register(L, "fixed_deg2rad", LuaUtilFuncs::fixed_deg2rad);

	lua_pushstring(L, PIONEER_DATA_DIR);
	lua_setglobal(L, "CurrentDirectory");

	lua_pushcfunction(L, mylua_panic);
	if (luaL_loadfile(L, (std::string(PIONEER_DATA_DIR) + "/pisystems.lua").c_str())) {
		mylua_panic(L);
	} else {
		lua_pcall(L, 0, LUA_MULTRET, -2);
	}

	lua_close(L);
}

const std::list<const CustomSystem*> CustomSystem::GetCustomSystemsForSector(int sectorX, int sectorY)
{
	std::list<const CustomSystem*> sector_systems;

	for (std::list<CustomSystem>::iterator i = custom_systems.begin(); i != custom_systems.end(); i++) {
		CustomSystem *cs = &(*i);
		if (cs->sectorX == sectorX && cs->sectorY == sectorY)
			sector_systems.push_back(cs);
	}

	return sector_systems;
}

const CustomSystem* CustomSystem::GetCustomSystem(const char *name)
{
	for (std::list<CustomSystem>::iterator i = custom_systems.begin(); i != custom_systems.end(); i++) {
		CustomSystem *cs = &(*i);
		if (!cs->name.compare(name)) return cs;
	}
	return NULL;
}

const SBodyPath CustomSystem::GetSBodyPathForCustomSystem(const CustomSystem* cs)
{
	const std::list<const CustomSystem*> cslist = GetCustomSystemsForSector(cs->sectorX, cs->sectorY);
	int idx = 0;
	for (std::list<const CustomSystem*>::const_iterator i = cslist.begin(); i != cslist.end(); i++) {
		if (!(*i)->name.compare(cs->name)) break;
			idx++;
	}
	assert(idx < static_cast<int>(cslist.size()));
	return SBodyPath(cs->sectorX, cs->sectorY, idx);
}

const SBodyPath CustomSystem::GetSBodyPathForCustomSystem(const char* name)
{
	return GetSBodyPathForCustomSystem(GetCustomSystem(name));
}
