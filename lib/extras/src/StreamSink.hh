#pragma once
#ifndef _MEDIA_SINK_HH
#include "MediaSink.hh"
#endif
class StreamSink : public MediaSink {
public:
	typedef void (live555responseHandler)(void *myth, unsigned char* data, unsigned int length,unsigned int timestamp);
	live555responseHandler* live555response;
	void* phwnd;
	static StreamSink* createNew(UsageEnvironment& env,unsigned bufferSize = 20000);
	// "bufferSize" should be at least as large as the largest expected
	//   input frame.
	// "oneFilePerFrame" - if True - specifies that each input frame will
	//   be written to a separate file (using the presentation time as a
	//   file name suffix).  The default behavior ("oneFilePerFrame" == False)
	//   is to output all incoming data into a single file.

	virtual void addData(unsigned char const* data, unsigned dataSize,
	struct timeval presentationTime);
	// (Available in case a client wants to add extra data to the output file)

protected:
	StreamSink(UsageEnvironment& env, unsigned bufferSize);
	// called only by createNew()
	virtual ~StreamSink();

protected: // redefined virtual functions:
	virtual Boolean continuePlaying();

protected:
	static void afterGettingFrame(void* clientData, unsigned frameSize,
		unsigned numTruncatedBytes,
	struct timeval presentationTime,
		unsigned durationInMicroseconds);
	virtual void afterGettingFrame(unsigned frameSize,
		unsigned numTruncatedBytes,
	struct timeval presentationTime);

	unsigned fBufferIndex;
	unsigned char* fBuffer;
	unsigned fBufferSize;
	unsigned char* fTotalBuffer;
	unsigned fTotalBufferSize;
	struct timeval fPrevPresentationTime;
	unsigned fSamePresentationTimeCounter;
private:
	unsigned long fTimeStamp;
};

