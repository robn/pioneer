#include "SystemCache.h"
#include "StarSystem.h"

SystemCache SystemCache::s_cache;

RefCountedPtr<StarSystem> SystemCache::GetCached(const SystemPath &path)
{
	SystemPath sysPath(path.SystemOnly());

	SystemCacheMap::iterator i = s_cache.m_cache.find(sysPath);
	if (i != s_cache.m_cache.end())
		return (*i).second;
	
	RefCountedPtr<StarSystem> s(new StarSystem(sysPath));
	s_cache.m_cache.insert(std::make_pair(sysPath, s));

	return s;
}

void SystemCache::ShrinkCache()
{
	SystemCacheMap::iterator i = s_cache.m_cache.begin();
	while (i != s_cache.m_cache.end()) {
		if ((*i).second->GetRefCount() == 1)
			s_cache.m_cache.erase(i++);
		else
			i++;
	}
}

SystemCache::~SystemCache()
{
	ShrinkCache();
}
