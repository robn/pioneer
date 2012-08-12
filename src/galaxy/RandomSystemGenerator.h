#ifndef _RANDOMSYSTEMGENERATOR_H
#define _RANDOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"
#include "StarSystem.h"

class RandomSystemGenerator : public SystemGenerator {
public:
	RandomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem();

private:
	void GenerateSystemSingle(MTRand &rand);
	void GenerateSystemDouble(MTRand &rand);
	void GenerateSystemTriple(MTRand &rand);
	void GenerateSystemQuadruple(MTRand &rand);

	static SystemBody *NewStar(SystemBody::BodyType type, MTRand &rand);
	static SystemBody *MakeBinaryPair(SystemBody *a, SystemBody *b, MTRand &rand);

	void Populate();
	void PopulateStage1(SystemBody *sbody, fixed &outTotalPop);
	void MakeShortDescription(MTRand &rand);

	void MakePlanetsAround(SystemBody *primary, MTRand &rand);
	void PopulateAddStations(SystemBody *body);

	bool m_unexplored;

	std::string m_shortDesc, m_longDesc;
	SysPolit m_polit;

	const SystemDescriptor m_desc;
	std::vector<SystemBody*> m_bodies;
	StarSystem::EconomicData m_econ;
};

#endif
