/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// RTSP流接收(播放)管理类，接收音视频流 [11/8/2015 Dingshuai]
// Add by SwordTwelve

#pragma once

#define KEY "79393674363469576B5A73417275646170526852532F74555A584E304B56634D5671442F532B424859585A7062695A4359574A76633246414D6A41784E6B566863336C4559584A33615735555A5746745A57467A65513D3D"


#include "../libEasyPlayer/libEasyPlayerAPI.h"

//Gavin's Source Struct流信息结构
typedef struct __EASY_LOCAL_SOURCE_T
{	
	int		rtspSourceId;
	void*	pusherHandle;

	char	pushServerAddr[128];
	int		pushServerPort;
	char	sdpName[64];
	void* pMaster;
}EASY_LOCAL_SOURCE_T;

class EasyPlayerManager
{
public:
	EasyPlayerManager(void);
	~EasyPlayerManager(void);

	//Member Function
public:
	//初始化
	static int Init()
	{
		return EasyPlayer_Init((char*)KEY);
	}

	// 释放
	static void UnInit()
	{
		EasyPlayer_Release();
	}

	//打开流
	int Start(char* szURL, HWND hShowWnd, RENDER_FORMAT eRenderFormat, int rtpovertcp, const char *username, const char *password, int bHardDecode, MediaSourceCallBack callback=NULL, void *userPtr=NULL) ;
	//设置参数
	void Config(int nFrameCache,  BOOL bPlaySound, BOOL bShowToScale = TRUE, BOOL  bShowStatisticInfo = FALSE);
	//关闭流
	void	Close();
	void SetOSD(int show, EASY_PALYER_OSD osd);
	int InRunning();

	//Member Var
private:
	//接收的流信息
	EASY_LOCAL_SOURCE_T		m_sSourceInfo;

};

