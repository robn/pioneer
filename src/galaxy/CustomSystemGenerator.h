#ifndef _CUSTOMSYSTEMGENERATOR_H
#define _CUSTOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemDescriptor.h"

class CustomSystemGenerator : public SystemGenerator {
public:
	CustomSystemGenerator(const SystemDescriptor &desc) : SystemGenerator(), m_desc(desc) {}

	virtual RefCountedPtr<StarSystem> GenerateSystem() const;

private:
	const SystemDescriptor m_desc;
};

#endif
