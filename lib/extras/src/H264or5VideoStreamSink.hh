#pragma once
#include "StreamSink.hh"
class H264or5VideoStreamSink : public StreamSink {
protected:
	H264or5VideoStreamSink(UsageEnvironment& env, 
		unsigned bufferSize, live555responseHandler* handler, void* live555responseData,
		char const* sPropParameterSetsStr1,
		char const* sPropParameterSetsStr2 = NULL,
		char const* sPropParameterSetsStr3 = NULL);
	// we're an abstract base class
	virtual ~H264or5VideoStreamSink();

protected: // redefined virtual functions:
	virtual void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime);

private:
	char const* fSPropParameterSetsStr[3];
	Boolean fHaveWrittenFirstFrame;
};

