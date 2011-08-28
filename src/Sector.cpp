#include "Sector.h"
#include "StarSystem.h"
#include "Galaxy.h"
#include "Pi.h"
#include "utils.h"

#define SYS_NAME_FRAGS	32
static const char *sys_names[SYS_NAME_FRAGS] =
{ "en", "la", "can", "be", "and", "phi", "eth", "ol", "ve", "ho", "a",
  "lia", "an", "ar", "ur", "mi", "in", "ti", "qu", "so", "ed", "ess",
  "ex", "io", "ce", "ze", "fa", "ay", "wa", "da", "ack", "gre" };

const float Sector::SIZE = 8;

std::map<SystemPath,Sector*> Sector::s_startupCache;
std::map<SystemPath,Sector*> Sector::s_gameCache;

Sector *Sector::Get(int x, int y, int z)
{
	SystemPath path(x,y,z,0,0);

	Sector *s;
	std::map<SystemPath,Sector*>::iterator i;

	i = s_gameCache.find(path);
	if (i != s_gameCache.end()) {
		s = (*i).second;
		s->IncRefCount();
		return s;
	}

	i = s_startupCache.find(path);
	if (i != s_startupCache.end()) {
		s = (*i).second;
		s->IncRefCount();
		return s;
	}

	s = new Sector(x,y,z);
	s->IncRefCount();
	return s;
}

void Sector::Release()
{
	DecRefCount();
	if (!GetRefCount())
        delete this;
}

const std::vector<Sector::System> &Sector::GetSystems() const
{
	GenerateSystems();
	return m_systems;
}

const Sector::System &Sector::GetSystem(Uint32 n) const
{
	GenerateSystems();
	assert(n < m_systems.size());
	return m_systems[n];
}

Uint32 Sector::GetNumSystems() const
{
	GenerateSystems();
	return m_systems.size();
}

void Sector::AddSystem(const CustomSystem *customSystem)
{
	// XXX don't add duplicates
	if (Pi::IsGameStarted())
		m_gameCustomSystems.push_back(customSystem);
	else
		m_startupCustomSystems.push_back(customSystem);

	SystemPath path(sx,sy,sz,0,0);

	std::map<SystemPath,Sector*>::iterator i;

	if (Pi::IsGameStarted()) {
		i = s_gameCache.find(path);
		if (i == s_gameCache.end()) {
			IncRefCount();
			s_gameCache.insert( std::make_pair(path, this) );
		}
	}

	else {
		i = s_startupCache.find(path);
		if (i == s_startupCache.end()) {
			IncRefCount();
			s_startupCache.insert( std::make_pair(path, this) );
		}
	}

	m_dirty = true;
}



Sector::Sector(int x, int y, int z) :
	sx(x), sy(y), sz(z),
	m_dirty(true),
	m_customOnly(false)
{
}


/*
void Sector::GetCustomSystems()
{
	const std::list<const CustomSystem*> systems = CustomSystem::GetCustomSystemsForSector(sx, sy, sz);
	if (systems.size() == 0) return;

	for (std::list<const CustomSystem*>::const_iterator i = systems.begin(); i != systems.end(); i++) {
		const CustomSystem *cs = *i;
		System s;
		s.p = SIZE*cs->pos;
		s.name = cs->name;
		for (s.numStars=0; s.numStars<cs->numStars; s.numStars++) {
			if (cs->primaryType[s.numStars] == 0) break;
			s.starType[s.numStars] = cs->primaryType[s.numStars];
		}
		s.customSys = cs;
		s.seed = cs->seed;
		m_systems.push_back(s);
	}
}
*/

#define CUSTOM_ONLY_RADIUS	4

void Sector::GenerateSystems() const
{
	if (!m_dirty) return;

	m_systems.clear();

	if (!m_customOnly) {
		unsigned long _init[4] = { sx, sy, sz, UNIVERSE_SEED };
		MTRand rng(_init, 4);
		MTRand rand(UNIVERSE_SEED);

#if 0
		GetCustomSystems();
	
		/* Always place random systems outside the core custom-only region */
		if ((x < -CUSTOM_ONLY_RADIUS) || (x > CUSTOM_ONLY_RADIUS-1) ||
		    (y < -CUSTOM_ONLY_RADIUS) || (y > CUSTOM_ONLY_RADIUS-1) ||
		    (z < -CUSTOM_ONLY_RADIUS) || (z > CUSTOM_ONLY_RADIUS-1)) {
#endif
			int numSystems = (rng.Int32(4,20) * Galaxy::GetSectorDensity(sx, sy, sz)) >> 8;
	
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
				s.p.z = rng.Double(SIZE);
				s.seed = 0;
				s.customSystem = 0;
				
				float spec = rng.Int32(1000000);
				// frequencies from wikipedia
				/*if (spec < 100) { // should be 1 but that is boring
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
				}*/
				//if ((sx > 50) || (sx < -50) ||
				//	(sy > 50) || (sy < -50))
	
				// Frequencies are low enough that we probably don't need this anymore.
				if (isqrt(1+sx*sx+sy*sy) > 10) 
				{
					if (spec < 1) { 
						s.starType[0] = SBody::TYPE_STAR_IM_BH;  // These frequencies are made up
					} else if (spec < 3) {
						s.starType[0] = SBody::TYPE_STAR_S_BH; 
					} else if (spec < 6) {
						s.starType[0] = SBody::TYPE_STAR_K_HYPER_GIANT; 
					} else if (spec < 10) {
						s.starType[0] = SBody::TYPE_STAR_G_HYPER_GIANT;
					} else if (spec < 15) {
						s.starType[0] = SBody::TYPE_STAR_O_HYPER_GIANT; 
					} else if (spec < 20) {
						s.starType[0] = SBody::TYPE_STAR_A_HYPER_GIANT;
					} else if (spec < 25) {
						s.starType[0] = SBody::TYPE_STAR_F_HYPER_GIANT;
					} else if (spec < 30) {
						s.starType[0] = SBody::TYPE_STAR_B_HYPER_GIANT; 
					} else if (spec < 40) {
						s.starType[0] = SBody::TYPE_STAR_M_HYPER_GIANT; 
					} else if (spec < 50) {
						s.starType[0] = SBody::TYPE_STAR_K_SUPER_GIANT; 
					} else if (spec < 60) {
						s.starType[0] = SBody::TYPE_STAR_G_SUPER_GIANT;
					} else if (spec < 70) {
						s.starType[0] = SBody::TYPE_STAR_O_SUPER_GIANT; 
					} else if (spec < 80) {
						s.starType[0] = SBody::TYPE_STAR_A_SUPER_GIANT;
					} else if (spec < 90) {
						s.starType[0] = SBody::TYPE_STAR_F_SUPER_GIANT;
					} else if (spec < 100) {
						s.starType[0] = SBody::TYPE_STAR_B_SUPER_GIANT; 
					} else if (spec < 150) {
						s.starType[0] = SBody::TYPE_STAR_M_SUPER_GIANT; 
					} else if (spec < 200) {
						s.starType[0] = SBody::TYPE_STAR_K_GIANT; 
					} else if (spec < 250) {
						s.starType[0] = SBody::TYPE_STAR_G_GIANT;
					} else if (spec < 300) {
						s.starType[0] = SBody::TYPE_STAR_O_GIANT; 
					} else if (spec < 350) {
						s.starType[0] = SBody::TYPE_STAR_A_GIANT;
					} else if (spec < 400) {
						s.starType[0] = SBody::TYPE_STAR_F_GIANT;
					} else if (spec < 500) {
						s.starType[0] = SBody::TYPE_STAR_B_GIANT; 
					} else if (spec < 700) {
						s.starType[0] = SBody::TYPE_STAR_M_GIANT; 
					} else if (spec < 800) {
						s.starType[0] = SBody::TYPE_STAR_O;  // should be 1 but that is boring
					} else if (spec < 2000) { // spec < 1300 / 20500
						s.starType[0] = SBody::TYPE_STAR_B;  
					} else if (spec < 8000) { // spec < 7300
						s.starType[0] = SBody::TYPE_STAR_A;  
					} else if (spec < 37300) { // spec < 37300
						s.starType[0] = SBody::TYPE_STAR_F;  
					} else if (spec < 113300) { // spec < 113300
						s.starType[0] = SBody::TYPE_STAR_G;  
					} else if (spec < 234300) { // spec < 234300
						s.starType[0] = SBody::TYPE_STAR_K;  
					} else if (spec < 250000) { // spec < 250000
						s.starType[0] = SBody::TYPE_WHITE_DWARF;
					} else if (spec < 900000) {  //spec < 900000
						s.starType[0] = SBody::TYPE_STAR_M;
					} else {
						s.starType[0] = SBody::TYPE_BROWN_DWARF;
					}
				} else {
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
				}
				//printf("%d: %d%\n", sx, sy);
	
				if (s.numStars > 1) {
					s.starType[1] = SBody::BodyType(rng.Int32(SBody::TYPE_STAR_MIN, s.starType[0]));
					if (s.numStars > 2) {
						s.starType[2] = SBody::BodyType(rng.Int32(SBody::TYPE_STAR_MIN, s.starType[0]));
						s.starType[3] = SBody::BodyType(rng.Int32(SBody::TYPE_STAR_MIN, s.starType[2]));
					}
				}
	
				if ((s.starType[0] <= SBody::TYPE_STAR_A) && (rng.Int32(10)==0)) {
					// make primary a giant. never more than one giant in a system
					// while 
					if (isqrt(1+sx*sx+sy*sy) > 10)
					{			
						if (rand.Int32(0,1000) >= 999) {
							s.starType[0] = SBody::TYPE_STAR_B_HYPER_GIANT;
						} else if (rand.Int32(0,1000) >= 998) {
							s.starType[0] = SBody::TYPE_STAR_O_HYPER_GIANT;
						} else if (rand.Int32(0,1000) >= 997) {
							s.starType[0] = SBody::TYPE_STAR_K_HYPER_GIANT;
						} else if (rand.Int32(0,1000) >= 995) {
							s.starType[0] = SBody::TYPE_STAR_B_SUPER_GIANT;
						} else if (rand.Int32(0,1000) >= 993) {
							s.starType[0] = SBody::TYPE_STAR_O_SUPER_GIANT;
						} else if (rand.Int32(0,1000) >= 990) {
							s.starType[0] = SBody::TYPE_STAR_K_SUPER_GIANT;
						} else if (rand.Int32(0,1000) >= 985) {
							s.starType[0] = SBody::TYPE_STAR_B_GIANT;
						} else if (rand.Int32(0,1000) >= 980) {
							s.starType[0] = SBody::TYPE_STAR_O_GIANT;
						} else if (rand.Int32(0,1000) >= 975) {
							s.starType[0] = SBody::TYPE_STAR_K_GIANT;
						} else if (rand.Int32(0,1000) >= 950) {
							s.starType[0] = SBody::TYPE_STAR_M_HYPER_GIANT;
						} else if (rand.Int32(0,1000) >= 875) {
							s.starType[0] = SBody::TYPE_STAR_M_SUPER_GIANT;
						} else {
							s.starType[0] = SBody::TYPE_STAR_M_GIANT;
						} 
					} else if (isqrt(1+sx*sx+sy*sy) > 5) s.starType[0] = SBody::TYPE_STAR_M_GIANT;
					else s.starType[0] = SBody::TYPE_STAR_M;
	
					//printf("%d: %d%\n", sx, sy);
				}
	
				s.name = GenName(s, rng);
				//printf("%s: \n", s.name.c_str());
	
				m_systems.push_back(s);
			}
#if 0
		}
#endif
	}

	const std::vector<const CustomSystem *> *customLists[2] = { &m_startupCustomSystems, &m_gameCustomSystems };
	for (int n = 0; n < 2; n++) {
		for (std::vector<const CustomSystem *>::const_iterator i = customLists[n]->begin(); i != customLists[n]->end(); i++) {
			const CustomSystem *cs = (*i);
			System s;

			s.name         = cs->name;
			s.p            = cs->pos;
			s.numStars     = 1;
			s.starType[0]  = cs->primaryType;
			s.seed         = cs->seed;
	        s.customSystem = cs;

			m_systems.push_back(s);
		}
	}

	m_dirty = false;
}

float Sector::DistanceBetween(const Sector *a, int sysIdxA, const Sector *b, int sysIdxB)
{
	vector3f dv = a->GetSystem(sysIdxA).p - b->GetSystem(sysIdxB).p;
	dv += Sector::SIZE*vector3f(a->sx - b->sx, a->sy - b->sy, a->sz - b->sz);
	return dv.Length();
}

std::string Sector::GenName(System &sys, MTRand &rng) const
{
	std::string name;
	const int dist = std::max(std::max(abs(sx),abs(sy)),abs(sz));

	int chance = 100;
	switch (sys.starType[0]) {
		case SBody::TYPE_STAR_O:
		case SBody::TYPE_STAR_B: break;
		case SBody::TYPE_STAR_A: chance += dist; break;
		case SBody::TYPE_STAR_F: chance += 2*dist; break;
		case SBody::TYPE_STAR_G: chance += 4*dist; break;
		case SBody::TYPE_STAR_K: chance += 8*dist; break;
		case SBody::TYPE_STAR_O_GIANT:
		case SBody::TYPE_STAR_B_GIANT: chance = 50; break;
		case SBody::TYPE_STAR_A_GIANT: chance = 0.2*dist; break;
		case SBody::TYPE_STAR_F_GIANT: chance = 0.4*dist; break;
		case SBody::TYPE_STAR_G_GIANT: chance = 0.5*dist; break;
		case SBody::TYPE_STAR_K_GIANT:
		case SBody::TYPE_STAR_M_GIANT: chance = dist; break;
		case SBody::TYPE_STAR_O_SUPER_GIANT:
		case SBody::TYPE_STAR_B_SUPER_GIANT: chance = 10; break;
		case SBody::TYPE_STAR_A_SUPER_GIANT:
		case SBody::TYPE_STAR_F_SUPER_GIANT:
		case SBody::TYPE_STAR_G_SUPER_GIANT:
		case SBody::TYPE_STAR_K_SUPER_GIANT: chance = 15; break;
		case SBody::TYPE_STAR_M_SUPER_GIANT: chance = 20; break;
		case SBody::TYPE_STAR_O_HYPER_GIANT:
		case SBody::TYPE_STAR_B_HYPER_GIANT:
		case SBody::TYPE_STAR_A_HYPER_GIANT:
		case SBody::TYPE_STAR_F_HYPER_GIANT:
		case SBody::TYPE_STAR_G_HYPER_GIANT:
		case SBody::TYPE_STAR_K_HYPER_GIANT:
		case SBody::TYPE_STAR_M_HYPER_GIANT: chance = 1; break;  //Should give a nice name almost all the time
		default: chance += 16*dist; break;
	}
	if (rng.Int32(chance) < 500) {
		/* well done. you get a real name */
		int len = rng.Int32(2,3);
		for (int i=0; i<len; i++) {
			name += sys_names[rng.Int32(0,SYS_NAME_FRAGS-1)];
		}
		name[0] = toupper(name[0]);
		return name;
	} else if (rng.Int32(chance) < 800) {
		char buf[128];
		snprintf(buf, sizeof(buf), "MJBN %d%+d%+d", rng.Int32(10,999),sx,sy); // MJBN -> Morton Jordan Bennett Norris
		return buf;
	} else if (rng.Int32(chance) < 1200) {
		char buf[128];
		snprintf(buf, sizeof(buf), "SC %d%+d%+d", rng.Int32(1000,9999),sx,sy); 
		return buf;
	} else {
		char buf[128];
		snprintf(buf, sizeof(buf), "DSC %d%+d%+d", rng.Int32(1000,9999),sx,sy); 
		return buf;
	}
}

bool Sector::WithinBox(const int Xmin, const int Xmax, const int Ymin, const int Ymax, const int Zmin, const int Zmax) const {
	if(sx >= Xmin && sx <= Xmax) {
		if(sy >= Ymin && sy <= Ymax) {
			if(sz >= Zmin && sz <= Zmax) {
				return true;
			}
		}
	}
	return false;
}
