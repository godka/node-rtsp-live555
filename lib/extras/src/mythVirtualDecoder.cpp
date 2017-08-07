/********************************************************************
Created by MythKAst
©2013 MythKAst Some rights reserved.


You can build it with vc2010,gcc.
Anybody who gets this source code is able to modify or rebuild it anyway,
but please keep this section when you want to spread a new version.
It's strongly not recommended to change the original copyright. Adding new version
information, however, is Allowed. Thanks.
For the latest version, please be sure to check my website:
Http://code.google.com/mythkast


你可以用vc2010,gcc编译这些代码
任何得到此代码的人都可以修改或者重新编译这段代码，但是请保留这段文字。
请不要修改原始版权，但是可以添加新的版本信息。
最新版本请留意：Http://code.google.com/mythkast
B
MythKAst(asdic182@sina.com), in 2013 June.
*********************************************************************/
#include "mythVirtualDecoder.hh"
#ifdef WIN32
#include <Windows.h>
#endif


mythVirtualDecoder::mythVirtualDecoder(void){
	flag = 0;
	OnFrameCallback = NULL;
	OnFrameData = NULL;
	_times = 0;
	PresentationTime = ~0;
	PresentationBytesLength = 0;
}

void mythVirtualDecoder::ChangeURL(std::string url){
	_url = url;
}

unsigned long mythVirtualDecoder::mythTickCount(){
#ifdef WIN32
	return GetTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

int mythVirtualDecoder::mythDelay(unsigned long usec)
{
#ifdef WIN32
	Sleep((DWORD) (usec));
#else
	usleep(usec * 1000);
#endif
	return 0;
}
void mythVirtualDecoder::start(){
	MainLoopstatic(this);
}

unsigned int mythVirtualDecoder::getSpeed()
{
	unsigned long timenow = mythTickCount();
	unsigned int ret = 0;
	if (timenow - PresentationTime != 0){
		ret = PresentationBytesLength / (timenow - PresentationTime);
	}
	PresentationBytesLength = 0;
	PresentationTime = timenow;
	return ret;
}

mythVirtualDecoder* mythVirtualDecoder::CreateNew(void){
	return new mythVirtualDecoder();
}
mythVirtualDecoder::~mythVirtualDecoder(void){
}
int mythVirtualDecoder::MainLoopstatic(void* data)
{
	mythVirtualDecoder* decoder = (mythVirtualDecoder*) data;
	if (decoder){
		return decoder->MainLoop();
	}
	return 0;
}

void mythVirtualDecoder::OnReconnect(int times)
{

}

void mythVirtualDecoder::stop()
{

}

void mythVirtualDecoder::SetOnFrameCallback(OnFrame* callback, void* data)
{
	OnFrameCallback = callback;
	OnFrameData = data;
}

int mythVirtualDecoder::MainLoop()
{
	return 0;
}
