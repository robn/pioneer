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

	struct EconomicData;

	StarSystem(const SystemDescriptor &_desc, const std::vector<SystemBody*> &bodies, const EconomicData &_econ, bool _unexplored);
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

	struct EconomicData {
		EconomicData() : metallicity(0), industrial(0), agricultural(0), humanProx(0), totalPop(0), econType(0) {
			for (int i = 0; i < Equip::TYPE_MAX; i++)
				tradeLevel[i] = 0;
		}

		EconomicData(fixed _metallicity, fixed _industrial, fixed _agricultural, fixed _humanProx, fixed _totalPop, int _econType) :
			metallicity(_metallicity), industrial(_industrial), agricultural(_agricultural), humanProx(_humanProx), totalPop(_totalPop), econType(_econType) {
			for (int i = 0; i < Equip::TYPE_MAX; i++)
				tradeLevel[i] = 0;
		}

		fixed metallicity;  // metals -> mining
		fixed industrial;   // industrial capacity
		fixed agricultural; // agricultural capacity

		fixed humanProx;    // distance to Sol, 0->1 == far->near
		fixed totalPop;     // population in billions

		// percent price alteration
		int tradeLevel[Equip::TYPE_MAX];
		int econType;
	};
	EconomicData econ;

	bool unexplored;

private:
	void MakeShortDescription(MTRand &rand);
	void Populate();

	std::string m_shortDesc, m_longDesc;
	SysPolit m_polit;

	bool m_isCustom;
	bool m_hasCustomBodies;
};

#endif /* _STARSYSTEM_H */
