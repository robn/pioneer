#include "SystemCache.h"

SystemCache::SystemCache()
{
}

SystemCache::~SystemCache()
{
}

StarSystem *SystemCache::GetCached(const SystemPath &path)
{
	StarSystem *s = 0;

	for (std::map<SystemPath,StarSystem*>::iterator i = m_cachedSystems.begin(); i != m_cachedSystems.end(); i++) {
		if ((*i).first == path)
			s = (*i).second;
	}

	if (!s) {
		s = new StarSystem(path);
		m_cachedSystems.insert( std::pair<SystemPath,StarSystem*>(path, s) );
	}

	s->IncRefCount();
	return s;
}

void SystemCache::ShrinkCache()
{
	std::map<SystemPath,StarSystem*>::iterator i = m_cachedSystems.begin();
	while (i != m_cachedSystems.end()) {
		StarSystem *s = (*i).second;
		if (s->GetRefCount() == 0) {
			delete s;
			m_cachedSystems.erase(i++);
		}
		else
			i++;
	}
}

