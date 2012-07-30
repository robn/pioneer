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
	friend class CustomSystemGenerator;

	StarSystem(const SystemDescriptor &_desc, const std::vector<SystemBody*> &bodies);
	~StarSystem();

	const SystemDescriptor desc;

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
	void MakeShortDescription(MTRand &rand);
	void Populate();

	std::string m_shortDesc, m_longDesc;
	SysPolit m_polit;

	bool m_isCustom;
	bool m_hasCustomBodies;
};

#endif /* _STARSYSTEM_H */
