// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "ServerAgent.h"
#include "StringF.h"
#include <curl/curl.h>

bool ServerAgent::s_initialised = false;

ServerAgent::ServerAgent(const std::string &baseUrl) :
	m_baseUrl(baseUrl)
{
	if (!s_initialised)
		curl_global_init(CURL_GLOBAL_ALL);

	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);

	curl_easy_setopt(m_curl, CURLOPT_POST, 1);

	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, ServerAgent::FillRequestBuffer);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ServerAgent::FillResponseBuffer);

	m_curlHeaders = 0;
	m_curlHeaders = curl_slist_append(m_curlHeaders, "Content-type: application/json");
	curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_curlHeaders);

	m_requestQueueLock = SDL_CreateMutex();
	m_requestQueueCond = SDL_CreateCond();

	m_responseQueueLock = SDL_CreateMutex();

	m_thread = SDL_CreateThread(&ServerAgent::ThreadEntry, "ServerAgent", this);
}

ServerAgent::~ServerAgent()
{
	// flush the queue
	SDL_LockMutex(m_requestQueueLock);
	while (m_requestQueue.size() > 0)
		m_requestQueue.pop();
	SDL_UnlockMutex(m_requestQueueLock);

	// signal the thread. empty queue will cause it to exit
	SDL_CondBroadcast(m_requestQueueCond);

	// first thing the queue does is check the queue, so we must
	// not shut down until its done that. it will release the queue
	// lock before it dies, so try to take it
	SDL_LockMutex(m_requestQueueLock);

	// we have the lock, so we know the thread won't doing anything
	// else. we can clean up now
	SDL_DestroyMutex(m_responseQueueLock);
	SDL_DestroyMutex(m_requestQueueLock);
	SDL_DestroyCond(m_requestQueueCond);

	curl_slist_free_all(m_curlHeaders);

	curl_easy_cleanup(m_curl);
}

void ServerAgent::Call(const std::string &method, const Json::Value &data, SuccessCallback onSuccess, FailCallback onFail)
{
	SDL_LockMutex(m_requestQueueLock);
	m_requestQueue.push(Request(method, data, onSuccess, onFail));
	SDL_UnlockMutex(m_requestQueueLock);

	SDL_CondBroadcast(m_requestQueueCond);
}

void ServerAgent::ProcessResponses()
{
	std::queue<Response> responseQueue;

	// make a copy of the response queue so we can process
	// the response at our leisure
	SDL_LockMutex(m_responseQueueLock);
	responseQueue = m_responseQueue;
	while (m_responseQueue.size() > 0)
		m_responseQueue.pop();
	SDL_UnlockMutex(m_responseQueueLock);

	while (responseQueue.size() > 0) {
		Response &resp = responseQueue.front();

		if (resp.success)
			resp.onSuccess(resp.data);
		else
			resp.onFail(resp.buffer);

		responseQueue.pop();
	}
}

int ServerAgent::ThreadEntry(void *data)
{
	reinterpret_cast<ServerAgent*>(data)->ThreadMain();
	return 0;
}

void ServerAgent::ThreadMain()
{
	while (1) {

		// sleep until the main thread wakes us
		SDL_LockMutex(m_requestQueueLock);
		SDL_CondWait(m_requestQueueCond, m_requestQueueLock);

		// woken up but nothing on the queue means we're being destroyed
		if (m_requestQueue.size() == 0) {
			// main thread is waiting for this lock, and will start
			// cleanup as soon as it has it
			SDL_UnlockMutex(m_requestQueueLock);

			// might be cleaned up already, so don't do anything else,
			// just get out of here
			return;
		}

		// grab a request
		Request req = m_requestQueue.front();
		m_requestQueue.pop();

		// done with the queue
		SDL_UnlockMutex(m_requestQueueLock);

		Json::FastWriter writer;
		req.buffer = writer.write(req.data);

		Response resp(req.onSuccess, req.onFail);

		curl_easy_setopt(m_curl, CURLOPT_URL, std::string(m_baseUrl+"/"+req.method).c_str());
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, req.buffer.size());
		curl_easy_setopt(m_curl, CURLOPT_READDATA, &req);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &resp);

		CURLcode rc = curl_easy_perform(m_curl);
		resp.success = rc == CURLE_OK;
		if (!resp.success)
			resp.buffer = std::string("call failed: " + std::string(curl_easy_strerror(rc)));

		if (resp.success) {
			long code;
			curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &code);
			if (code != 200) {
				resp.success = false;
				resp.buffer = stringf("call returned HTTP status: %0{d}", code);
			}
		}

		if (resp.success) {
			Json::Reader reader;
			resp.success = reader.parse(resp.buffer, resp.data, false);
			if (!resp.success)
				resp.buffer = std::string("JSON parse error: " + reader.getFormattedErrorMessages());
		}

		SDL_LockMutex(m_responseQueueLock);
		m_responseQueue.push(resp);
		SDL_UnlockMutex(m_responseQueueLock);
	}
}

size_t ServerAgent::FillRequestBuffer(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	ServerAgent::Request *req = reinterpret_cast<ServerAgent::Request*>(userdata);
	size_t amount = std::max(size*nmemb, req->buffer.size());
	memcpy(ptr, req->buffer.data(), amount);
	req->buffer.erase(0, amount);
	return amount;
}

size_t ServerAgent::FillResponseBuffer(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	ServerAgent::Response *resp = reinterpret_cast<ServerAgent::Response*>(userdata);
	resp->buffer.append(ptr, size*nmemb);
	return size*nmemb;
}
