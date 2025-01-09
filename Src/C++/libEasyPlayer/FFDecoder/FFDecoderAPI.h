#ifndef __FF_DECODER_API_H__
#define __FF_DECODER_API_H__

#ifdef _DEBUG
//#include <vld.h>
#endif

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FFDECODER_API  __declspec(dllexport)
//=======================================================
//Decoder
#ifndef DECODER_H264
#define DECODER_H264			0x1C			//28
#endif
#ifndef DECODER_H265
#define DECODER_H265			0xAE			//174
#endif
#ifndef DECODER_MPEG4
#define DECODER_MPEG4			0x0D			//13
#endif
#ifndef DECODER_MPEG2
#define DECODER_MPEG2			0x02			//2
#endif
#ifndef DECODER_MJPEG
#define DECODER_MJPEG			0x08			//8
#endif

#ifndef DECODER_MP3
#define DECODER_MP3				0x15001			//86017
#endif

#ifndef DECODER_AAC
#define DECODER_AAC				0x15002			//86018
#endif

//=======================================================
//�����ʽ
#ifndef OUTPUT_PIX_FMT_YUV420P
#define OUTPUT_PIX_FMT_YUV420P		0
#endif
#ifndef OUTPUT_PIX_FMT_YUYV422
#define OUTPUT_PIX_FMT_YUYV422		1
#endif
#ifndef OUTPUT_PIX_FMT_RGB565LE
#define OUTPUT_PIX_FMT_RGB565LE		44
#endif
#ifndef OUTPUT_PIX_FMT_RGBA
#define OUTPUT_PIX_FMT_RGBA			28
#endif

//=======================================================
//ͼ����
//=======================================================
typedef enum __VIDEO_FILTER_TYPE
{
	VIDEO_ROTATION_90_0				=		0,	//˳ʱ����ת90��
	VIDEO_ROTATION_90_1,						//��ʱ����ת90��
	VIDEO_ROTATION_90_0_FLIP,					//˳ʱ����ת90��,��ˮƽ��ת
	VIDEO_ROTATION_90_1_FLIP,					//��ʱ����ת90��,�ٴ�ֱ��ת

	VIDEO_TEXT,

}VIDEO_FILTER_TYPE;




//=======================================================
typedef void *FFD_HANDLE;


//=======================================================
extern "C"
{
	int	FFDECODER_API	FFD_Init(FFD_HANDLE *_handle);
	int	FFDECODER_API	FFD_Deinit(FFD_HANDLE *_handle);

	int FFDECODER_API	FFD_SetVideoDecoderParam(FFD_HANDLE _handle, int _width, int _height, int _decoder, int _outformat, int multiThread);
	int FFDECODER_API	FFD_SetAudioDecoderParam(FFD_HANDLE _handle, unsigned char _channel, unsigned int _sample_rate, unsigned int bitsPerSample, unsigned int _decoder);

	int	FFDECODER_API	FFD_GetVideoDecoderInfo(FFD_HANDLE _handle, int *_decoder, int *_width, int *_height);

	//function name:	FFD_Decode
	//parameters:
		//Input:
	int	FFDECODER_API	FFD_DecodeVideo(FFD_HANDLE _handle, char *pInBuf, int inputSize, char **pOutBuf, int dstW, int dstH);

	//function name:	FFD_Decode2Buf
	//desc:				���������ݣ�ֱ���͵�ָ�����ڴ���
	int	FFDECODER_API	FFD_DecodeVideo2Buf(FFD_HANDLE _handle, char *_inbuf, int _bufsize, void *_outbuf[8], int _pitch);

	int	FFDECODER_API	FFD_DecodeVideo3(FFD_HANDLE _handle, char *_inbuf, int _bufsize, void *yuvbuf, int dstW, int dstH);
	int	FFDECODER_API	FFD_ConvertDecodeFrameFormat(FFD_HANDLE _handle, char *dstData, int dstFormat, int dstW, int dstH);		//���FFD_DecodeVideo3ʹ��,���ս������֡ת����ָ����ʽ
	int	FFDECODER_API	FFD_ConvertDecodeFrameFormat2(FFD_HANDLE _handle, char *dstData, int dstFormat, char *srcData, int srcFormat, int dstW, int dstH);

	int FFDECODER_API	FFD_DecodeVideoPacket(FFD_HANDLE _handle, char *pCodecCtx, unsigned char *avPacket, char **_outbuf);


	int	FFDECODER_API	FFD_DecodeAudio(FFD_HANDLE _handle, char *pInBuf, int inputSize, char *pOutBuf, int *outSize);
	int	FFDECODER_API	FFD_DecodeAudioPacket(FFD_HANDLE _handle, char *pCodecCtx, unsigned char *avPacket, char *pOutBuf, int *outSize);
};




#endif