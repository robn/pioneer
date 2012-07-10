#include "RandomSystemGenerator.h"

RandomSystemGenerator::RandomSystemGenerator(const SystemPath &path) : SystemGenerator(),
	m_path(path)
{
}

StarSystem *RandomSystemGenerator::GenerateSystem() const
{
	return 0;
}
