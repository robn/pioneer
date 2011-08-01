#ifndef _SYSTEMCACHE_H
#define _SYSTEMCACHE_H

#include "libs.h"
#include "ThreadManager.h"
#include "StarSystem.h"
#include "SystemPath.h"
#include <map>
#include <queue>

class SystemCache {
public:
	SystemCache();
	~SystemCache();

	StarSystem *GetCached(const SystemPath &path);

	typedef sigc::slot<void,StarSystem*> AsyncCallback;
	void GetCachedAsync(const SystemPath &path, AsyncCallback callback);

	struct AsyncData {
		std::queue<SystemPath> paths;
		std::queue<StarSystem*> systems;
		SDL_mutex *mutex;
	};
	static void GetCachedAsyncThreadEntry(Thread<AsyncData> *thread, AsyncData *data);

	void OnGetCachedAsyncUpdated(AsyncData *data);
	void OnGetCachedAsyncCompleted(AsyncData *data);

	void ShrinkCache();

private:
	std::map<SystemPath,StarSystem*> m_cachedSystems;
	std::map<SystemPath, std::list<AsyncCallback> > m_pendingCallbacks;
	AsyncData *m_asyncData;
};

#endif
