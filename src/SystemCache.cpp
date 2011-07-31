#include "SystemCache.h"
#include "ThreadManager.h"
#include "Pi.h"

SystemCache::SystemCache()
{
}

SystemCache::~SystemCache()
{
}

StarSystem *SystemCache::GetCached(const SystemPath &path)
{
	StarSystem *s = 0;

	std::map<SystemPath,StarSystem*>::iterator i = m_cachedSystems.find(path);
	if (i != m_cachedSystems.end())
		s = (*i).second;

	if (!s) {
		s = new StarSystem(path);
		m_cachedSystems.insert( std::pair<SystemPath,StarSystem*>(path, s) );
	}

	s->IncRefCount();
	return s;
}

void SystemCache::GetCachedAsyncThreadEntry(AsyncData *data)
{
	data->sys = new StarSystem(data->path);
}

void SystemCache::OnGetCachedAsyncCompleted(AsyncData *data)
{
	m_cachedSystems.insert( std::make_pair(data->path, data->sys) );

	std::map<SystemPath, std::list<AsyncCallback> >::iterator i = m_pendingCallbacks.find(data->path);
	if (i != m_pendingCallbacks.end()) {
		for (std::list<AsyncCallback>::iterator j = (*i).second.begin(); j != (*i).second.end(); j++) {
			(*(*j))(data->sys);
		}
	}
	m_pendingCallbacks.erase(i);

	delete data;
}

void SystemCache::GetCachedAsync(const SystemPath &path, AsyncCallback callback)
{
	StarSystem *s;

	{
		std::map<SystemPath,StarSystem*>::iterator i = m_cachedSystems.find(path);
		if (i != m_cachedSystems.end()) {
			s = (*i).second;
			(*callback)(s);
			return;
		}
	}

	{
		std::map<SystemPath, std::list<AsyncCallback> >::iterator i = m_pendingCallbacks.find(path);
		if (i != m_pendingCallbacks.end()) {
			(*i).second.push_back(callback);
			return;
		}
	}
	
	std::list<AsyncCallback> pendingCallbacks;
	pendingCallbacks.push_back(callback);
	m_pendingCallbacks.insert( std::make_pair(path, pendingCallbacks) );

	AsyncData *data = new AsyncData;
	data->path = path;
	Thread<AsyncData> *t = Pi::threadManager->StartThread(&SystemCache::GetCachedAsyncThreadEntry, data);
	t->onCompleted.connect(sigc::mem_fun(this, &SystemCache::OnGetCachedAsyncCompleted));
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

