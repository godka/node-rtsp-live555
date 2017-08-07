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
#include "mythLive555Decoder.hh"

mythLive555Decoder::mythLive555Decoder(char* rtsplink, bool usetcp)
	:mythVirtualDecoder()
{
	if (rtsplink)
		_url = rtsplink;
	flag = 0;
	isrunning = false;
	fusetcp = usetcp;
}

void mythLive555Decoder::OnReconnect(int times)
{
	fusetcp = !fusetcp;
	mythDelay(1000);
#ifdef _DEBUG
	//printf("Ready to reconnect %d\n",times);
#endif
}

int mythLive555Decoder::ParseRTSPlink(char* src, char* rtsplink, char* username, char* password)
{
	int len = strlen(src);
	//find @
	int i = 0,j = 0;
	for (i = 0; i < len; i++){
		if (src[i] == '@'){
			break;
		}
	}
	if (i == len){
		strcpy(rtsplink, src);
		username = "";
		password = "";
		return 0;
	}
	else{
		rtsplink[0] = 'r';rtsplink[1] = 't';rtsplink[2] = 's';rtsplink[3] = 'p';
		rtsplink[4] = ':';rtsplink[5] = '/';rtsplink[6] = '/';
		strcpy(rtsplink + 7, src + i + 1);
	}
	for (j = 7; j < len; j++){
		if (src[j] == ':'){
			break;
		}
	}
	if (j == len){
		strcpy(rtsplink, src);
		username = "";
		password = "";
	}
	else{
		if (i > j){
			memcpy(username, &src[7], j - 7);
			memcpy(password, &src[j + 1], i - j - 1);
		}
	}
	return 0;
}

mythLive555Decoder* mythLive555Decoder::CreateNew(char* rtsplink, char* username,char* password,bool usetcp /*= true*/)
{
	int len = strlen(rtsplink); 
	if (len > 7){
		if (username == NULL && password == NULL){
			return new mythLive555Decoder(rtsplink, usetcp);
		}
		else{
			char tmplink[256] = { 0 };
			strcpy(tmplink, rtsplink + 7);
			std::string _username = username;
			std::string _password = password;
			std::string retlink = "rtsp://" + _username + ":" + _password + "@" + tmplink;
			return new mythLive555Decoder((char*)retlink.c_str(), usetcp);
		}
	}
	else{
		return NULL;
	}
}

mythLive555Decoder* mythLive555Decoder::CreateNew(char* rtsplink, bool usetcp /*= true*/)
{
	//parse rtsp url start
	//auto rtsp link : rtsp://user:pass@link
	return new mythLive555Decoder(rtsplink,usetcp);
}
void mythLive555Decoder::callbackdatastatic(void *myth, unsigned char* data, unsigned int length, unsigned int timestamp){
	mythLive555Decoder* mythlive555 = (mythLive555Decoder*)myth;
	mythlive555->callbackdata(data,length,timestamp);
}
void mythLive555Decoder::callbackdata(unsigned char* data, unsigned int length, unsigned int timestamp)
{
	if (PresentationTime == ~0){
		PresentationTime = mythTickCount();
	}
	PresentationBytesLength += length;
	OnFrameCallback((char*)data, length, timestamp, OnFrameData);
}
int mythLive555Decoder::MainLoop(){
	isrunning = true;
	while (isrunning){
		rtsp = mythRTSP::CreateNew();
		if (rtsp){
			char tmplink[256] = { 0 };
			char tmpusername[32] = { 0 };
			char tmppassword[32] = { 0 };
			ParseRTSPlink((char*)_url.c_str(), tmplink, tmpusername, tmppassword);
			client = rtsp->openURL(tmplink, tmpusername, tmppassword, fusetcp, mythLive555Decoder::callbackdatastatic, (void*)this);
			if (client){
				rtsp->Start(client);
			}
		}
		if (isrunning){
			_times++;
			OnReconnect(_times);
		}
	}
	//printf("Connection closed");
	delete rtsp;
	rtsp = NULL;
	return 0;
}
mythLive555Decoder::~mythLive555Decoder(void)
{

}

void mythLive555Decoder::stop()
{
	isrunning = false;
	if (rtsp && client)
		rtsp->Stop(client);
	//ugly code to wait mainloop to stop
	while (rtsp)
		mythDelay(1);
	//delete rtsp;
}
