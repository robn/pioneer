#include "Sector.h"
#include "StarSystem.h"
#include "CustomSystem.h"
#include "Galaxy.h"
#include "MyLuaMathTypes.h"
#include "LuaUtilFuncs.h"

#define SYS_NAME_FRAGS	32
static const char *sys_names[SYS_NAME_FRAGS] =
{ "en", "la", "can", "be", "and", "phi", "eth", "ol", "ve", "ho", "a",
  "lia", "an", "ar", "ur", "mi", "in", "ti", "qu", "so", "ed", "ess",
  "ex", "io", "ce", "ze", "fa", "ay", "wa", "da", "ack", "gre" };

const float Sector::SIZE = 8;

static int define_system(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 2) {
		luaL_error(L, "define_system takes 2 arguments");
		return 0;
	}

	const char *system_name = luaL_checkstring(L, 1);

	if (!lua_istable(L, 2)) {
		luaL_error(L, "define_system 2nd argument must be a table");
		return 0;
	}

	printf("define_system: %s\n", system_name);

	return 0;
}

void Sector::Init()
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	MyLuaVec::Vec_register(L);
	lua_pop(L, 1);

	lua_register(L, "v", MyLuaVec::Vec_new);
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

void Sector::GetCustomSystems()
{
	const std::list<CustomSystem*> systems = CustomSystem::GetCustomSystemsForSector(sx, sy);
	return if custom_systems.size() == 0;

	for (const std::list<CustomSystem*>::iterator i = systems.begin(); i != custom_systems.end(); i++) {
		CustomSystem *cs = *i;
		System s;
		s.p = SIZE*cs->pos;
		s.name = cs->name;
		for (s.numStars=0; s.numStars<4; s.numStars++) {
			if (cs->primaryType[s.numStars] == 0) break;
			s.starType[s.numStars] = cs->primaryType[s.numStars];
		}
		s.customSys = cs;
		s.seed = cs->seed;
		m_systems.push_back(s);
	}
}

//////////////////////// Sector
Sector::Sector(int x, int y)
{
	unsigned long _init[3] = { x, y, UNIVERSE_SEED };
	sx = x; sy = y;
	MTRand rng(_init, 3);

	GetCustomSystems();

	if (m_systems.size() != 0) {
		// custom sector

	} else {
		int numSystems = (rng.Int32(4,20) * Galaxy::GetSectorDensity(x, y)) >> 8;

		for (int i=0; i<numSystems; i++) {
			System s;
			switch (rng.Int32(15)) {
				case 0:
					s.numStars = 4; break;
				case 1: case 2:
					s.numStars = 3; break;
				case 3: case 4: case 5: case 6:
					s.numStars = 2; break;
				default:
					s.numStars = 1; break;
			}

			s.p.x = rng.Double(SIZE);
			s.p.y = rng.Double(SIZE);
			s.p.z = rng.Double(2*SIZE)-SIZE;
			s.seed = 0;
			s.customSys = 0;
			
			float spec = rng.Int32(1000000);
			// frequencies from wikipedia
			if (spec < 100) { // should be 1 but that is boring
				s.starType[0] = SBody::TYPE_STAR_O;
			} else if (spec < 1300) {
				s.starType[0] = SBody::TYPE_STAR_B;
			} else if (spec < 7300) {
				s.starType[0] = SBody::TYPE_STAR_A;
			} else if (spec < 37300) {
				s.starType[0] = SBody::TYPE_STAR_F;
			} else if (spec < 113300) {
				s.starType[0] = SBody::TYPE_STAR_G;
			} else if (spec < 234300) {
				s.starType[0] = SBody::TYPE_STAR_K;
			} else if (spec < 250000) {
				s.starType[0] = SBody::TYPE_WHITE_DWARF;
			} else if (spec < 900000) {
				s.starType[0] = SBody::TYPE_STAR_M;
			} else {
				s.starType[0] = SBody::TYPE_BROWN_DWARF;
			}

			if (s.numStars > 1) {
				s.starType[1] = (SBody::BodyType)rng.Int32(SBody::TYPE_STAR_MIN, s.starType[0]);
				if (s.numStars > 2) {
					s.starType[2] = (SBody::BodyType)rng.Int32(SBody::TYPE_STAR_MIN, s.starType[0]);
					s.starType[3] = (SBody::BodyType)rng.Int32(SBody::TYPE_STAR_MIN, s.starType[2]);
				}
			}

			if ((s.starType[0] <= SBody::TYPE_STAR_A) && (rng.Int32(10)==0)) {
				// make primary a giant. never more than one giant in a system
				// while 
				s.starType[0] = SBody::TYPE_STAR_M_GIANT;
			}

			s.name = GenName(s, rng);

			m_systems.push_back(s);
		}
	}
}

float Sector::DistanceBetween(const Sector *a, int sysIdxA, const Sector *b, int sysIdxB)
{
	vector3f dv = a->m_systems[sysIdxA].p - b->m_systems[sysIdxB].p;
	dv += Sector::SIZE*vector3f(a->sx - b->sx, a->sy - b->sy, 0);
	return dv.Length();
}

std::string Sector::GenName(System &sys, MTRand &rng)
{
	std::string name;
	const int dist = MAX(abs(sx),abs(sy));

	int chance = 100;
	switch (sys.starType[0]) {
		case SBody::TYPE_STAR_O:
		case SBody::TYPE_STAR_B: break;
		case SBody::TYPE_STAR_A: chance += dist; break;
		case SBody::TYPE_STAR_F: chance += 2*dist; break;
		case SBody::TYPE_STAR_G: chance += 4*dist; break;
		case SBody::TYPE_STAR_K: chance += 8*dist; break;
		default: chance += 16*dist; break;
	}
	if (rng.Int32(chance) < 100) {
		/* well done. you get a real name */
		int len = rng.Int32(2,3);
		for (int i=0; i<len; i++) {
			name += sys_names[rng.Int32(0,SYS_NAME_FRAGS-1)];
		}
		name[0] = toupper(name[0]);
		return name;
	} else {
		char buf[128];
		snprintf(buf, sizeof(buf), "SC %d%+d%+d", rng.Int32(1000,9999),sx,sy);
		return buf;
	}
}

