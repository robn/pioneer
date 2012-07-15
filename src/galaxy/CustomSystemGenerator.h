#ifndef _CUSTOMSYSTEMGENERATOR_H
#define _CUSTOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class CustomSystem;
class CustomSystemBody;

class CustomSystemGenerator : public SystemGenerator {
public:
	CustomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem() const;

private:
	static void GenerateFromCustom(StarSystem *s, const CustomSystem *, MTRand &rand);
	static void CustomGetKidsOf(StarSystem *s, SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand);

	const SystemDescriptor m_desc;
};

#endif
