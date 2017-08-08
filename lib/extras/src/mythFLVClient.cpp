#include "mythFLVClient.hh"
#include <stdio.h>
#ifdef WIN32
#include <Windows.h>
#include <io.h>
#else
#include <time.h>
#endif
#include <stdlib.h>
#include <memory.h>
#define BUFSIZE 4096
unsigned long mythFLVClient::mythTickCount(){
#ifdef WIN32
	return GetTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}
uint8_t * mythFLVClient::get_nal(uint32_t *len, uint8_t **offset, uint8_t *start, uint32_t total)
{
	uint32_t info;
	uint8_t *q;
	uint8_t *p = *offset;
	*len = 0;

	while (1) {
		info = find_start_code(p);
		if (info > 0)
			break;
		p++;
		if ((p - start) >= total)
			return NULL;
	}
	q = p + info;
	p = q;
	while (1) {
		info = find_start_code(p);
		if (info > 0)
			break;
		p++;
		if ((p - start) >= total){
			//maybe cause error
			break;
		}
	}

	*len = (p - q);
	*offset = p;
	return q;
}
int mythFLVClient::DataCallBack(unsigned char* data, int len, unsigned int timestamp)
{
	if (_basictick == ~0){
		_basictick = mythTickCount();
	}
	if (timestamp == ~0){
		timestamp = mythTickCount() - _basictick;
	}
	uint8_t *buf_offset = data;
	uint32_t nal_len;
	uint8_t *nal;
	if (isfirst){
		uint8_t flv_header[13] = { 0x46, 0x4c, 0x56, 0x01, 0x01, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00 };
		if (mythSendMessage(flv_header, 13) < 0){
			return -1;
		}
		isfirst = false;
	}
	while (1) {
		nal = get_nal(&nal_len, &buf_offset, (uint8_t*) data, len);
		if (nal_len <= 3) break;
		int nalu = nal[0] & 0x1f;
		switch (nalu)
		{
		case 7:
			if (_spslen == 0){
				_sps = new char[nal_len];
				memcpy(_sps, nal, nal_len);
				_spslen = nal_len;
			}
			break;
		case 8:
			if (_ppslen == 0){
				_pps = new char[nal_len];
				memcpy(_pps, nal, nal_len);
				_ppslen = nal_len;
			}
			//if (_spslen > 0 && _ppslen > 0){
			//	if (writespspps((uint8_t*) _sps, _spslen, (uint8_t*) _pps, _ppslen, timestamp) < 0)
			//		return -1;
			//}
			break;
		case 5:
			if (!_hassendIframe){
				if (_spslen > 0 && _ppslen > 0){
					if (writespspps((uint8_t*) _sps, _spslen, (uint8_t*) _pps, _ppslen, timestamp) < 0)
						return -1;
				}
			}
			if (writeavcframe(nal, nal_len, timestamp, true) < 0)
				return -1;
			_hassendIframe = true;
			break;
		default:
			if (_hassendIframe){
				if (writeavcframe(nal, nal_len, timestamp, false) < 0)
					return -1;
			}
			break;
		}
	}
	return 0;
}

mythFLVClient::~mythFLVClient()
{
	if (_sps){
		delete [] _sps;
	}
	if (_pps){
		delete [] _pps;
	}
}


mythFLVClient::mythFLVClient()
{
	#ifdef WIN32
		_setmode(_fileno(stdout), 0x8000);
	#endif
	_hassendIframe = false;
	_sps = nullptr; _pps = nullptr;
	_spslen = 0; _ppslen = 0;
	width = 0; height = 0; fps = 0;
	_basictick = ~0;
	isfirst = true;
}

int mythFLVClient::writespspps(uint8_t * sps, uint32_t spslen, uint8_t * pps, uint32_t ppslen, uint32_t timestamp)
{
	uint32_t body_len = spslen + ppslen + 16;
	uint32_t output_len = body_len + FLV_TAG_HEAD_LEN + FLV_PRE_TAG_LEN;
	uint8_t * output = (uint8_t*) malloc(output_len);
	uint32_t offset = 0;
	// flv tag header
	output[offset++] = 0x09; //tagtype video
	output[offset++] = (uint8_t) (body_len >> 16); //data len
	output[offset++] = (uint8_t) (body_len >> 8); //data len
	output[offset++] = (uint8_t) (body_len); //data len
	output[offset++] = (uint8_t) (timestamp >> 16); //time stamp
	output[offset++] = (uint8_t) (timestamp >> 8); //time stamp
	output[offset++] = (uint8_t) (timestamp); //time stamp
	output[offset++] = (uint8_t) (timestamp >> 24); //time stamp
	output[offset++] = 0x00; //stream id 0
	output[offset++] = 0x00; //stream id 0
	output[offset++] = 0x00; //stream id 0

	//flv VideoTagHeader
	output[offset++] = 0x17; //key frame, AVC
	output[offset++] = 0x00; //avc sequence header
	output[offset++] = 0x00; //composit time ??????????
	output[offset++] = 0x00; // composit time
	output[offset++] = 0x00; //composit time

	//flv VideoTagBody --AVCDecoderCOnfigurationRecord
	output[offset++] = 0x01; //configurationversion
	output[offset++] = sps[1]; //avcprofileindication
	output[offset++] = sps[2]; //profilecompatibilty
	output[offset++] = sps[3]; //avclevelindication
	output[offset++] = 0xff; //reserved + lengthsizeminusone
	output[offset++] = 0xe1; //numofsequenceset
	output[offset++] = (uint8_t) (spslen >> 8); //sequence parameter set length high 8 bits
	output[offset++] = (uint8_t) (spslen); //sequence parameter set  length low 8 bits
	memcpy(output + offset, sps, spslen); //H264 sequence parameter set
	offset += spslen;
	output[offset++] = 0x01; //numofpictureset
	output[offset++] = (uint8_t) (ppslen >> 8); //picture parameter set length high 8 bits
	output[offset++] = (uint8_t) (ppslen); //picture parameter set length low 8 bits
	memcpy(output + offset, pps, ppslen); //H264 picture parameter set

	//no need set pre_tag_size ,RTMP NO NEED
	// flv test 
	offset += ppslen;
	uint32_t fff = body_len + FLV_TAG_HEAD_LEN;
	output[offset++] = (uint8_t) (fff >> 24); //data len
	output[offset++] = (uint8_t) (fff >> 16); //data len
	output[offset++] = (uint8_t) (fff >> 8); //data len
	output[offset++] = (uint8_t) (fff); //data len
	if (mythSendMessage(output, output_len) < 0){
		//RTMP Send out
		free(output);
		return -1;
	}
	else{
		free(output);
	}
	return 0;
}

int mythFLVClient::writeavcframe(uint8_t * nal, uint32_t nal_len, uint32_t timestamp, bool IsIframe)
{
	uint32_t body_len = nal_len + 5 + 4; //flv VideoTagHeader +  NALU length
	uint32_t output_len = body_len + FLV_TAG_HEAD_LEN + FLV_PRE_TAG_LEN;
	uint8_t * output = (uint8_t*) malloc(output_len);
	uint32_t offset = 0;
	// flv tag header
	output[offset++] = 0x09; //tagtype video
	output[offset++] = (uint8_t) (body_len >> 16); //data len
	output[offset++] = (uint8_t) (body_len >> 8); //data len
	output[offset++] = (uint8_t) (body_len); //data len
	output[offset++] = (uint8_t) (timestamp >> 16); //time stamp
	output[offset++] = (uint8_t) (timestamp >> 8); //time stamp
	output[offset++] = (uint8_t) (timestamp); //time stamp
	output[offset++] = (uint8_t) (timestamp >> 24); //time stamp
	output[offset++] = 0x00; //stream id 0
	output[offset++] = 0x00; //stream id 0
	output[offset++] = 0x00; //stream id 0

	//flv VideoTagHeader
	if (IsIframe)
		output[offset++] = 0x17; //key frame, AVC
	else
		output[offset++] = 0x27; //key frame, AVC

	output[offset++] = 0x01; //avc NALU unit
	output[offset++] = 0x00; //composit time ??????????
	output[offset++] = 0x00; // composit time
	output[offset++] = 0x00; //composit time

	output[offset++] = (uint8_t) (nal_len >> 24); //nal length 
	output[offset++] = (uint8_t) (nal_len >> 16); //nal length 
	output[offset++] = (uint8_t) (nal_len >> 8); //nal length 
	output[offset++] = (uint8_t) (nal_len); //nal length 
	memcpy(output + offset, nal, nal_len);

	//no need set pre_tag_size ,RTMP NO NEED
	offset += nal_len;
	uint32_t fff = body_len + FLV_TAG_HEAD_LEN;
	output[offset++] = (uint8_t) (fff >> 24); //data len
	output[offset++] = (uint8_t) (fff >> 16); //data len
	output[offset++] = (uint8_t) (fff >> 8); //data len
	output[offset++] = (uint8_t) (fff); //data len
	//fwrite(output, output_len, 1, flv_file);
	if (mythSendMessage(output, output_len) < 0){
		free(output);
		return -1;
	}
	else{
		free(output);
		return 0;
	}
}
int mythFLVClient::mythSendMessage(uint8_t * output, int output_len)
{
#ifdef _DEBUG
	int _index = 0;
	int _ret = 0;
	do{
		if (output_len < BUFSIZE){
			fwrite(output + _index, 1, output_len, stdout);
			break;
		}
		else{
			_ret = fwrite(output + _index, 1, BUFSIZE, stdout);
			_index += _ret;
			output_len -= _ret;
		}
	} while (1);
#else
	fwrite(output, 1, output_len, stdout);
#endif
	return 0;
}
