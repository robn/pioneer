#include "CustomSystem.h"
#include "LuaConstants.h"
#include "MyLuaMathTypes.h"
#include "LuaUtilFuncs.h"
#include "Polit.h"

static std::list<CustomSystem> custom_systems;

static CustomSBody define_sbody(lua_State *L, OOLUA::Lua_table &body_def)
{
	CustomSBody csbody;

	if (! body_def.safe_at("name", csbody.name)) {
		luaL_error(L, "define_sbody: value for field 'name' must be a string");
		return csbody;
	}

	printf("define_sbody: %s\n", csbody.name.c_str());

	int type;
	if (! body_def.safe_at("type", type)) {
		luaL_error(L, "define_sbody: value for field 'name' missing or invalid");
		return csbody;
	}
	csbody.type = static_cast<SBody::BodyType>(type);

	fixed *f;
	csbody.radius         = body_def.safe_at("radius",          f) ? *f : 0;
	csbody.mass           = body_def.safe_at("mass",            f) ? *f : 0;
	csbody.semiMajorAxis  = body_def.safe_at("semi_major_axis", f) ? *f : 0;
	csbody.eccentricity   = body_def.safe_at("eccentricity",    f) ? *f : 0;
	csbody.rotationPeriod = body_def.safe_at("rotation_period", f) ? *f : 0;
	csbody.axialTilt      = body_def.safe_at("axial_tilt",      f) ? *f : 0;

	if (! body_def.safe_at("temp",      csbody.averageTemp)) csbody.averageTemp = 0;
	if (! body_def.safe_at("latitude",  csbody.latitude))    csbody.latitude = 0;
	if (! body_def.safe_at("longitude", csbody.longitude))   csbody.longitude = 0;

	body_def.at("height_map", csbody.heightMapFilename);

	OOLUA::Lua_table children, child_def;
	if (body_def.safe_at("children", children)) {
		for (int i = 0;; i++) {
			if (! children.safe_at(i+1, child_def))
				break;

			csbody.children.push_back(define_sbody(L, child_def));
		}
	}

	return csbody;
}

static int define_system(lua_State *L)
{
	CustomSystem cs;

	if (lua_gettop(L) != 2) {
		luaL_error(L, "define_system: expected 2 arguments");
		return 0;
	}

	OOLUA::Lua_table t;
	if (! OOLUA::pull2cpp(L, t)) {
		luaL_error(L, "define_system: 2nd argument must be a table");
		return 0;
	}

	if (! OOLUA::pull2cpp(L, cs.name)) {
		luaL_error(L, "define_system: 1st argument must be a string");
		return 0;
	}

	printf("define_system: %s\n", cs.name.c_str());

	OOLUA::Lua_table star_types;
	if (! t.safe_at("type", star_types)) {
		luaL_error(L, "define_system: required field 'type' must be a table");
		return 0;
	}
	for (int i = 0; i < 4; i++) {
		int type;
		if (star_types.safe_at(i+1, type))
			cs.primaryType[i] = static_cast<SBody::BodyType>(type);
		else
			cs.primaryType[i] = SBody::TYPE_GRAVPOINT;
	}

	OOLUA::Lua_table sector;
	if (! t.safe_at("sector", sector)) {
		luaL_error(L, "define_system: required field 'sector' must be a table");
		return 0;
	}
	if (! sector.safe_at(1, cs.sectorX)) {
		luaL_error(L, "define_system: position 1 in field 'sector' is not an integer");
		return 0;
	}
	if (! sector.safe_at(2, cs.sectorY)) {
		luaL_error(L, "define_system: position 2 in field 'sector' is not an integer");
		return 0;
	}

	vector3f *v;
	if (! t.safe_at("pos", v)) {
		luaL_error(L, "define_system: value for field 'pos' must be a vector");
		return 0;
	}
	cs.pos = *v;

	if (! t.safe_at("seed", cs.seed)) cs.seed = 0;

	int govtype;
	if (! t.safe_at("govtype", govtype)) govtype = static_cast<int>(Polit::GOV_NONE);
	cs.govType = static_cast<Polit::GovType>(govtype);

	t.at("short_desc", cs.shortDesc);
	t.at("long_desc", cs.longDesc);

	OOLUA::Lua_table body_def;
	if (t.safe_at("bodies", body_def)) {
		cs.sBody = define_sbody(L, body_def);
		if (cs.sBody.type != cs.primaryType[0]) {
			luaL_error(L, "define_system: primary body must be of same type as first star");
			return 0;
		}
	}

	custom_systems.push_back(cs);

	return 0;
}

void CustomSystem::Init()
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	OOLUA::setup_user_lua_state(L);
	OOLUA::register_class<vector3f>(L);
	OOLUA::register_class<fixed>(L);

	LuaConstants::RegisterConstants(L);

	lua_register(L, "define_system", define_system);
	lua_register(L, "load_lua", LuaUtilFuncs::load_lua);

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
