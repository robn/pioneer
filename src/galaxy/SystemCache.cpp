#include "SystemCache.h"
#include "StarSystem.h"
#include "RandomSystemGenerator.h"

RefCountedPtr<StarSystem> SystemCache::GetSystem(const SystemPath &path)
{
	SystemPath sysPath(path.SystemOnly());

	SystemCacheMap::iterator i = m_cache.find(sysPath);
	if (i != m_cache.end())
		return (*i).second;
	
	// XXX choose appropriate generator here
	RefCountedPtr<StarSystem> s(RandomSystemGenerator(sysPath).GenerateSystem());
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
