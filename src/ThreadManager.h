#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "libs.h"

class ThreadManager;

class ThreadBase {
	friend class ThreadManager;

	enum State {
		INIT,
		RUNNING,
		COMPLETED,
	};

protected:
	ThreadBase() : m_thread(0), m_state(INIT) {
		m_mutex = SDL_CreateMutex();
	}

	~ThreadBase() {
		Kill();
		SDL_DestroyMutex(m_mutex);
	}

	void Kill() { 
		if (GetState() == RUNNING)
			SDL_KillThread(m_thread);
	}

	State GetState() {
		SDL_mutexP(m_mutex);
		State s = m_state;
		SDL_mutexV(m_mutex);
		return s;
	}

	void SetState(State s) {
		SDL_mutexP(m_mutex);
		m_state = s;
		SDL_mutexV(m_mutex);
	}

	virtual void SignalCompleted() {}

	SDL_Thread *m_thread;

private:
	SDL_mutex *m_mutex;
	State m_state;
};

template <typename T>
class Thread : public ThreadBase {
public:
	Thread(void (*entry)(T*), T* data) : ThreadBase(), m_entry(entry), m_data(data) {
		m_thread = SDL_CreateThread(&Trampoline, this);
	}

	sigc::signal<void,T*> onCompleted;
	
private:
	static int Trampoline(void *vt) {
		Thread<T> *t = static_cast<Thread<T>*>(vt);
		t->SetState(ThreadBase::RUNNING);
		(*t->m_entry)(t->m_data);
		t->SetState(ThreadBase::COMPLETED);
		return 0;
	}

	virtual void SignalCompleted() {
		onCompleted.emit(m_data);
	}

	void (*m_entry)(T *data);
	T *m_data;
};

class ThreadManager {
public:
	ThreadManager() {}
	~ThreadManager();

	template <typename T> Thread<T> *StartThread(void (*entry)(T *data), T *data) {
		Thread<T> *t = new Thread<T>(entry, data);
		m_threads.push_back(t);
		return t;
	}

	void CleanupCompletedThreads(void);

private:
	std::list<ThreadBase*> m_threads;
};

#endif
