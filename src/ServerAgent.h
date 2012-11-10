// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef SERVERAGENT_H
#define SERVERAGENT_H

#include "libs.h"
#include <map>
#include <queue>
#include <json/json.h>

class ServerAgent {
public:
	ServerAgent(const std::string &baseUrl);
	virtual ~ServerAgent();

	typedef std::map<std::string,std::string> DataMap;
	typedef sigc::slot<void,const Json::Value &> SuccessCallback;
	typedef sigc::slot<void> FailCallback;

	void Call(const std::string &method, const DataMap &data, SuccessCallback onSuccess = sigc::ptr_fun(&ServerAgent::IgnoreSuccessCallback), FailCallback onFail = sigc::ptr_fun(&ServerAgent::IgnoreFailCallback));

	void ProcessResponses();

private:

	struct Request {
		Request(const std::string &_method, const DataMap &_data, SuccessCallback _onSuccess, FailCallback _onFail) :
			method(_method), data(_data), onSuccess(_onSuccess), onFail(_onFail) {}
		const std::string method;
		const DataMap data;
		SuccessCallback onSuccess;
		FailCallback onFail;
	};

	struct Response {
		Response(SuccessCallback _onSuccess, FailCallback _onFail) :
			onSuccess(_onSuccess), onFail(_onFail) {}
		bool success;
		SuccessCallback onSuccess;
		FailCallback onFail;
		std::string contentBuffer;
		Json::Value data;
	};

	static int ThreadEntry(void *data);
	void ThreadMain();

	static size_t FillResponseBuffer(char *ptr, size_t size, size_t nmemb, void *userdata);

	static void IgnoreSuccessCallback(const Json::Value &data) {}
	static void IgnoreFailCallback() {}

	static bool s_initialised;

	const std::string m_baseUrl;
	
	SDL_Thread *m_thread;

	std::queue<Request> m_requestQueue;
	SDL_mutex *m_requestQueueLock;
	SDL_cond *m_requestQueueCond;

	std::queue<Response> m_responseQueue;
	SDL_mutex *m_responseQueueLock;
};

#endif
