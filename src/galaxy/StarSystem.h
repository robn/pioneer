#ifndef _STARSYSTEM_H
#define _STARSYSTEM_H

#include "libs.h"
#include "EquipType.h"
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

	StarSystem(
		const SystemDescriptor &desc, const std::vector<SystemBody*> &bodies, const EconomicData &_econ,
		bool _unexplored, const std::string &_shortDesc, const std::string &_longDesc = "");
	~StarSystem();

	SystemBody *GetBodyByPath(const SystemPath &path) const;
	static void Serialize(Serializer::Writer &wr, StarSystem *);
	static RefCountedPtr<StarSystem> Unserialize(Serializer::Reader &rd);

	const SystemDescriptor &GetDescriptor() const { return m_desc; }
	const EconomicData &GetEconomicData() const { return m_econ; }
	bool IsUnexplored() const { return m_unexplored; }
	const std::string &GetShortDescription() const { return m_shortDesc; }
	const std::string &GetLongDescription() const { return m_longDesc; }

	SystemBody *rootBody;
	std::vector<SystemBody*> m_spaceStations;
	// index into this will be the SystemBody ID used by SystemPath
	std::vector<SystemBody*> m_bodies;

private:
	SystemDescriptor m_desc;

	EconomicData m_econ;

	bool m_unexplored;

	std::string m_shortDesc;
	std::string m_longDesc;

	bool m_isCustom;
	bool m_hasCustomBodies;
};

#endif /* _STARSYSTEM_H */
