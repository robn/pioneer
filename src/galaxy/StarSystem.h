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
#include "SystemDescriptor.h"
#include "SystemPath.h"
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
	friend class RandomSystemGenerator;

	StarSystem(const SystemDescriptor &_desc) : desc(_desc) {}
	~StarSystem();

	const SystemDescriptor desc;

	SystemPath GetPathOf(const SystemBody *sbody) const;
	SystemBody *GetBodyByPath(const SystemPath &path) const;
	static void Serialize(Serializer::Writer &wr, StarSystem *);
	static RefCountedPtr<StarSystem> Unserialize(Serializer::Reader &rd);
	const char *GetShortDescription() const { return m_shortDesc.c_str(); }
	const char *GetLongDescription() const { return m_longDesc.c_str(); }
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

	bool m_unexplored;

	int GetCommodityBasePriceModPercent(int t) {
		return m_tradeLevel[t];
	}

private:
	SystemBody *NewBody() {
		SystemBody *body = new SystemBody;
		body->path = desc.path;
		body->path.bodyIndex = m_bodies.size();
		m_bodies.push_back(body);
		return body;
	}

	void AddBody(SystemBody *body) {
		body->path = desc.path;
		body->path.bodyIndex = m_bodies.size();
		m_bodies.push_back(body);
	}

	void MakeShortDescription(MTRand &rand);
	void MakePlanetsAround(SystemBody *primary, MTRand &rand);
	SystemBody MakeStarOfTypeLighterThan(SystemBody::BodyType type, fixed maxMass, MTRand &rand);
	void CustomGetKidsOf(SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand);
	void GenerateFromCustom(const CustomSystem *, MTRand &rand);
	void Populate(bool addSpaceStations);

	std::string m_shortDesc, m_longDesc;
	SysPolit m_polit;

	bool m_isCustom;
	bool m_hasCustomBodies;
};

#endif /* _STARSYSTEM_H */
