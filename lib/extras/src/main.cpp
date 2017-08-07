#include "MythConfig.hh"
#include "mythLive555Decoder.hh"
#include "mythFLVClient.hh"
mythFLVClient* client = NULL;
void oncallback(char* data, int len, int timestamp, void* callback){
	client->DataCallBack((unsigned char*)data, len, timestamp);
}
int main(int argc, char* argv[])
{
	if (argc > 1){
		client = mythFLVClient::CreateNew();
		mythVirtualDecoder* decoder = mythLive555Decoder::CreateNew(argv[1]);
		decoder->SetOnFrameCallback(oncallback, NULL);
		decoder->start();
	}
	return 0;
}

