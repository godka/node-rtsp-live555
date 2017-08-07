#include "StreamSink.hh"

////////// FileSink //////////

StreamSink::StreamSink(UsageEnvironment& env, unsigned bufferSize)
	: MediaSink(env), fBufferSize(bufferSize), fSamePresentationTimeCounter(0){
	fBuffer = new unsigned char[bufferSize];
	fTotalBufferSize = bufferSize * 8;		//for only one frame is fucking enough
	fTotalBuffer = new unsigned char[fTotalBufferSize];
	fBufferIndex = 0;
	fPrevPresentationTime.tv_sec = ~0; fPrevPresentationTime.tv_usec = 0;
	fTimeStamp = ~0;
}

StreamSink::~StreamSink() {
	delete [] fBuffer;
	delete [] fTotalBuffer;
}

StreamSink* StreamSink::createNew(UsageEnvironment& env,unsigned bufferSize) {

	return NULL;
}

Boolean StreamSink::continuePlaying() {
	if (fSource == NULL) return False;

	fSource->getNextFrame(fBuffer, fBufferSize,
		afterGettingFrame, this,
		onSourceClosure, this);

	return True;
}

void StreamSink::afterGettingFrame(void* clientData, unsigned frameSize,
	unsigned numTruncatedBytes,
struct timeval presentationTime,
	unsigned /*durationInMicroseconds*/) {
	StreamSink* sink = (StreamSink*) clientData;
	sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime);
}

void StreamSink::addData(unsigned char const* data, unsigned dataSize,
struct timeval presentationTime) {
//hey man
	memcpy(fTotalBuffer + fBufferIndex, data, dataSize);
	fBufferIndex += dataSize;
}

void StreamSink::afterGettingFrame(unsigned frameSize,
	unsigned numTruncatedBytes, struct timeval presentationTime) {

	if (numTruncatedBytes > 0) {
		envir() << "FileSink::afterGettingFrame(): The input frame data was too large for our buffer size ("
			<< fBufferSize << ").  "
			<< numTruncatedBytes << " bytes of trailing data was dropped!  Correct this by increasing the \"bufferSize\" parameter in the \"createNew()\" call to at least "
			<< fBufferSize + numTruncatedBytes << "\n";
	}
	addData(fBuffer, frameSize, presentationTime);
	// Then try getting the next frame:
	unsigned long TimeStamp = presentationTime.tv_sec * 1000 + presentationTime.tv_usec / 1000;
	if (fTimeStamp == ~0){
		fTimeStamp = presentationTime.tv_sec * 1000 + presentationTime.tv_usec / 1000;
	}
	if (live555response){
		live555response(this->phwnd, (unsigned char*) fTotalBuffer, fBufferIndex, TimeStamp - fTimeStamp);
	}
	//fTimeStamp = TimeStamp;
	fBufferIndex = 0;
	continuePlaying();
}
