// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef RPCAGENT_H
#define RPCAGENT_H

#include "libs.h"
#include <map>
#include <queue>
#include <json/json.h>
#include <curl/curl.h>

class RPCAgent {
public:
	virtual ~RPCAgent() {}

	typedef sigc::slot<void,const Json::Value &,void *> SuccessCallback;
	typedef sigc::slot<void,const std::string &,void *> FailCallback;

	virtual void Call(const std::string &method, const Json::Value &data, SuccessCallback onSuccess, FailCallback onFail, void *userdata) = 0;

	virtual void ProcessResponses() = 0;

protected:
	static void IgnoreSuccessCallback(const Json::Value &data) {}
	static void IgnoreFailCallback(const std::string &error) {}
};


class NullRPCAgent : public RPCAgent {
public:
	virtual void Call(
		const std::string &method, const Json::Value &data,
		RPCAgent::SuccessCallback onSuccess = sigc::ptr_fun(&RPCAgent::IgnoreSuccessCallback),
		RPCAgent::FailCallback onFail = sigc::ptr_fun(&RPCAgent::IgnoreFailCallback),
		void *userdata = 0);

	virtual void ProcessResponses();

private:

	struct Response {
		Response(FailCallback _onFail, void *_userdata) :
			onFail(_onFail), userdata(_userdata)
			{}

		FailCallback onFail;
		void *userdata;
	};

	std::queue<Response> m_queue;
};


class HTTPRPCAgent : public RPCAgent {
public:
	HTTPRPCAgent(const std::string &baseUrl);
	virtual ~HTTPRPCAgent();

	virtual void Call(
		const std::string &method, const Json::Value &data,
		SuccessCallback onSuccess = sigc::ptr_fun(&RPCAgent::IgnoreSuccessCallback),
		FailCallback onFail = sigc::ptr_fun(&RPCAgent::IgnoreFailCallback),
		void *userdata = 0);

	virtual void ProcessResponses();

private:

	struct Request {
		Request(const std::string &_method, const Json::Value &_data, SuccessCallback _onSuccess, FailCallback _onFail, void *_userdata) :
			method(_method), data(_data), onSuccess(_onSuccess), onFail(_onFail), userdata(_userdata) {}

		const std::string method;
		const Json::Value data;

		std::string buffer;

		SuccessCallback onSuccess;
		FailCallback onFail;

		void *userdata;
	};

	struct Response {
		Response(SuccessCallback _onSuccess, FailCallback _onFail, void *_userdata) :
			onSuccess(_onSuccess), onFail(_onFail), userdata(_userdata) {}

		bool success;

		std::string buffer;

		SuccessCallback onSuccess;
		Json::Value data;

		FailCallback onFail;

		void *userdata;
	};

	static int ThreadEntry(void *data);
	void ThreadMain();

	static const std::string &UserAgent();

	static size_t FillRequestBuffer(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t FillResponseBuffer(char *ptr, size_t size, size_t nmemb, void *userdata);

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
