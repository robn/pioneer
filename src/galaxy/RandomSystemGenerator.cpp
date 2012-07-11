#include "RandomSystemGenerator.h"
#include "StarSystem.h"
#include <cassert>

RandomSystemGenerator::RandomSystemGenerator(const SystemPath &path) : SystemGenerator(),
	m_path(path)
{
	assert(path.IsSystemPath());
}

RefCountedPtr<StarSystem> RandomSystemGenerator::GenerateSystem() const
{
	return RefCountedPtr<StarSystem>(new StarSystem(m_path));
}
