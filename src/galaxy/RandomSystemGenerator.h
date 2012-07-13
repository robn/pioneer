#ifndef _RANDOMSYSTEMGENERATOR_H
#define _RANDOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class RandomSystemGenerator : public SystemGenerator {
public:
	RandomSystemGenerator(const SystemDescriptor &sysDesc) : SystemGenerator(), m_sysDesc(sysDesc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem() const;

private:
	const SystemDescriptor m_sysDesc;
};

#endif
