#ifndef _RANDOMSYSTEMGENERATOR_H
#define _RANDOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class RandomSystemGenerator : public SystemGenerator {
public:
	RandomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem();

private:
	static SystemBody *NewStar(SystemBody::BodyType type, MTRand &rand);
	static SystemBody *NewBinaryPair(SystemBody::BodyType typeA, SystemBody::BodyType typeB, MTRand &rand);

	void MakeBinaryPair(SystemBody *a, SystemBody *b, fixed minDist, MTRand &rand);
	void MakePlanetsAround(SystemBody *primary, MTRand &rand);
	void PopulateAddStations(SystemBody *body);

	const SystemDescriptor m_desc;

	std::vector<SystemBody*> m_bodies;
};

#endif
