#ifndef _CUSTOMSYSTEMGENERATOR_H
#define _CUSTOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class CustomSystem;
class CustomSystemBody;

class CustomSystemGenerator : public SystemGenerator {
public:
	CustomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem();

private:
	void GenerateFromCustom(const CustomSystem *, MTRand &rand);
	void CustomGetKidsOf(SystemBody *parent, const std::vector<CustomSystemBody*> &children, int *outHumanInfestedness, MTRand &rand);

	const SystemDescriptor m_desc;

	std::vector<SystemBody*> m_bodies;
};

#endif
