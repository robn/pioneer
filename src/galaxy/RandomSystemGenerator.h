#ifndef _RANDOMSYSTEMGENERATOR_H
#define _RANDOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class RandomSystemGenerator : public SystemGenerator {
public:
	RandomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem() const;

private:
	static void MakeBinaryPair(SystemBody *a, SystemBody *b, fixed minDist, MTRand &rand);
	static void MakePlanetsAround(StarSystem *s, SystemBody *primary, MTRand &rand);

	const SystemDescriptor m_desc;
};

#endif
