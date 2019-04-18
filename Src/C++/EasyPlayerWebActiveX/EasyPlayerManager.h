/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// RTSP������(����)�����࣬��������Ƶ�� [11/8/2015 Dingshuai]
// Add by SwordTwelve

#pragma once

#define KEY "6D75724D7A4969576B5A73414F7A314C744D4B6F652F4A705A586877624739795A5849755A58686C567778576F4F4E372F69426C59584E35"


#include "../libEasyPlayer/libEasyPlayerAPI.h"

//Gavin's Source Struct����Ϣ�ṹ
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
	//��ʼ��
	static int Init()
	{
		return EasyPlayer_Init((char*)KEY);
	}

	// �ͷ�
	static void UnInit()
	{
		EasyPlayer_Release();
	}

	//����
	int Start(char* szURL, HWND hShowWnd, RENDER_FORMAT eRenderFormat, int rtpovertcp, const char *username, const char *password, int bHardDecode, MediaSourceCallBack callback=NULL, void *userPtr=NULL) ;
	//���ò���
	void Config(int nFrameCache,  BOOL bPlaySound, BOOL bShowToScale = TRUE, BOOL  bShowStatisticInfo = FALSE);
	//�ر���
	void	Close();
	void SetOSD(int show, EASY_PALYER_OSD osd);
	int InRunning();

	//Member Var
private:
	//���յ�����Ϣ
	EASY_LOCAL_SOURCE_T		m_sSourceInfo;

};

