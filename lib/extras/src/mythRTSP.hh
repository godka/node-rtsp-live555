#pragma once
#include "liveMedia.hh"
#include "mythUseageEnvironment.hh"
//#include "DummySink.hh"
#include "StreamClientState.hh"
#include "OurRTSPClient.hh"
class mythRTSP
{
public:
	static mythRTSP* CreateNew(){
		return new mythRTSP();
	}
	int Start(RTSPClient* rtspclient);
	int Stop(RTSPClient* rtspClient);
	typedef void (live555responseHandler)(void *handle, unsigned char* data, unsigned int length, unsigned int timestamp);
	// RTSP 'response handlers':
	static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
	static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

	// Other event handler functions:
	static void subsessionAfterPlaying(void* clientData); // called when a stream's subsession (e.g., audio or video substream) ends
	static void subsessionByeHandler(void* clientData); // called when a RTCP "BYE" is received for a subsession
	static void streamTimerHandler(void* clientData);
	// called at the end of a stream's expected duration (if the stream has not already signaled its end using a RTCP "BYE")


	// Used to iterate through each stream's 'subsessions', setting up each one:
	static void setupNextSubsession(RTSPClient* rtspClient);

	RTSPClient* openURL(char const* rtspURL, char const *username, char const *password, bool usetcp, live555responseHandler* responsehandler = NULL, void* live555responseData = NULL);
	// Used to shut down and close a stream (including its "RTSPClient" object):
	static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

	unsigned rtspClientCount; // Counts how many streams (i.e., "RTSPClient"s) are currently in use.
	~mythRTSP();
protected:
	// The main streaming routine (for each "rtsp://" URL):
	mythRTSP();
private:
	TaskScheduler* scheduler;
	UsageEnvironment* env;
	char eventLoopWatchVariable;
};

