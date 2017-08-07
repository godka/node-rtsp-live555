#pragma once
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "StreamClientState.hh"
class ourRTSPClient : public RTSPClient {
public:
	typedef void (live555responseHandler)(void *myth, unsigned char* data, unsigned int length, unsigned int timestamp);
	static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL, const char* username, const char* password,
		bool usetcp = true,
		int verbosityLevel = 0,
		char const* applicationName = NULL, void* handle = NULL, live555responseHandler* reponsehandler = NULL, void* live555responseData= NULL,
		portNumBits tunnelOverHTTPPortNum = 0);
protected:
	ourRTSPClient(UsageEnvironment& env, char const* rtspURL, const char* username, const char* password, bool usetcp,
		int verbosityLevel, char const* applicationName, void* handle, live555responseHandler* reponsehandler, void* live555responseData, portNumBits tunnelOverHTTPPortNum);
	// called only by createNew();
	virtual ~ourRTSPClient();

public:
	StreamClientState scs;
	char eventLoopWatchVariable;
	bool TestUrl;
	bool Success;
};
