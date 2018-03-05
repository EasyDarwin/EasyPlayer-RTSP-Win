/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// Add by SwordTwelve
// EasyMP4Writer.h: interface for the ZMP4AVC1class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ZMP4GPAC_H)
#define ZMP4GPAC_H

#include <time.h>

#ifndef _WIN32
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#else
#include <io.h>
#include <direct.h>
#include <windows.h>
#endif

#include <string>

#ifndef gap_char
#define gap_char ('\\')
#endif
#ifndef gap_str
#define gap_str ("\\")
#endif

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
#include <gpac/tools.h>
#include <gpac/media_tools.h>
#include <gpac/isomedia.h>
#include <gpac/constants.h>
#include <gpac/isomedia.h>
#include <gpac/internal/media_dev.h>
#ifdef __cplusplus
}
#endif
#pragma comment(lib,"libgpac.lib")


#define HTON32(x)  ((x>>24&0xff)|(x>>8&0xff00)|(x<<8&0xff0000)|(x<<24&0xff000000))

#ifndef ZOUTFILE_FLAG_FULL
#define ZOUTFILE_FLAG_VIDEO 0x01
#define ZOUTFILE_FLAG_AUDIO 0x02
#define ZOUTFILE_FLAG_FULL (ZOUTFILE_FLAG_AUDIO|ZOUTFILE_FLAG_VIDEO)
#endif

#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))
#define MAX_PATH_LENGTH 1024

class EasyMP4Writer 
{
	//设置风格
	int		m_flag;//包含音频视频

	long	m_audiostartimestamp;
	long	m_videostartimestamp;

	GF_ISOFile *p_file;//MP4文件

    GF_AVCConfig *p_config;//H264写MP4配置
	// 增加写MP4对H265的支持 [2017/1/17 Dingshuai]
	GF_HEVCConfig *p_hevc_config;//H265写MP4配置

	GF_ISOSample *p_videosample;//MP4视频帧
	GF_ISOSample *p_audiosample;//MP4音频帧

	GF_AVCConfigSlot  m_slotsps;
	GF_AVCConfigSlot  m_slotpps;
	GF_AVCConfigSlot  m_slotvps;
	GF_HEVCParamArray m_spss;
	GF_HEVCParamArray m_ppss;
	GF_HEVCParamArray m_vpss;

	int m_wight;
	int m_hight;
	int m_videtrackid;
	int m_audiotrackid;

	unsigned int i_videodescidx;
	unsigned int i_audiodescidx;
	bool m_bwritevideoinfo;
	bool m_bwriteaudioinfo;
	unsigned char* m_psps;
	unsigned char* m_ppps;
	int m_spslen;
	int m_ppslen;
	// 增加写MP4对H265的支持 [2017/1/17 Dingshuai]
	unsigned char* m_pvps;
	int m_vpslen;

public:
	EasyMP4Writer();
	virtual ~EasyMP4Writer();
public:

	bool CreateMP4File(char*filename,int flag);
	//sps,pps第一个字节为0x67或68,
	bool WriteH264SPSandPPS(unsigned char*sps,int spslen,unsigned char*pps,int ppslen,int width,int height);
	//如果视频是H265则调用以下函数
	bool WriteH265VPSSPSandPPS(unsigned char*vps, int vpslen, unsigned char*sps, int spslen, 
		unsigned char*pps, int ppslen, int width, int height);

	//写入AAC信息
	bool WriteAACInfo(unsigned char*info,int len, int nSampleRate, int nChannel, int nBitsPerSample);
	//写入一帧h264，前四字节为该帧NAL长度
	bool WriteVideoFrame(unsigned char*data,int len,bool keyframe,long timestamp);
	//写入aac数据，只有raw数据
	bool WriteAACFrame(unsigned char*data,int len,long timestamp);
	int WriteAACToMp4File(unsigned char*data,int len,long timestamp, int sample_rate, int channels, int bits_per_sample);
	int WriteH264ToMp4File(unsigned char* pdata, int datasize, bool keyframe, long nTimestamp, int nWidth, int nHeight);
	int WriteH265ToMp4File(unsigned char* pdata, int datasize, bool keyframe, long nTimestamp, int nWidth, int nHeight);
	//保存文件
	bool SaveFile();
	bool CanWrite();
	int EnsureDirExist(string dir);
	//检测磁盘是否已满， fSpace表示磁盘剩余多少为满(单位：GB)
	BOOL CheckDiskSpacePlenty(const char* strPath, float fSpace = 1.0);
	//字符编码转换
	void Convert(const char* strIn, char* strOut, int sourceCodepage, int targetCodepage);

public:
	// 重载兼容性接口,增加MP4Writer进行切片处理 
	// nTargetDuration 单位：s
	int ResetStreamCache(string strRootDir, string strSubDir, string strMediaName, int nTargetDuration, int nFreeDisk);
	int VideoMux(int nEncType,unsigned int uiFrameType, const u8 *data, int dataLength, unsigned long long dts, int width, int height);
	int AudioMux(const u8 *data, int dataLength, unsigned long long dts, int nSampleRate, int nChannel, int nBitsPerSample);
	void ReleaseMP4Writer();

private:
	unsigned char* FindNal(unsigned char*buff,int inlen,int&outlen,bool&end);
	int find_nal_unit(unsigned char* buf, int size, int* nal_start, int* nal_end);

	int	MP4Writer_Lock()
	{
#ifdef _WIN32
		WaitForSingleObject(m_hMutex, INFINITE);		//Lock
#else
		pthread_mutex_lock(&m_hMutex);
#endif
		return 0;
	}

	int	MP4Writer_UnLock()
	{
#ifdef _WIN32
		ReleaseMutex(m_hMutex);
#else
		pthread_mutex_unlock(&m_hMutex);
#endif
		return 0;
	}

private:

#ifdef _WIN32
	HANDLE			m_hMutex;
#else
	pthread_mutex_t m_hMutex;
#endif

	int    m_nCapacity;
	int	   m_nTargetDuration;
	string m_strRootDir;
	string m_strSubDir;
	string m_strWholeDir;
	string m_strMediaName;
	bool   m_bRecordStart;
	int    m_nCreateFileFlag;
	 int m_nFreeDisk;

	//当前的
	int m_nCurMediaFirstDTS;
	//第一个文件存储索引
	int     m_nMediaFirstIndex;
	//最后一个文件存储索引
	int     m_nMediaLastIndex;

};

#endif // !defined(AFX_ZMP4_H__CD8C3DF9_A2A4_494D_948E_5672ADBE739D__INCLUDED_)
