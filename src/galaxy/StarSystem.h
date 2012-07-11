#ifndef _STARSYSTEM_H
#define _STARSYSTEM_H

#include "libs.h"
#include "EquipType.h"
#include "Polit.h"
#include "Serializer.h"
#include <vector>
#include <string>
#include "DeleteEmitter.h"
#include "RefCounted.h"
#include "galaxy/SystemPath.h"
#include "SystemBody.h"

class CustomSystemBody;
class CustomSystem;
class SystemBody;

// doubles - all masses in Kg, all lengths in meters
// fixed - any mad scheme

enum EconType { // <enum name=EconType prefix=ECON_>
	ECON_MINING = 1<<0,
	ECON_AGRICULTURE = 1<<1,
	ECON_INDUSTRY = 1<<2,
};

class StarSystem : public DeleteEmitter, public RefCounted {
public:
	friend class SystemBody;

	static RefCountedPtr<StarSystem> GetCached(const SystemPath &path);
	static void ShrinkCache();

	const std::string &GetName() const { return m_name; }
	SystemPath GetPathOf(const SystemBody *sbody) const;
	SystemBody *GetBodyByPath(const SystemPath &path) const;
	static void Serialize(Serializer::Writer &wr, StarSystem *);
	static RefCountedPtr<StarSystem> Unserialize(Serializer::Reader &rd);
	const SystemPath &GetPath() const { return m_path; }
	const char *GetShortDescription() const { return m_shortDesc.c_str(); }
	const char *GetLongDescription() const { return m_longDesc.c_str(); }
	int GetNumStars() const { return m_numStars; }
	const SysPolit &GetSysPolit() const { return m_polit; }

	SystemBody *rootBody;
	std::vector<SystemBody*> m_spaceStations;
	// index into this will be the SystemBody ID used by SystemPath
	std::vector<SystemBody*> m_bodies;

	fixed m_metallicity;
	fixed m_industrial;
	fixed m_agricultural;

	fixed m_humanProx;
	fixed m_totalPop;
	// percent price alteration
	int m_tradeLevel[Equip::TYPE_MAX];
	int m_econType;
	int m_seed;

	bool m_unexplored;

	int GetCommodityBasePriceModPercent(int t) {
		return m_tradeLevel[t];
	}
private:
	StarSystem(const SystemPath &path);
	~StarSystem();

	SystemBody *NewBody() {
		SystemBody *body = new SystemBody;
		body->path = m_path;
		body->path.bodyIndex = m_bodies.size();
		m_bodies.push_back(body);
		return body;
	}
	void MakeShortDescription(MTRand &rand);
	void MakePlanetsAround(SystemBody *primary, MTRand &rand);
	void MakeStarOfType(SystemBody *sbody, SystemBody::BodyType type, MTRand &rand);
	void MakeStarOfTypeLighterThan(SystemBody *sbody, SystemBody::BodyType type, fixed maxMass, MTRand &rand);
	void MakeBinaryPair(SystemBody *a, SystemBody *b, fixed minDist, MTRand &rand);
	void CustomGetKidsOf(SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand);
	void GenerateFromCustom(const CustomSystem *, MTRand &rand);
	void Populate(bool addSpaceStations);

	SystemPath m_path;
	int m_numStars;
	std::string m_name;
	std::string m_shortDesc, m_longDesc;
	SysPolit m_polit;

	bool m_isCustom;
	bool m_hasCustomBodies;
};

#endif /* _STARSYSTEM_H */
