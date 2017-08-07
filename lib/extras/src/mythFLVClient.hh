#pragma once
#include "MythConfig.hh"
#define FLV_TAG_HEAD_LEN 11
#define FLV_PRE_TAG_LEN 4
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
class mythFLVClient
{
public:
	static mythFLVClient* CreateNew(){
		return new mythFLVClient();
	}
	unsigned long mythTickCount();
	int DataCallBack(unsigned char* data, int len, unsigned int timestamp);
	~mythFLVClient();
protected:
	mythFLVClient();
private:
	uint8_t * get_nal(uint32_t *len, uint8_t **offset, uint8_t *start, uint32_t total);
	int writespspps(uint8_t * sps, uint32_t spslen, uint8_t * pps, uint32_t ppslen, uint32_t timestamp);
	int writeavcframe(uint8_t * nal, uint32_t nal_len, uint32_t timestamp, bool IsIframe);
	char* _sps, *_pps;
	int _spslen; int _ppslen;
	unsigned int width, height, fps;
	long long timestart;
	bool _hassendIframe;
	inline uint32_t find_start_code(uint8_t *buf){
		auto nal = *(unsigned int*) &buf[0];
		if ((nal & 0x00ffffff) == 0x00010000){
			return 3;
		}
		else if (nal == 0x01000000){
			return 4;
		}
		else{
			return 0;
		}
	}
	int mythSendMessage(uint8_t * output, int output_len);
	unsigned long _basictick;
	bool isfirst;
};

