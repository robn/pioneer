#ifndef _CITYONPLANET_H
#define _CITYONPLANET_H

#include "libs.h"
#include "mtrand.h"
#include "Object.h"
#include "LmrModel.h"

class Planet;
class SpaceStation;
class Frame;
class Geom;

#define CITY_ON_PLANET_RADIUS 5000.0

struct Building {
	LmrModel *model;
	const LmrCollMesh *collMesh;
	double xzRadius;

	enum BuildingType {
		TYPE_GENERAL,
		TYPE_POWER
	};
	enum BuildingEnvironment {
		ENV_EARTHLIKE,
		ENV_HOSTILE
	};
	enum BuildingCitySize {
		SIZE_TINY,
		SIZE_SMALL,
		SIZE_MEDIUM,
		SIZE_LARGE,
		SIZE_HUGE
	};

	BuildingType type;
	BuildingEnvironment environment;
	BuildingCitySize minCitySize, maxCitySize;
	double population;
};


class CityOnPlanet: public Object {
public:
	OBJDEF(CityOnPlanet, Object, CITYONPLANET);
	CityOnPlanet(Planet *planet, SpaceStation *station, Uint32 seed);
	virtual ~CityOnPlanet();
	void Render(const SpaceStation *station, const vector3d &viewCoords, const matrix4x4d &viewTransform);
	inline Planet *GetPlanet() const { return m_planet; }

	static void Init();
private:
	void PutCityBit(MTRand &rand, const matrix4x4d &rot, vector3d p1, vector3d p2, vector3d p3, vector3d p4);
	void AddStaticGeomsToCollisionSpace();
	void RemoveStaticGeomsFromCollisionSpace();

	Planet *m_planet;
	Frame *m_frame;
	int m_detailLevel;

	static std::vector<Building> s_buildings;
	static bool s_buildingsLoaded;

	std::vector<const Building *> m_candidateBuildings;

	struct BuildingDef {
		LmrModel *model;
		float clipRadius;
		int rotation; // 0-3
		vector3d pos;
		Geom *geom;
		// may not be at lower detail level
		bool isEnabled;
	};
	std::vector<BuildingDef> m_buildings;


};

#endif /* _CITYONPLANET_H */
