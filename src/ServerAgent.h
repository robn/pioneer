// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef SERVERAGENT_H
#define SERVERAGENT_H

#include "libs.h"
#include <map>
#include <queue>
#include <json/json.h>
#include <curl/curl.h>

class ServerAgent {
public:
	ServerAgent(const std::string &baseUrl);
	virtual ~ServerAgent();

	typedef sigc::slot<void,const Json::Value &> SuccessCallback;
	typedef sigc::slot<void,const std::string &> FailCallback;

	void Call(const std::string &method, const Json::Value &data, SuccessCallback onSuccess = sigc::ptr_fun(&ServerAgent::IgnoreSuccessCallback), FailCallback onFail = sigc::ptr_fun(&ServerAgent::IgnoreFailCallback));

	void ProcessResponses();

private:

	struct Request {
		Request(const std::string &_method, const Json::Value &_data, SuccessCallback _onSuccess, FailCallback _onFail) :
			method(_method), data(_data), onSuccess(_onSuccess), onFail(_onFail) {}

		const std::string method;
		const Json::Value data;

		std::string buffer;

		SuccessCallback onSuccess;
		FailCallback onFail;
	};

	struct Response {
		Response(SuccessCallback _onSuccess, FailCallback _onFail) :
			onSuccess(_onSuccess), onFail(_onFail) {}

		bool success;

		std::string buffer;

		SuccessCallback onSuccess;
		Json::Value data;

		FailCallback onFail;
	};

	static int ThreadEntry(void *data);
	void ThreadMain();

	static size_t FillRequestBuffer(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t FillResponseBuffer(char *ptr, size_t size, size_t nmemb, void *userdata);

	static void IgnoreSuccessCallback(const Json::Value &data) {}
	static void IgnoreFailCallback(const std::string &error) {}

	static bool s_initialised;

	const std::string m_baseUrl;
	
	SDL_Thread *m_thread;

	CURL *m_curl;
	curl_slist *m_curlHeaders;

	std::queue<Request> m_requestQueue;
	SDL_mutex *m_requestQueueLock;
	SDL_cond *m_requestQueueCond;

	std::queue<Response> m_responseQueue;
	SDL_mutex *m_responseQueueLock;
};

#endif
