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
		TYPE_NORMAL,
		TYPE_STARPORT
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

	struct ZoneDef {
		Building::BuildingType type;
		double minRadius, maxRadius;
	};

private:

	struct Zone {
		ZoneDef def;
		vector3d centre;
		double size;
	};

	class Division {
	public:
		Division() {}
		Division(vector3d _p1, vector3d _p2, vector3d _p3, vector3d _p4) : p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}
		vector3d p1, p2, p3, p4;
	};

	void PutCityBit(MTRand &rand, const matrix4x4d &rot, const Zone zones[], Division d);

	void AddStaticGeomsToCollisionSpace();
	void RemoveStaticGeomsFromCollisionSpace();

	Planet *m_planet;
	Frame *m_frame;
	int m_detailLevel;

	static std::vector<Building> s_buildings;
	static bool s_buildingsLoaded;

	std::map< Building::BuildingType, std::vector<const Building *> > m_candidateBuildings;

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
