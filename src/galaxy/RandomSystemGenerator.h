#ifndef _RANDOMSYSTEMGENERATOR_H
#define _RANDOMSYSTEMGENERATOR_H

#include "SystemGenerator.h"
#include "SystemPath.h"

class RandomSystemGenerator : public SystemGenerator {
public:
	RandomSystemGenerator(const SystemPath &path);
	virtual ~RandomSystemGenerator();

	virtual StarSystem *GenerateSystem() const;

private:
	const SystemPath m_path;
};

#endif
