#pragma once
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
class StreamClientState {
public:
	typedef void (live555responseHandler)(void *myth, unsigned char* data, unsigned int length, unsigned int timestamp);
	StreamClientState();
	virtual ~StreamClientState();

public:
	bool usetcp;
	Authenticator* authenticator;	//add
	MediaSubsessionIterator* iter;
	MediaSession* session;
	MediaSubsession* subsession;
	TaskToken streamTimerTask;
	double duration;
	void* handle;
	live555responseHandler* responsehandler;
	void* responseData;
};
