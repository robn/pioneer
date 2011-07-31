#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (std::list<ThreadBase*>::iterator i = m_threads.begin(); i != m_threads.end(); i++)
		(*i)->Kill();
}

void ThreadManager::CleanupCompletedThreads()
{
	std::list<ThreadBase*>::iterator i = m_threads.begin();
	while (i != m_threads.end()) {
		ThreadBase *t = (*i);

		if (t->GetState() == ThreadBase::COMPLETED) {
			i = m_threads.erase(i);
			t->SignalCompleted();
			delete t;
			continue;
		}

		i++;
	}
}
