#ifndef _SYSTEMCACHE_H
#define _SYSTEMCACHE_H

#include "SystemPath.h"
#include "RefCounted.h"
#include <map>

class StarSystem;

class SystemCache {
public:
	RefCountedPtr<StarSystem> GetSystem(const SystemPath &path);
	void Shrink();
	void Empty();

private:
	typedef std::map< SystemPath,RefCountedPtr<StarSystem> > SystemCacheMap;
	SystemCacheMap m_cache;
};

#endif
