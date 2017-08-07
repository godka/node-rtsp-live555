#pragma once
#include "H264or5VideoStreamSink.hh"

class H264VideoStreamSink : public H264or5VideoStreamSink {
public:
	static H264VideoStreamSink* createNew(UsageEnvironment& env,
		live555responseHandler* handler = NULL,void* live555responseData = NULL,
		char const* sPropParameterSetsStr = NULL,
		// "sPropParameterSetsStr" is an optional 'SDP format' string
		// (comma-separated Base64-encoded) representing SPS and/or PPS NAL-units
		// to prepend to the output
		unsigned bufferSize = 300000);
	// See "FileSink.hh" for a description of these parameters.

protected:
	H264VideoStreamSink(UsageEnvironment& env,
		live555responseHandler* handle, void* live555responseData,
		char const* sPropParameterSetsStr,
		unsigned bufferSize);
	// called only by createNew()
	virtual ~H264VideoStreamSink();
};
