#include "OurRTSPClient.hh"

ourRTSPClient* ourRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL, const char* username, const char* password, bool usetcp,
	int verbosityLevel, char const* applicationName,void* handle,live555responseHandler* reponsehandler,void* live555responseData,portNumBits tunnelOverHTTPPortNum) {
	return new ourRTSPClient(env, rtspURL, username, password, usetcp,verbosityLevel,applicationName, handle,reponsehandler,live555responseData,tunnelOverHTTPPortNum);
}

ourRTSPClient::ourRTSPClient(UsageEnvironment& env, char const* rtspURL, const char* username, const char* password, bool usetcp,
	int verbosityLevel, char const* applicationName, void* handle, live555responseHandler* reponsehandler, void* live555responseData, portNumBits tunnelOverHTTPPortNum)
	: RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) {
	Authenticator* au = new Authenticator(username, password);
	scs.authenticator = au;
	scs.handle = handle;
	scs.responsehandler = reponsehandler;
	scs.responseData = live555responseData;
	scs.usetcp = usetcp;
	eventLoopWatchVariable = 0;
	Success = false;
}

ourRTSPClient::~ourRTSPClient() {
}
