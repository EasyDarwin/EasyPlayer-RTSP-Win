// DlgVideo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyPlayer.h"
#include "DlgVideo.h"
#include "afxdialogex.h"
#include "EasyPlayerDlg.h"

// CDlgVideo �Ի���

IMPLEMENT_DYNAMIC(CDlgVideo, CDialogEx)

CDlgVideo::CDlgVideo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVideo::IDD, pParent)
{
	m_WindowId	=	-1;
	m_ChannelId	=	-1;
	bDrag		=	false;
	m_bDestoryWnd = false;
	InitialComponents();
}

CDlgVideo::~CDlgVideo()
{

}

void CDlgVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVideo, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDlgVideo::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_CHECK_OSD, &CDlgVideo::OnBnClickedCheckOsd)
	ON_WM_HSCROLL()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_RTPTRANSMODE, &CDlgVideo::OnBnClickedCheckRtptransmode)
//	ON_BN_CLICKED(IDC_BUTTON_SEEK, &CDlgVideo::OnBnClickedButtonSeek)
END_MESSAGE_MAP()


// CDlgVideo ��Ϣ�������
LRESULT CDlgVideo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_PAINT == message || WM_SIZE==message)
	{
		UpdateComponents();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode
	// string.
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
 
	if(dwSize < dwMinSize)
	{
		return false;
	}
 
	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return true;
}
BOOL CDlgVideo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateComponents();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgVideo::DestroyWindow()
{
	DeleteComponents();

	return CDialogEx::DestroyWindow();
}

void CDlgVideo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	HWND hWnd = ::GetParent(GetSafeHwnd());
	::PostMessageW(hWnd, WM_WINDOW_MAXIMIZED, m_WindowId, 0);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CDlgVideo::OnLButtonDown(UINT nFlags, CPoint point)
{
	bDrag = true;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgVideo::OnLButtonUp(UINT nFlags, CPoint point)
{
	bDrag = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDlgVideo::OnMouseMove(UINT nFlags, CPoint point)
{
	if (bDrag)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		if (! rcClient.IsRectEmpty())
		{
			int nX = (int)(((float)point.x / (float)rcClient.Width() * 100.0f));
			int nY = (int)(((float)point.y / (float)rcClient.Height() * 100.0f));

			TRACE("X: %d\tY: %d\n", nX, nY);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void	CDlgVideo::SetWindowId(int _windowId)	
{
	m_WindowId = _windowId;

	if (m_WindowId == 0)
	{
		if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"));
	}
	//if (m_WindowId == 1)
	//{
	//	//if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://121.15.129.227"));
	//	if (NULL != pEdtURL)		pEdtURL->SetWindowText(TEXT("rtsp://192.168.1.101:554/id=0"));
	//}	
}
void	CDlgVideo::SetURL(char *url)
{
	wchar_t wszURL[128] = {0,};
	MByteToWChar(url, wszURL, sizeof(wszURL)/sizeof(wszURL[0]));
	if (NULL != pEdtURL)		pEdtURL->SetWindowText(wszURL);
}

void	CDlgVideo::SetShownToScale(int shownToScale)
{
	if (m_ChannelId > 0)
	{
		EasyPlayer_SetShownToScale(m_ChannelId, shownToScale);
	}
}

void	CDlgVideo::InitialComponents()
{
	pDlgRender	=	NULL;
	pEdtURL		=	NULL;
	pChkOSD		=	NULL;
	pSliderCache=	NULL;
	pBtnPreview	=	NULL;
	pChkRTPTransMode = NULL;
	pChkDecodeMode = NULL;
	pBtnSeek = NULL;
}

void	CDlgVideo::CreateComponents()
{
	if (NULL == pDlgRender)
	{
		pDlgRender = new CDlgRender();
		pDlgRender->Create(IDD_DIALOG_RENDER, this);
		pDlgRender->ShowWindow(SW_SHOW);
	}

	__CREATE_WINDOW(pEdtURL,		CEdit,		IDC_EDIT_RTSP_URL);
	__CREATE_WINDOW(pChkOSD,		CButton,	IDC_CHECK_OSD);
	__CREATE_WINDOW(pSliderCache,	CSliderCtrl,IDC_SLIDER_CACHE);
	__CREATE_WINDOW(pBtnPreview,	CButton,	IDC_BUTTON_PREVIEW);
	__CREATE_WINDOW(pChkRTPTransMode	,	CButton,	IDC_CHECK_RTPTRANSMODE);
	__CREATE_WINDOW(pChkDecodeMode	,	CButton,	IDC_CHECK_DECODEMODE);
	__CREATE_WINDOW(pBtnSeek	,	CButton,	IDC_BUTTON_SEEK);

	if (NULL != pEdtURL)			pEdtURL->SetWindowText(TEXT("rtsp://"));
	if (NULL != pSliderCache)	pSliderCache->SetRange(1, 10);
	if (NULL != pSliderCache)	pSliderCache->SetPos(3);
	if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));

	if (pChkRTPTransMode)
	{
		pChkRTPTransMode->SetCheck(1);
	}
	if (pChkDecodeMode)
	{
		pChkDecodeMode->SetCheck(0);
	}
}
void	CDlgVideo::UpdateComponents()
{
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())		return;

	CRect	rcRender;
	rcRender.SetRect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom-20);
	__MOVE_WINDOW(pDlgRender, rcRender);
	if (NULL != pDlgRender)		pDlgRender->Invalidate();

	CRect	rcURL;
	rcURL.SetRect(rcClient.left, rcRender.bottom+2, rcClient.right-300, rcClient.bottom);
	__MOVE_WINDOW(pEdtURL, rcURL);
	if (NULL != pEdtURL)		pEdtURL->Invalidate();

	// RTP OVER TCP/UDP [8/17/2016 SwordTwelve]
	CRect	rcRTPMode;
	rcRTPMode.SetRect(rcURL.right + 10, rcURL.top, rcURL.right + 2 + 50, rcURL.bottom);
	__MOVE_WINDOW(pChkRTPTransMode, rcRTPMode);
	if (NULL != pChkRTPTransMode)		pChkRTPTransMode->Invalidate();	

	CRect	rcOSD;
	rcOSD.SetRect(rcRTPMode.right+10, rcRTPMode.top, rcRTPMode.right+2+55, rcRTPMode.bottom);
	__MOVE_WINDOW(pChkOSD, rcOSD);
	if (NULL != pChkOSD)		pChkOSD->Invalidate();

	CRect	rcDecodeMode;
	rcDecodeMode.SetRect(rcOSD.right+10, rcOSD.top, rcOSD.right+2+60, rcOSD.bottom);
	__MOVE_WINDOW(pChkDecodeMode, rcDecodeMode);
	if (NULL != pChkDecodeMode)		pChkDecodeMode->Invalidate();	

	CRect	rcCache;
	rcCache.SetRect(rcDecodeMode.right+2, rcDecodeMode.top, rcDecodeMode.right+2+70, rcDecodeMode.bottom);
	__MOVE_WINDOW(pSliderCache, rcCache);
	if (NULL != pSliderCache)		pSliderCache->Invalidate();

	CRect	rcPreview;
	rcPreview.SetRect(rcCache.right+2, rcURL.top-2, rcClient.right, rcURL.bottom);
	__MOVE_WINDOW(pBtnPreview, rcPreview);
	if (NULL != pBtnPreview)		pBtnPreview->Invalidate();
}

void	CDlgVideo::DeleteComponents()
{
	m_bDestoryWnd = true;
	if (m_ChannelId > 0)
	{
		EasyPlayer_CloseStream(m_ChannelId);
		m_ChannelId = -1;
	}
	__DELETE_WINDOW(pDlgRender);
}

bool __WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return false;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return true;
}

void CDlgVideo::OnBnClickedButtonPreview()
{
	if (m_ChannelId > 0)
	{
		int nChannelId = m_ChannelId;
		m_ChannelId = -1;
		EasyPlayer_CloseStream(nChannelId);

 		if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_ChannelId);

		if (NULL != pDlgRender)	
		{
			//����¼��ֹͣ״̬
			pDlgRender->SetChannelStatus(0,pDlgRender->channelStatus.showOSD);
			pDlgRender->Invalidate();
		}
		if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Play"));
	}
	else
	{
		wchar_t wszURL[128] = {0,};
		if (NULL != pEdtURL)	pEdtURL->GetWindowTextW(wszURL, sizeof(wszURL));
		if (wcslen(wszURL) < 1)		return;

// 		wchar_t wszUsername[32] = {0,};
// 		wchar_t wszPassword[32] = {0,};
// 		if (NULL != pEdtUsername)	pEdtUsername->GetWindowText(wszUsername, sizeof(wszUsername));
// 		if (NULL != pEdtPassword)	pEdtPassword->GetWindowText(wszPassword, sizeof(wszPassword));

		char szURL[128] = {0,};
		__WCharToMByte(wszURL, szURL, sizeof(szURL)/sizeof(szURL[0]));
		char szUsername[32] = {0,};
		char szPassword[32] = {0,};
// 		__WCharToMByte(wszUsername, szUsername, sizeof(szUsername)/sizeof(szUsername[0]));
// 		__WCharToMByte(wszPassword, szPassword, sizeof(szPassword)/sizeof(szPassword[0]));

		int nRtpOverTcp = 1;
		if (NULL != pChkRTPTransMode)
		{
			nRtpOverTcp= pChkRTPTransMode->GetCheck();
		}
		bool bHardDecode = true;
		if (pChkDecodeMode)
		{
			int nCheck = pChkDecodeMode->GetCheck() ;
			if (nCheck== 1)
			{
				bHardDecode = true;
			} 
			else
			{
				bHardDecode = false ;
			}
		}


		HWND hWnd = NULL;
		if (NULL != pDlgRender)	hWnd = pDlgRender->GetSafeHwnd();
		m_ChannelId = EasyPlayer_OpenStream(szURL, hWnd, (RENDER_FORMAT)RenderFormat, nRtpOverTcp, szUsername, szPassword, &CDlgVideo::EasyPlayerCallBack, this, bHardDecode, NULL, NULL);
		if (m_ChannelId > 0)
		{
			int iPos = pSliderCache->GetPos();
			EasyPlayer_SetFrameCache(m_ChannelId, iPos);		//���û���
			EasyPlayer_PlaySound(m_ChannelId);

#if 0	//OSD Example
			EASY_PALYER_OSD osd;
			osd.alpha = 255;
			osd.size = 35;
			osd.color = RGB(255,0,255);
			osd.rect.left = 10;
			osd.rect.right = 5000;
			osd.rect.top = 100;
			osd.rect.bottom = 800;
			osd.shadowcolor = RGB(0,0,0);
			char* ss =  "����EasyPlayer-RTSP-Win������ \r\n����Ļ���ӽӿڵ�Ч��������\r\n��\"\\r\\n\"Ϊ���н�������\r\nע�⣺ÿ�еĳ��Ȳ��ܳ���128���ֽ�\r\n�ܵ�OSD���Ȳ��ܳ���1024���ֽ�";
			strcpy(osd.stOSD ,ss);
			EasyPlayer_ShowOSD(m_ChannelId, 1,  osd);
#endif
			CString strFilePath = GET_MODULE_FILE_INFO.strPath;
			char sFilePath[MAX_PATH];
			__WCharToMByte(strFilePath.GetBuffer(strFilePath.GetLength()), sFilePath, sizeof(sFilePath)/sizeof(sFilePath[0]));

			EasyPlayer_SetShownToScale(m_ChannelId, shownToScale);

			// ����ץͼ��¼�ƴ��·�� [10/10/2016 dingshuai]
			EasyPlayer_SetManuRecordPath(m_ChannelId, sFilePath);
			EasyPlayer_SetManuPicShotPath(m_ChannelId, sFilePath,"test");

			if (NULL != pDlgRender)	pDlgRender->SetChannelId(m_ChannelId);

			if (NULL != pBtnPreview)		pBtnPreview->SetWindowText(TEXT("Stop"));
		}
	}
}

void CDlgVideo::LogErr(CString strLog)
{
	if(!strLog.IsEmpty())
	{
		TCHAR* szLog = new TCHAR[strLog.GetLength()+1];
		StrCpy(szLog, strLog);
		CWnd* m_pMainDlg = GetParent();
		if(m_pMainDlg)
			m_pMainDlg->PostMessage(MSG_LOG, 0, (LPARAM)szLog);
// 		delete[] szLog;
// 		szLog = NULL;
	}
}

int CDlgVideo::EasyPlayerCallBack( int _channelId, int *_channelPtr, int _frameType, char *pBuf, EASY_FRAME_INFO* _frameInfo)
{
	if (_frameType == EASY_SDK_EVENT_FRAME_FLAG)
	{
		TRACE( "%s", pBuf  );
		CDlgVideo* pMaster = (CDlgVideo*)_channelPtr;
		if (pMaster->m_bDestoryWnd)
		{
			return 0;
		}
		if (pMaster)
		{
			CString str = (CString)pBuf;
			pMaster->LogErr(str);
		}
		if (_frameInfo&&_frameInfo->codec == EASY_SDK_EVENT_CODEC_EXIT)
		{
			//pMaster->OnBnClickedButtonPreview();
			if (pMaster->m_ChannelId > 0)
			{
				//EasyPlayer_CloseStream(pMaster->m_ChannelId);
				pMaster->m_ChannelId = -1;

				if (NULL != pMaster->pDlgRender)	pMaster->pDlgRender->SetChannelId(pMaster->m_ChannelId);

				if (NULL !=pMaster->pDlgRender)			pMaster->pDlgRender->Invalidate();
				if (NULL != pMaster->pBtnPreview)		pMaster->pBtnPreview->SetWindowText(TEXT("Play"));
			}

		}
		else if(_frameInfo && EASY_SDK_DECODE_VIDEO_FLAG == _frameInfo->codec )
		{

		}
	}
	return 0;
}


void CDlgVideo::OnBnClickedCheckOsd()
{
	int nShow = 0x00;

	if (NULL != pChkOSD)		nShow = pChkOSD->GetCheck();

	if (m_ChannelId > 0)
	{
		EasyPlayer_ShowStatisticalInfo(m_ChannelId, nShow);
	}
}

void CDlgVideo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( NULL != pScrollBar && NULL != pSliderCache &&
		pSliderCache->GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
	{
		int iPos = pSliderCache->GetPos();
		
		if (m_ChannelId > 0)
		{
			EasyPlayer_SetFrameCache(m_ChannelId, iPos);
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgVideo::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CDlgVideo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

}


void CDlgVideo::OnBnClickedCheckRtptransmode()
{
	// TODO: Add your control notification handler code here

}


//void CDlgVideo::OnBnClickedButtonSeek()
//{
//	UpdateData(TRUE);
//	CString strSeekTime;
//	if (NULL != pEdtSeekTime)	
//		pEdtSeekTime->GetWindowText( strSeekTime);
//	USES_CONVERSION;
//	char* playTime = T2A(strSeekTime);
//	int nRet = EasyPlayer_PlaybackSeek(m_ChannelId, playTime);
//	if(nRet == 0)
//	{
//
//
//	}
//}
