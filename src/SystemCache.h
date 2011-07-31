#ifndef _SYSTEMCACHE_H
#define _SYSTEMCACHE_H

#include "libs.h"
#include "StarSystem.h"
#include "SystemPath.h"
#include <map>

class SystemCache {
public:
	SystemCache();
	~SystemCache();

	StarSystem *GetCached(const SystemPath &path);

	typedef sigc::slot<void,StarSystem*> AsyncCallback;
	void GetCachedAsync(const SystemPath &path, AsyncCallback callback);

	struct AsyncData {
		SystemPath path;
		StarSystem *sys;
	};
	static void GetCachedAsyncThreadEntry(AsyncData *data);
	void OnGetCachedAsyncCompleted(AsyncData *data);

	void ShrinkCache();

private:
	std::map<SystemPath,StarSystem*> m_cachedSystems;

	std::map<SystemPath, std::list<AsyncCallback> > m_pendingCallbacks;
};

#endif
