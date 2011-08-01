#include "SystemCache.h"
#include "Pi.h"

SystemCache::SystemCache() : m_asyncData(0)
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

void SystemCache::GetCachedAsyncThreadEntry(Thread<AsyncData> *thread, AsyncData *data)
{
	printf("thread entry\n");

	SDL_mutexP(data->mutex);
	while(data->paths.size()) {
		printf("  %d in queue\n", data->paths.size());
		SystemPath path = data->paths.front();
		data->paths.pop();
		SDL_mutexV(data->mutex);

		printf("loading [%d,%d,%d]\n", path.sectorX, path.sectorY, path.systemIndex);
		StarSystem *sys = new StarSystem(path);
		printf("  loaded %s\n", sys->GetName().c_str());

		SDL_mutexP(data->mutex);
		data->systems.push(sys);
		SDL_mutexV(data->mutex);

		thread->ReportUpdate();

		SDL_mutexP(data->mutex);
	}

	SDL_mutexV(data->mutex);

	printf("thread exit\n");
}

void SystemCache::OnGetCachedAsyncUpdated(AsyncData *data)
{
	SDL_mutexP(data->mutex);

	while (data->systems.size()) {
		StarSystem *sys = data->systems.front();
		data->systems.pop();
		SDL_mutexV(data->mutex);

		SystemPath path = sys->GetPath();
		m_cachedSystems.insert( std::make_pair(path, sys) );

		std::map<SystemPath, std::list<AsyncCallback> >::iterator i = m_pendingCallbacks.find(path);
		if (i != m_pendingCallbacks.end()) {
			for (std::list<AsyncCallback>::iterator j = (*i).second.begin(); j != (*i).second.end(); j++) {
				sys->IncRefCount();
				(*j)(sys);
			}
			m_pendingCallbacks.erase(i);
		}

		SDL_mutexP(data->mutex);
	}
	
	SDL_mutexV(data->mutex);
}

void SystemCache::OnGetCachedAsyncCompleted(AsyncData *data)
{
	delete m_asyncData;
	m_asyncData = 0;
}

void SystemCache::GetCachedAsync(const SystemPath &path, AsyncCallback callback)
{
	StarSystem *s;

	{
		std::map<SystemPath,StarSystem*>::iterator i = m_cachedSystems.find(path);
		if (i != m_cachedSystems.end()) {
			s = (*i).second;
			s->IncRefCount();
			callback(s);
			return;
		}
	}

	{
		std::map<SystemPath, std::list<AsyncCallback> >::iterator i = m_pendingCallbacks.find(path);
		if (i != m_pendingCallbacks.end()) {
			for (std::list<AsyncCallback>::iterator j = (*i).second.begin(); j != (*i).second.end(); j++)
				if ((*j) == callback)
					return;
			(*i).second.push_back(callback);
			return;
		}
	}
	
	std::list<AsyncCallback> pendingCallbacks;
	pendingCallbacks.push_back(callback);
	m_pendingCallbacks.insert( std::make_pair(path, pendingCallbacks) );

	if (!m_asyncData) {
		m_asyncData = new AsyncData;
		m_asyncData->mutex = SDL_CreateMutex();
		m_asyncData->paths.push(path);

		Thread<AsyncData> *t = Pi::threadManager->StartThread(&SystemCache::GetCachedAsyncThreadEntry, m_asyncData);
		t->onUpdated.connect(sigc::mem_fun(this, &SystemCache::OnGetCachedAsyncUpdated));
		t->onCompleted.connect(sigc::mem_fun(this, &SystemCache::OnGetCachedAsyncCompleted));
	}

	else {
		SDL_mutexP(m_asyncData->mutex);
		m_asyncData->paths.push(path);
		SDL_mutexV(m_asyncData->mutex);
	}
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

