#ifndef _SYSTEMCACHE_H
#define _SYSTEMCACHE_H

#include "StarSystem.h"
#include "SystemPath.h"
#include <map>


class SystemCache {
public:
	SystemCache();
	~SystemCache();

	StarSystem *GetCached(const SystemPath &path);
	void ShrinkCache();

private:
	std::map<SystemPath,StarSystem*> m_cachedSystems;
};

#endif
