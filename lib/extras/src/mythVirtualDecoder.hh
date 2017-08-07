#pragma once
#include "MythConfig.hh"
#include <time.h>
#include <string>
#ifndef WIN32
#include <unistd.h>
#endif // !WIN32

class mythVirtualDecoder
{
public:
	typedef void (OnFrame)(char* data,int len,int timestamp,void* callback);
	static mythVirtualDecoder* CreateNew(void);
	void start();
	virtual unsigned int getSpeed();
	virtual void stop();
	virtual void ChangeURL(std::string url);
	void SetOnFrameCallback(OnFrame* callback, void* data);
	static int MainLoopstatic(void* data);
	virtual int MainLoop();
	virtual ~mythVirtualDecoder(void);
protected:
	mythVirtualDecoder(void);
	virtual void OnReconnect(int times);
	unsigned long mythTickCount();
	int mythDelay(unsigned long usec);
	int flag;
	OnFrame* OnFrameCallback;
	void* OnFrameData;
	int _times;
	unsigned long PresentationTime;
	unsigned long PresentationBytesLength;
	std::string _url;
};

