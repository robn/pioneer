#include "libs.h"
#include "CityOnPlanet.h"
#include "Frame.h"
#include "SpaceStation.h"
#include "Planet.h"
#include "Pi.h"
#include "LuaConstants.h"
#include "collider/Geom.h"

#define START_SEG_SIZE CITY_ON_PLANET_RADIUS
#define MIN_SEG_SIZE 50.0

std::vector<Building> CityOnPlanet::s_buildings;
bool CityOnPlanet::s_buildingsLoaded = false;

#define CITYFLAVOURS 5
struct cityflavourdef_t {
	//int buildingListIdx;
	vector3d center;
	double size;
} cityflavour[CITYFLAVOURS];

#if 0
#define MAX_BUILDING_LISTS 1
struct citybuildinglist_t {
	const char *modelTagName;
	double minRadius, maxRadius;
	int numBuildings;
	citybuilding_t *buildings;
};

citybuildinglist_t s_buildingLists[MAX_BUILDING_LISTS] = {
	{ "city_building", 800, 2000, 0, NULL },
	//{ "city_power", 100, 250, 0, NULL },
	//{ "city_starport_building", 300, 400, 0, NULL },
};
#endif

static Plane planes[6];
LmrObjParams cityobj_params;

void CityOnPlanet::PutCityBit(MTRand &rand, const matrix4x4d &rot, Division div)
{
	double rad = (div.p1-div.p2).Length()*0.5;
	LmrModel *model;
	double modelRadXZ;
	const LmrCollMesh *cmesh;
	vector3d cent = (div.p1+div.p2+div.p3+div.p4)*0.25;

	cityflavourdef_t *flavour;
	//citybuildinglist_t *buildings;

	bool found = false;

	// pick a building flavour (city, windfarm, etc)
	for (int flv=0; flv<CITYFLAVOURS; flv++) {
		flavour = &cityflavour[flv];
		//buildings = &s_buildingLists[flavour->buildingListIdx];
       
		int tries;
		for (tries=20; tries--; ) {
			//const citybuilding_t &bt = buildings->buildings[rand.Int32(buildings->numBuildings)];
			const Building *b = m_candidateBuildings[Building::TYPE_NORMAL][rand.Int32(m_candidateBuildings[Building::TYPE_NORMAL].size())];
			model = b->model;
			modelRadXZ = b->xzRadius;
			cmesh = b->collMesh;
			if (modelRadXZ < rad) break;
			if (tries == 0) return;
		}
		
		bool tooDistant = ((flavour->center - cent).Length()*(1.0/flavour->size) > rand.Double());
		if (!tooDistant) {
			found = true;
			break;
		}
	}

	if (!found)
		if (rad <= MIN_SEG_SIZE)
			return;

	if (!found || rad > modelRadXZ*2.0) {
		vector3d a = (div.p1+div.p2)*0.5;
		vector3d b = (div.p2+div.p3)*0.5;
		vector3d c = (div.p3+div.p4)*0.5;
		vector3d d = (div.p4+div.p1)*0.5;
		vector3d e = (div.p1+div.p2+div.p3+div.p4)*0.25;

		PutCityBit(rand, rot, Division(div.p1, a, e, d));
		PutCityBit(rand, rot, Division(a, div.p2, b, e));
		PutCityBit(rand, rot, Division(e, b, div.p3, c));
		PutCityBit(rand, rot, Division(d, e, c, div.p4));
	} else {
		cent = cent.Normalized();
		double height = m_planet->GetTerrainHeight(cent);
		/* don't position below sealevel! */
		if (height - m_planet->GetSBody()->GetRadius() <= 0.0) return;
		cent = cent * height;

		Geom *geom = new Geom(cmesh->geomTree);
		int rotTimes90 = rand.Int32(4);
		matrix4x4d grot = rot * matrix4x4d::RotateYMatrix(M_PI*0.5*double(rotTimes90));
		geom->MoveTo(grot, cent);
		geom->SetUserData(this);
//		f->AddStaticGeom(geom);

		BuildingDef def = { model, cmesh->GetBoundingRadius(), rotTimes90, cent, geom, false };
		m_buildings.push_back(def);
	}
}

void CityOnPlanet::AddStaticGeomsToCollisionSpace()
{
	int skipMask;
	switch (Pi::detail.cities) {
		case 0: skipMask = 0xf; break;
		case 1: skipMask = 0x7; break;
		case 2: skipMask = 0x3; break;
		case 3: skipMask = 0x1; break;
		default:
			skipMask = 0; break;
	}
	for (unsigned int i=0; i<m_buildings.size(); i++) {
		if (i & skipMask) {
			m_buildings[i].isEnabled = false;
		} else {
			m_frame->AddStaticGeom(m_buildings[i].geom);
			m_buildings[i].isEnabled = true;
		}
	}
	m_detailLevel = Pi::detail.cities;
}

void CityOnPlanet::RemoveStaticGeomsFromCollisionSpace()
{
	for (unsigned int i=0; i<m_buildings.size(); i++) {
		m_frame->RemoveStaticGeom(m_buildings[i].geom);
		m_buildings[i].isEnabled = false;
	}
}

void CityOnPlanet::Init()
{
	if (s_buildingsLoaded)
		return;
	
	lua_State *l = LmrGetLuaState();

	std::vector<LmrModel*> models;
	LmrGetModelsWithTag("building", models);
	for (std::vector<LmrModel*>::iterator i = models.begin(); i != models.end(); i++) {
		Building b;
		b.model = (*i);
		b.collMesh = new LmrCollMesh(b.model, &cityobj_params);

		double maxx = std::max(fabs(b.collMesh->GetAabb().max.x), fabs(b.collMesh->GetAabb().min.x));
		double maxy = std::max(fabs(b.collMesh->GetAabb().max.z), fabs(b.collMesh->GetAabb().min.z));
		b.xzRadius = sqrt(maxx*maxx + maxy*maxy);

		LUA_DEBUG_START(l);

		b.model->PushAttributeToLuaStack("building_info");
		int info = lua_gettop(l);

		lua_getfield(l, info, "type");
		if (lua_isnil(l, -1))
			b.type = Building::TYPE_NORMAL;
		else
			b.type = static_cast<Building::BuildingType>(LuaConstants::GetConstant(l, "BuildingType", luaL_checkstring(l, -1)));

		lua_getfield(l, info, "environment");
		if (lua_isnil(l, -1))
			b.environment = Building::ENV_EARTHLIKE;
		else
			b.environment = static_cast<Building::BuildingEnvironment>(LuaConstants::GetConstant(l, "BuildingEnvironment", luaL_checkstring(l, -1)));

		lua_getfield(l, info, "min_city_size");
		if (lua_isnil(l, -1))
			b.minCitySize = Building::SIZE_TINY;
		else
			b.minCitySize = static_cast<Building::BuildingCitySize>(LuaConstants::GetConstant(l, "BuildingCitySize", luaL_checkstring(l, -1)));

		lua_getfield(l, info, "max_city_size");
		if (lua_isnil(l, -1))
			b.maxCitySize = Building::SIZE_HUGE;
		else
			b.maxCitySize = static_cast<Building::BuildingCitySize>(LuaConstants::GetConstant(l, "BuildingCitySize", luaL_checkstring(l, -1)));

		lua_getfield(l, info, "population");
		if (lua_isnil(l, -1))
			b.population = 1.0;
		else
			b.population = luaL_checknumber(l, -1);

		lua_pop(l, 6);

		LUA_DEBUG_END(l, 0);

		s_buildings.push_back(b);
	}

	s_buildingsLoaded = true;
}

CityOnPlanet::~CityOnPlanet()
{
	// frame may be null (already removed from 
	for (unsigned int i=0; i<m_buildings.size(); i++) {
		m_frame->RemoveStaticGeom(m_buildings[i].geom);
		delete m_buildings[i].geom;
	}
}

CityOnPlanet::CityOnPlanet(Planet *planet, SpaceStation *station, Uint32 seed) :
	m_planet(planet),
	m_frame(planet->GetFrame()),
	m_detailLevel(Pi::detail.cities)
{
	assert(s_buildingsLoaded);

	const SBody *body = m_planet->GetSBody();

	Building::BuildingEnvironment environment = 
		(body->m_life > fixed(7,10) && body->m_volatileGas > fixed(2,10))
			? Building::ENV_EARTHLIKE : Building::ENV_HOSTILE;

	int nstarports = 0;
	for (std::vector<SBody*>::const_iterator i = body->children.begin(); i != body->children.end(); i++)
		if ((*i)->type == SBody::TYPE_STARPORT_SURFACE)
			nstarports++;
	
	double population = body->m_population.ToDouble() / nstarports;
	Building::BuildingCitySize size = 
		population >= 1.00 ? Building::SIZE_HUGE :
		population >= 0.75 ? Building::SIZE_LARGE :
		population >= 0.50 ? Building::SIZE_MEDIUM :
		population >= 0.25 ? Building::SIZE_SMALL :
		                     Building::SIZE_TINY;

	for (std::vector<Building>::const_iterator i = s_buildings.begin(); i != s_buildings.end(); i++) {
		Building b = (*i);
		if (b.environment == environment && b.minCitySize <= size && b.maxCitySize >= size)
			m_candidateBuildings[b.type].push_back(&(*i));
	}

	Aabb aabb;
	station->GetAabb(aabb);
	
	matrix4x4d m;
	station->GetRotMatrix(m);

	vector3d mx = m*vector3d(1,0,0);
	vector3d mz = m*vector3d(0,0,1);
		
	MTRand rand;
	rand.seed(seed);

	vector3d p = station->GetPosition();

	vector3d p1, p2, p3, p4;
	double sizex = START_SEG_SIZE;// + rand.Int32((int)START_SEG_SIZE);
	double sizez = START_SEG_SIZE;// + rand.Int32((int)START_SEG_SIZE);
	
	// always have random shipyard buildings around the space station
	//cityflavour[0].buildingListIdx = 0;//2;
	cityflavour[0].center = p;
	cityflavour[0].size = 500;

	for (int i=1; i<CITYFLAVOURS; i++) {
		//cityflavour[i].buildingListIdx = MAX_BUILDING_LISTS>1 ? rand.Int32(MAX_BUILDING_LISTS-1) : 0;
		//citybuildinglist_t *blist = &s_buildingLists[cityflavour[i].buildingListIdx];
		double a = rand.Int32(-1000,1000);
		double b = rand.Int32(-1000,1000);
		cityflavour[i].center = p + a*mx + b*mz;
		//cityflavour[i].size = rand.Int32(int(blist->minRadius), int(blist->maxRadius));
		cityflavour[i].size = rand.Int32(800, 2000);
	}
	
	for (int side=0; side<4; side++) {
		Division d;

		/* put buildings on all sides of spaceport */
		switch(side) {
			case 3:
				d.p1 = p + mx*(aabb.min.x) + mz*aabb.min.z;
				d.p2 = p + mx*(aabb.min.x) + mz*(aabb.min.z-sizez);
				d.p3 = p + mx*(aabb.min.x+sizex) + mz*(aabb.min.z-sizez);
				d.p4 = p + mx*(aabb.min.x+sizex) + mz*(aabb.min.z);
				break;
			case 2:
				d.p1 = p + mx*(aabb.min.x-sizex) + mz*aabb.max.z;
				d.p2 = p + mx*(aabb.min.x-sizex) + mz*(aabb.max.z-sizez);
				d.p3 = p + mx*(aabb.min.x) + mz*(aabb.max.z-sizez);
				d.p4 = p + mx*(aabb.min.x) + mz*(aabb.max.z);
				break;
			case 1:
				d.p1 = p + mx*(aabb.max.x-sizex) + mz*aabb.max.z;
				d.p2 = p + mx*(aabb.max.x) + mz*aabb.max.z;
				d.p3 = p + mx*(aabb.max.x) + mz*(aabb.max.z+sizez);
				d.p4 = p + mx*(aabb.max.x-sizex) + mz*(aabb.max.z+sizez);
				break;
			default:
			case 0:
				d.p1 = p + mx*aabb.max.x + mz*aabb.min.z;
				d.p2 = p + mx*(aabb.max.x+sizex) + mz*aabb.min.z;
				d.p3 = p + mx*(aabb.max.x+sizex) + mz*(aabb.min.z+sizez);
				d.p4 = p + mx*aabb.max.x + mz*(aabb.min.z+sizez);
				break;
		}

		PutCityBit(rand, m, d);
	}

	AddStaticGeomsToCollisionSpace();
}

void CityOnPlanet::Render(const SpaceStation *station, const vector3d &viewCoords, const matrix4x4d &viewTransform)
{
	matrix4x4d rot[4];
	station->GetRotMatrix(rot[0]);

	// change detail level if necessary
	if (m_detailLevel != Pi::detail.cities) {
		RemoveStaticGeomsFromCollisionSpace();
		AddStaticGeomsToCollisionSpace();
	}
	
	rot[0] = viewTransform * rot[0];
	for (int i=1; i<4; i++) {
		rot[i] = rot[0] * matrix4x4d::RotateYMatrix(M_PI*0.5*double(i));
	}

	GetFrustum(planes);
	
	memset(&cityobj_params, 0, sizeof(LmrObjParams));
	// this fucking rubbish needs to be moved into a function
	cityobj_params.argDoubles[1] = Pi::GetGameTime();
	cityobj_params.argDoubles[2] = Pi::GetGameTime() / 60.0;
	cityobj_params.argDoubles[3] = Pi::GetGameTime() / 3600.0;
	cityobj_params.argDoubles[4] = Pi::GetGameTime() / (24*3600.0);


	for (std::vector<BuildingDef>::const_iterator i = m_buildings.begin();
			i != m_buildings.end(); ++i) {

		if (!(*i).isEnabled) continue;

		vector3d pos = viewTransform * (*i).pos;
		/* frustum cull */
		bool cull = false;
		for (int j=0; j<6; j++) {
			if (planes[j].DistanceToPoint(pos)+(*i).clipRadius < 0) {
				cull = true;
				break;
			}
		}
		if (cull) continue;
		matrix4x4f _rot;
		for (int e=0; e<16; e++) _rot[e] = float(rot[(*i).rotation][e]);
		_rot[12] = float(pos.x);
		_rot[13] = float(pos.y);
		_rot[14] = float(pos.z);
		(*i).model->Render(_rot, &cityobj_params);
	}
}

