#include "SystemCache.h"
#include "Sector.h"
#include "StarSystem.h"
#include "RandomSystemGenerator.h"
#include "CustomSystemGenerator.h"

RefCountedPtr<StarSystem> SystemCache::GetSystem(const SystemPath &path)
{
	SystemPath sysPath(path.SystemOnly());

	SystemCacheMap::iterator i = m_cache.find(sysPath);
	if (i != m_cache.end())
		return (*i).second;

	Sector sec(sysPath.sectorX, sysPath.sectorY, sysPath.sectorZ);
	assert(sysPath.systemIndex < sec.m_systems.size());

	const SystemDescriptor &sysDesc = sec.m_systems[sysPath.systemIndex];
	
	// XXX choose appropriate generator here
	// XXX RefCountedPtr<StarSystem> s(sysDesc.customSys ? CustomSystemGenerator(sysDesc).GenerateSystem() : RandomSystemGenerator(sysDesc).GenerateSystem());
	RefCountedPtr<StarSystem> s(RandomSystemGenerator(sysDesc).GenerateSystem());
	m_cache.insert(std::make_pair(sysPath, s));

	return s;
}

void SystemCache::Shrink()
{
	SystemCacheMap::iterator i = m_cache.begin();
	while (i != m_cache.end()) {
		if ((*i).second->GetRefCount() == 1)
			m_cache.erase(i++);
		else
			i++;
	}
}

void SystemCache::Empty()
{
	m_cache.empty();
}
