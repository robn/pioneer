#ifndef _SYSTEMCACHE_H
#define _SYSTEMCACHE_H

#include "SystemPath.h"
#include "RefCounted.h"
#include <map>

class StarSystem;

class SystemCache {
public:
	static RefCountedPtr<StarSystem> GetCached(const SystemPath &path);
	static void ShrinkCache();

private:
	SystemCache() {}
	~SystemCache();

	typedef std::map< SystemPath,RefCountedPtr<StarSystem> > SystemCacheMap;
	SystemCacheMap m_cache;

	static SystemCache s_cache;
};

#endif
