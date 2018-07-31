/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Sword@easydarwin.org
*/

#ifndef __LIB_EASY_PLAYER_API_H__
#define __LIB_EASY_PLAYER_API_H__

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EasyRTSPClient\EasyRTSPClientAPI.h"

#define LIB_EASYPLAYER_API __declspec(dllexport)

typedef enum __RENDER_FORMAT
{
	DISPLAY_FORMAT_YV12		=		842094169,
	DISPLAY_FORMAT_YUY2		=		844715353,
	DISPLAY_FORMAT_UYVY		=		1498831189,
	DISPLAY_FORMAT_A8R8G8B8	=		21,
	DISPLAY_FORMAT_X8R8G8B8	=		22,
	DISPLAY_FORMAT_RGB565	=		23,
	DISPLAY_FORMAT_RGB555	=		25,

	DISPLAY_FORMAT_RGB24_GDI=		26
}RENDER_FORMAT;


typedef struct tagEASY_PALYER_OSD
{
	char	stOSD[1024];
	DWORD	alpha;		//0-255
	DWORD	color;		//RGB(0xf9,0xf9,0xf9)
	DWORD	shadowcolor;		//RGB(0x4d,0x4d,0x4d) 全为0背景透明
	RECT	rect;		//OSD基于图像右上角显示区域
	int			size;		//just D3D Support
}EASY_PALYER_OSD;

typedef int (CALLBACK *MediaSourceCallBack)( int _channelId, int *_channelPtr, int _frameType, char *pBuf, RTSP_FRAME_INFO* _frameInfo);

LIB_EASYPLAYER_API int EasyPlayer_Init(char* key);
LIB_EASYPLAYER_API void EasyPlayer_Release();

// 增加RTSP回放相关接口参数 [5/9/2018 SwordTwelve]
LIB_EASYPLAYER_API int EasyPlayer_OpenStream(const char *url, HWND hWnd, RENDER_FORMAT renderFormat, 
	int rtpovertcp, const char *username, const char *password, MediaSourceCallBack callback=NULL, void *userPtr=NULL, 
	bool bHardDecode=true, char* startTime = NULL, char* endTime=NULL, float fScale = 1.0f );
LIB_EASYPLAYER_API void EasyPlayer_CloseStream(int channelId);
LIB_EASYPLAYER_API int EasyPlayer_SetFrameCache(int channelId, int cache);
LIB_EASYPLAYER_API int EasyPlayer_SetShownToScale(int channelId, int shownToScale);
LIB_EASYPLAYER_API int EasyPlayer_SetDecodeType(int channelId, int decodeKeyframeOnly);
LIB_EASYPLAYER_API int EasyPlayer_SetRenderRect(int channelId, LPRECT lpSrcRect);
LIB_EASYPLAYER_API int EasyPlayer_ShowStatisticalInfo(int channelId, int show);
LIB_EASYPLAYER_API int EasyPlayer_ShowOSD(int channelId, int show, EASY_PALYER_OSD osd);
LIB_EASYPLAYER_API int EasyPlayer_GetMediaInfo(int channelId, MEDIA_INFO& mediaInfo);

LIB_EASYPLAYER_API int EasyPlayer_SetDragStartPoint(int channelId, POINT pt);
LIB_EASYPLAYER_API int EasyPlayer_SetDragEndPoint(int channelId, POINT pt);
LIB_EASYPLAYER_API int EasyPlayer_ResetDragPoint(int channelId);

LIB_EASYPLAYER_API int EasyPlayer_StartManuRecording(int channelId);
LIB_EASYPLAYER_API int EasyPlayer_StopManuRecording(int channelId);

LIB_EASYPLAYER_API int EasyPlayer_PlaySound(int channelId);
LIB_EASYPLAYER_API int EasyPlayer_StopSound();

LIB_EASYPLAYER_API int		EasyPlayer_SetManuRecordPath(int channelId, const char* recordPath);
LIB_EASYPLAYER_API int		EasyPlayer_SetManuPicShotPath(int channelId, const char* shotPath);

LIB_EASYPLAYER_API int		EasyPlayer_StartManuPicShot(int channelId);//pThread->manuScreenshot
LIB_EASYPLAYER_API int		EasyPlayer_StopManuPicShot(int channelId);

// 增加RTSP回放相关接口 [5/9/2018 SwordTwelve]
//设置回放速度
LIB_EASYPLAYER_API int		EasyPlayer_SetPlaybackSpeed(int channelId, float fSpeed);
/* 跳转播放时间 */
LIB_EASYPLAYER_API int		EasyPlayer_PlaybackSeek(int channelId, const char *playTime);
/* 暂停播放 */
LIB_EASYPLAYER_API int		EasyPlayer_PlaybackPause(int channelId);
/* 恢复播放 */
LIB_EASYPLAYER_API int		EasyPlayer_PlaybackResume(int channelId);

#endif
