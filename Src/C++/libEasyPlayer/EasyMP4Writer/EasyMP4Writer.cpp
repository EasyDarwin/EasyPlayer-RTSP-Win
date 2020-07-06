/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// Add by SwordTwelve
#include "EasyMP4Writer.h"

unsigned char* EasyMP4Writer::FindNal(unsigned char*buff,int inlen,int&outlen,bool& end)
{
	unsigned char*tempstart=NULL;
	unsigned char*search=buff+2;
	unsigned char*searchper1=buff;
	unsigned char*searchper2=buff+1;
	outlen=0;
	end=false;
	while((search-buff)<inlen)
	{
		if (search[0]==0x01&&searchper1[0]==0x00&&searchper2[0]==0x00)
		{
			if (tempstart==NULL)
			{
				tempstart=search+1;
			}
			else
			{
				outlen=search-tempstart-3+1;
				break;
			}
		}
		searchper2=searchper1;
		searchper1=search;
		search++;
	}
	if (outlen==0&&tempstart!=NULL)
	{
		outlen=search-tempstart;
		end=true;
	}
	if (tempstart==NULL)
	{
		end=true;
	}
	return tempstart;
}

//STD Find Nal unit
/** this function is taken from the h264bitstream library written by Alex Izvorski and Alex Giladi
 Find the beginning and end of a NAL (Network Abstraction Layer) unit in a unsigned char buffer containing H264 bitstream data.
 @param[in]   buf        the buffer
 @param[in]   size       the size of the buffer
 @param[out]  nal_start  the beginning offset of the nal
 @param[out]  nal_end    the end offset of the nal
 @return                 the length of the nal, or 0 if did not find start of nal, or -1 if did not find end of nal
 */
int EasyMP4Writer::find_nal_unit(unsigned char* buf, int size, int* nal_start, int* nal_end)
{
    int i;
    // find start
    *nal_start = 0;
    *nal_end = 0;

    i = 0;
    while (   //( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
           (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0x01) &&
           (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0 || buf[i+3] != 0x01)
           )
    {
        i++; // skip leading zero
        if (i+4 >= size) { return 0; } // did not find nal start
    }

    if  (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0x01) // ( next_bits( 24 ) != 0x000001 )
    {
        i++;
    }

    if  (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0x01) { /* error, should never happen */ return 0; }
    i+= 3;
    *nal_start = i;

    while (   //( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
           (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0) &&
           (buf[i] != 0 || buf[i+1] != 0 || buf[i+2] != 0x01)
           )
    {
        i++;
        // FIXME the next line fails when reading a nal that ends exactly at the end of the data
        if (i+3 >= size) 
		{ 
			*nal_end = size; 
			 return (*nal_end - *nal_start);
		} // did not find nal end, stream ended first
    }

    *nal_end = i;
    return (*nal_end - *nal_start);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EasyMP4Writer::EasyMP4Writer()
{
	m_videtrackid=-1;
	m_audiotrackid=-1;
	m_audiostartimestamp=-1;
	m_videostartimestamp=-1;

	m_nFreeDisk = 1;

	m_flag=0;
	p_file=NULL;
	p_config=NULL;
	p_hevc_config = NULL;
	p_videosample=NULL;
	p_audiosample=NULL;

	m_psps=NULL;
	m_ppps=NULL;
	m_spslen=0;
	m_ppslen=0;
	m_pvps = NULL;
	m_vpslen = 0;

	m_bwritevideoinfo = false;
	m_bwriteaudioinfo = false;

	memset(&m_slotsps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_slotpps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_slotvps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_spss, 0 ,sizeof(GF_HEVCParamArray));
	memset(&m_ppss, 0 ,sizeof(GF_HEVCParamArray));
	memset(&m_vpss, 0 ,sizeof(GF_HEVCParamArray));

	//  [11/2/2016 dingshuai]
	//第一个文件存储索引
	m_nMediaFirstIndex = 0;
	//最后一个文件存储索引
	m_nMediaLastIndex = 0;
	m_nCurMediaFirstDTS = -1;
	m_nCreateFileFlag = -1;

#ifdef _WIN32
	wchar_t wszMutexName[36] = { 0, };
	wsprintf(wszMutexName, TEXT("%s%d_mutex"), TEXT("MP4Writer"), this);
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, wszMutexName);
	if (NULL == m_hMutex)
	{
		m_hMutex = CreateMutex(NULL, FALSE, wszMutexName);
	}
#else
	pthread_mutex_init(&m_hMutex, NULL);
#endif
	m_bRecordStart = false;
}

EasyMP4Writer::~EasyMP4Writer()
{
}

//字符编码转换
void EasyMP4Writer::Convert(const char* strIn, char* strOut, int sourceCodepage, int targetCodepage)
{
#ifdef _WIN32
	int len = strlen(strIn);
	int unicodeLen = ::MultiByteToWideChar(sourceCodepage, 0, strIn, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(sourceCodepage, 0, strIn, -1, (LPWSTR)pUnicode, unicodeLen);
	BYTE * pTargetData = NULL;
	int targetLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, 0, NULL, NULL);
	pTargetData = new BYTE[targetLen + 1];
	memset(pTargetData, 0, targetLen + 1);
	WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);
	strcpy(strOut, (char*)pTargetData);
	delete pUnicode;
	delete pTargetData;
#endif
}

bool EasyMP4Writer::CreateMP4File(char*filename,int flag)
{
	SaveFile();
	m_audiostartimestamp=-1;
	m_videostartimestamp=-1;
	char filename2[256]={0};
	if(filename==NULL)
	{
		sprintf(filename2,"%d-gpac%d.mp4",time(NULL),rand());
		p_file=gf_isom_open(filename2,GF_ISOM_OPEN_WRITE,NULL);//打开文件
	}
	else
	{
		Convert(filename, filename2, CP_ACP, CP_UTF8);
		p_file = gf_isom_open(filename2, GF_ISOM_OPEN_WRITE, NULL);//打开文件
	}

	if (p_file==NULL)
	{
		return false;
	}

	gf_isom_set_brand_info(p_file,GF_ISOM_BRAND_MP42,0);

	//if(flag&ZOUTFILE_FLAG_VIDEO)
	//{
	//	m_videtrackid=gf_isom_new_track(p_file,0,GF_ISOM_MEDIA_VISUAL,1000);
	//	gf_isom_set_track_enabled(p_file,m_videtrackid,1);
	//}
	//if(flag&ZOUTFILE_FLAG_AUDIO)
	//{
	//	m_audiotrackid=gf_isom_new_track(p_file,0,GF_ISOM_MEDIA_AUDIO,1000);
	//	gf_isom_set_track_enabled(p_file,m_audiotrackid,1);
	//}
	m_nCreateFileFlag = flag;

	return true;
}
//sps,pps第一个字节为0x67或68,
bool EasyMP4Writer::WriteH264SPSandPPS(unsigned char*sps,int spslen,unsigned char*pps,int ppslen,int width,int height)
{	
	if (m_nCreateFileFlag&ZOUTFILE_FLAG_VIDEO)
	{
		m_videtrackid = gf_isom_new_track(p_file, 0, GF_ISOM_MEDIA_VISUAL, 1000);
		gf_isom_set_track_enabled(p_file, m_videtrackid, 1);
	}
	else
	{
		return false;
	}
	p_videosample=gf_isom_sample_new();
	p_videosample->data=(char*)malloc(1024*1024);


	p_config=gf_odf_avc_cfg_new();	
	gf_isom_avc_config_new(p_file,m_videtrackid,p_config,NULL,NULL,&i_videodescidx);
	gf_isom_set_visual_info(p_file,m_videtrackid,i_videodescidx,width,height);

	GF_AVCConfigSlot m_slotsps={0};
	GF_AVCConfigSlot m_slotpps={0};
	
	p_config->configurationVersion = 1;
	p_config->AVCProfileIndication = sps[1];
	p_config->profile_compatibility = sps[2];
	p_config->AVCLevelIndication = sps[3];
	
	m_slotsps.size=spslen;
	m_slotsps.data=(char*)malloc(spslen);
	memcpy(m_slotsps.data,sps,spslen);	
	gf_list_add(p_config->sequenceParameterSets,&m_slotsps);
	
	m_slotpps.size=ppslen;
	m_slotpps.data=(char*)malloc(ppslen);
	memcpy(m_slotpps.data,pps,ppslen);
	gf_list_add(p_config->pictureParameterSets,&m_slotpps);
	
	gf_isom_avc_config_update(p_file,m_videtrackid,1,p_config);

	free(m_slotsps.data);
	free(m_slotpps.data);

	return true;
}

bool EasyMP4Writer::WriteH265VPSSPSandPPS(unsigned char*vps, int vpslen, unsigned char*sps, int spslen,
	unsigned char*pps, int ppslen, int width, int height)
{
	if (m_nCreateFileFlag&ZOUTFILE_FLAG_VIDEO)
	{
		m_videtrackid = gf_isom_new_track(p_file, 0, GF_ISOM_MEDIA_VISUAL, 1000);
		gf_isom_set_track_enabled(p_file, m_videtrackid, 1);
	}
	else
	{
		return false;
	}
	p_videosample = gf_isom_sample_new();
	p_videosample->data = (char*)malloc(1024 * 1024);

	p_hevc_config = gf_odf_hevc_cfg_new();
	p_hevc_config->nal_unit_size = 32 / 8;

	//gf_isom_avc_config_new(p_file, m_videtrackid, p_config, NULL, NULL, &i_videodescidx);
	//gf_isom_set_visual_info(p_file, m_videtrackid, i_videodescidx, width, height);
	//初始化配置
	gf_isom_hevc_config_new(p_file, m_videtrackid, p_hevc_config, NULL, NULL, &i_videodescidx);
	gf_isom_set_nalu_extract_mode(p_file, m_videtrackid, GF_ISOM_NALU_EXTRACT_INSPECT);
	gf_isom_set_cts_packing(p_file, m_videtrackid, GF_TRUE);

	HEVCState hevc;
	memset(&hevc, 0 ,sizeof(HEVCState));

	memset(&m_slotsps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_slotpps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_slotvps, 0 ,sizeof(GF_AVCConfigSlot));
	memset(&m_spss, 0 ,sizeof(GF_HEVCParamArray));
	memset(&m_ppss, 0 ,sizeof(GF_HEVCParamArray));
	memset(&m_vpss, 0 ,sizeof(GF_HEVCParamArray));

	p_hevc_config->configurationVersion = 1;

	//Config vps
	int idx = gf_media_hevc_read_vps((char*)vps, vpslen, &hevc);
	hevc.vps[idx].crc = gf_crc_32((char*)vps, vpslen);
	p_hevc_config->avgFrameRate = hevc.vps[idx].rates[0].avg_pic_rate;
	p_hevc_config->constantFrameRate = hevc.vps[idx].rates[0].constand_pic_rate_idc;
	p_hevc_config->numTemporalLayers = hevc.vps[idx].max_sub_layers;
	p_hevc_config->temporalIdNested = hevc.vps[idx].temporal_id_nesting;

	m_vpss.nalus = gf_list_new();
	gf_list_add(p_hevc_config->param_array, &m_vpss);
	m_vpss.array_completeness = 1;
	m_vpss.type = GF_HEVC_NALU_VID_PARAM;// naltype = VPS
	m_slotvps.id = idx;
	m_slotvps.size = vpslen;
	m_slotvps.data = (char*)malloc(vpslen);
	memcpy(m_slotvps.data, vps, vpslen);
	gf_list_add(m_vpss.nalus, &m_slotvps);

	//Config sps
	idx = gf_media_hevc_read_sps((char*)sps, spslen, &hevc);
	hevc.sps[idx].crc = gf_crc_32((char*)sps, spslen);
	p_hevc_config->profile_space = hevc.sps[idx].ptl.profile_space;
	p_hevc_config->tier_flag = hevc.sps[idx].ptl.tier_flag;
	p_hevc_config->profile_idc = hevc.sps[idx].ptl.profile_idc;

	m_spss.nalus = gf_list_new();
	gf_list_add(p_hevc_config->param_array, &m_spss);
	m_spss.array_completeness = 1;
	m_spss.type = GF_HEVC_NALU_SEQ_PARAM;// naltype = SPS
	m_slotsps.id = idx;
	m_slotsps.size = spslen;
	m_slotsps.data = (char*)malloc(spslen);
	memcpy(m_slotsps.data, sps, spslen);
	gf_list_add(m_spss.nalus, &m_slotsps);
	int act_width = hevc.sps[idx].width;
	int act_height = hevc.sps[idx].height;

	//Config pps
	idx = gf_media_hevc_read_pps((char*)pps, ppslen, &hevc);
	hevc.pps[idx].crc = gf_crc_32((char*)pps, ppslen);

	m_ppss.nalus = gf_list_new();
	gf_list_add(p_hevc_config->param_array, &m_ppss);
	m_ppss.array_completeness = 1;
	m_ppss.type = GF_HEVC_NALU_PIC_PARAM;// naltype = PPS
	m_slotpps.id = idx;
	m_slotpps.size = ppslen;
	m_slotpps.data = (char*)malloc(ppslen);
	memcpy(m_slotpps.data, pps, ppslen);
	gf_list_add(m_ppss.nalus, &m_slotpps);

	gf_isom_set_visual_info(p_file, m_videtrackid, i_videodescidx, act_width, act_height);
	gf_isom_hevc_config_update(p_file, m_videtrackid, 1, p_hevc_config);

	//销毁申请的内存资源
	gf_list_del(m_vpss.nalus);
	gf_list_del(m_spss.nalus);
	gf_list_del(m_ppss.nalus);
	free(m_slotvps.data);
	free(m_slotsps.data);
	free(m_slotpps.data);
	p_hevc_config->param_array = NULL;
	return true;
}


//写入AAC信息
bool EasyMP4Writer::WriteAACInfo(unsigned char*info,int len, int nSampleRate, int nChannel, int nBitsPerSample)
{
	if (m_nCreateFileFlag&ZOUTFILE_FLAG_AUDIO)
	{
		m_audiotrackid = gf_isom_new_track(p_file, 0, GF_ISOM_MEDIA_AUDIO, 1000);
		gf_isom_set_track_enabled(p_file, m_audiotrackid, 1);
	}
	else
	{
		return false;
	}
	p_audiosample=gf_isom_sample_new();
	p_audiosample->data=(char*)malloc(1024*10);

	GF_ESD*esd=	gf_odf_desc_esd_new(0);
	esd->ESID=gf_isom_get_track_id(p_file,m_audiotrackid);
	esd->OCRESID=gf_isom_get_track_id(p_file,m_audiotrackid);
	esd->decoderConfig->streamType=0x05;
	esd->decoderConfig->objectTypeIndication=0x40;//0x40;
	esd->slConfig->timestampResolution=1000;//1000;//时间单元	
	esd->decoderConfig->decoderSpecificInfo=(GF_DefaultDescriptor*)gf_odf_desc_new(GF_ODF_DSI_TAG);

	esd->decoderConfig->decoderSpecificInfo->data=(char*)malloc(len);
	memcpy(esd->decoderConfig->decoderSpecificInfo->data,info,len);
	esd->decoderConfig->decoderSpecificInfo->dataLength=len;
	

	GF_Err gferr=gf_isom_new_mpeg4_description(p_file, m_audiotrackid, esd,  NULL, NULL, &i_audiodescidx);
	if (gferr!=0)
	{
//		TRACE("mpeg4_description:%d\n",gferr);
	}
	gferr=gf_isom_set_audio_info(p_file,m_audiotrackid,i_audiodescidx, nSampleRate,nChannel, nBitsPerSample);//44100 2 16
	if (gferr!=0)
	{
//		TRACE("gf_isom_set_audio:%d\n",gferr);
	}

	free(esd->decoderConfig->decoderSpecificInfo->data);

	return true;
}
//写入一帧，前四字节为该帧NAL长度
bool EasyMP4Writer::WriteVideoFrame(unsigned char*data,int len,bool keyframe,long timestamp)
{		
	if (!p_videosample)
	{
		return false;
	}
	if (m_videostartimestamp==-1&&keyframe)
	{
		m_videostartimestamp=timestamp;
	}
	if (m_videostartimestamp!=-1)
	{
		p_videosample->IsRAP=(SAPType)keyframe;
		p_videosample->dataLength=len;
		memcpy(p_videosample->data,data,len);
		p_videosample->DTS=timestamp-m_videostartimestamp;
		p_videosample->CTS_Offset=0;	
		GF_Err gferr=gf_isom_add_sample(p_file,m_videtrackid,i_videodescidx,p_videosample);			
		if (gferr==-1)
		{
			p_videosample->DTS=timestamp-m_videostartimestamp+15;
			gf_isom_add_sample(p_file,m_videtrackid,i_videodescidx,p_videosample);
		}
	}
	
	return true;
}
//写入aac数据，只有raw数据
bool EasyMP4Writer::WriteAACFrame(unsigned char*data,int len,long timestamp)
{
	if (!p_audiosample)
	{
		return false;
	}
	if (m_audiostartimestamp==-1)
	{
		m_audiostartimestamp=timestamp;
	}
	p_audiosample->IsRAP=(SAPType)1;
	p_audiosample->dataLength=len;
	memcpy(	p_audiosample->data,data,len);
	p_audiosample->DTS=timestamp-m_audiostartimestamp;
	p_audiosample->CTS_Offset=0;	
	GF_Err gferr=gf_isom_add_sample(p_file,m_audiotrackid,i_audiodescidx,p_audiosample);			
	if (gferr==-1)
	{
		p_audiosample->DTS=timestamp-m_audiostartimestamp+15;
		gf_isom_add_sample(p_file,m_audiotrackid,i_audiodescidx,p_audiosample);
	}
	return true;
}

int EasyMP4Writer::WriteAACToMp4File(unsigned char*data,int len,long timestamp, int sample_rate, int channels, int bits_per_sample)
{
	if (m_bwriteaudioinfo==false&&sample_rate>0)
	{
		if (channels<=0)
		{
			channels = 2;
		}
		if (bits_per_sample<=0)
		{
			bits_per_sample = 16;
		}
		// 		前五位为 AAC object types  LOW          2
		// 		接着4位为 码率index        16000        8
		// 		采样标志标准：
		//	static unsigned long tnsSupportedSamplingRates[13] = //音频采样率标准（标志），下标为写入标志
		//	{ 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,0 };

		// 		接着4位为 channels 个数                 2
		//		最后3位用0补齐
		// 		应打印出的正确2进制形式为  00010 | 1000 | 0010 | 000
		// 									 2       8      2
		//  BYTE ubDecInfoBuff[] =  {0x12,0x10};//00010 0100 0010 000

		//音频采样率标准（标志），下标为写入标志
		unsigned long tnsSupportedSamplingRates[13] = { 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,0 };
		int nI = 0;
		for ( nI = 0; nI<13; nI++)
		{
			if (tnsSupportedSamplingRates[nI] == sample_rate )
			{
				break;
			}
		}
		unsigned char ucDecInfoBuff[2] = {0x12,0x10};//

		unsigned short  nDecInfo = (1<<12) | (nI << 7) | (channels<<3);
		int nSize = sizeof(unsigned short);
		memcpy(ucDecInfoBuff, &nDecInfo, nSize);
		SWAP(ucDecInfoBuff[0], ucDecInfoBuff[1]);
		int unBuffSize = sizeof(ucDecInfoBuff)*sizeof(unsigned char);

		WriteAACInfo(ucDecInfoBuff,unBuffSize, sample_rate, channels, bits_per_sample);
		m_bwriteaudioinfo = true;
	}
	// 音频带ADTS头写文件需要去掉，否则某些播放器可能播放不出声音 [9/21/2016 dingshuai]
	if (data[0] == 0xFF && (data[1]&0xF0) == 0xF0)
		WriteAACFrame(data+7, len-7, timestamp);
	else
		WriteAACFrame(data, len, timestamp);
	return 1;
}


//保存文件
bool EasyMP4Writer::SaveFile()
{
	if (m_psps)
	{
		delete m_psps;
		m_psps = NULL;
	}
	if (m_ppps)
	{
		delete m_ppps;
		m_ppps = NULL;
	}
	m_spslen=0;
	m_ppslen=0;
	if (m_pvps)
	{
		delete m_pvps;
		m_pvps = NULL;
	}
	m_vpslen = 0;

	m_audiostartimestamp=-1;
	m_videostartimestamp=-1;
	if (p_file)
	{
		gf_isom_close(p_file);
		p_file=NULL;
	}
	if(p_config)
	{
	//	delete p_config->pictureParameterSets;
		p_config->pictureParameterSets=NULL;
	//	delete p_config->sequenceParameterSets;
		p_config->sequenceParameterSets=NULL;
		gf_odf_avc_cfg_del(p_config);
		p_config=NULL;
	}
	if (p_hevc_config)
	{
		gf_odf_hevc_cfg_del(p_hevc_config);
		p_hevc_config = NULL;
	}
	if(	p_audiosample)
	{
		if(	p_audiosample->data)
		{
			free(p_audiosample->data);
			p_audiosample->data=NULL;
		}
		gf_isom_sample_del(&p_audiosample);
		p_audiosample=NULL;
	}

	if(	p_videosample)
	{
		if(	p_videosample->data)
		{
			free(p_videosample->data);
			p_videosample->data=NULL;
		}
		gf_isom_sample_del(&p_videosample);
		p_audiosample=NULL;
	}
	m_bwriteaudioinfo = false;
	m_bwritevideoinfo = false;
	return true;
}

bool EasyMP4Writer::CanWrite()
{
	return m_bwritevideoinfo;
}

int EasyMP4Writer::WriteH264ToMp4File(unsigned char* pdata, int datasize, bool keyframe, long nTimestamp, int nWidth, int nHeight)
{
	// for a test [10/10/2016 SwordTwelve]
#if 0
	static int n = 0;
	char filename[64] = {0,};
	sprintf(filename, "videoframe_%d.txt", n);
	FILE *f = fopen(filename, "wb");
	if (NULL != f)
	{
		fwrite(pdata, 1, datasize, f);
		fclose(f);
	}
	n++;
#endif
	if (nTimestamp==0||(pdata==NULL)||datasize<=0)
	{
		return -1;
	}
	int inlen=datasize;
	unsigned char*pin=pdata;
	int outlen=0;
	unsigned char*pout=NULL;
	bool bend = false;

	int datalen=0;
	bool bSPSOrPPS = false;

	int iOutLen = datasize;

	unsigned char* pRealData = new unsigned char[datasize<<1];
	int nRealDataSize = 0;
	memset(pRealData,0x00, datasize+4);
	do 
	{
//		pout=FindNal(pin,inlen,outlen,bend);
		int nal_start = 0;
		int nal_end = 0;
		outlen = find_nal_unit(pin,inlen, &nal_start, &nal_end );
		if (outlen<=0)
		{
			break;
		}
		pout = pin+nal_start;
		if(pout!=NULL)
		{
			unsigned char naltype = ( pout[0] & 0x1F);

			if (naltype==0x07)//0x67
			{
// 				m_psps=pout;
// 				m_spslen=outlen;
				//pout[0] = 0x67;
				if(m_bwritevideoinfo==false)
				{
					m_psps = new unsigned char[outlen];
					memcpy(m_psps, pout, outlen);
					m_spslen=outlen;
				}
				bSPSOrPPS = true;
			}
			else if (naltype==0x08)//0x68
			{
				// 				m_ppps=pout;
				// 				m_ppslen=outlen;
				//pout[0] = 0x68;
				if(m_bwritevideoinfo==false)
				{
					m_ppps = new unsigned char[outlen];//outlen
					memcpy(m_ppps, pout, outlen);
					m_ppslen = outlen;
				}
				bSPSOrPPS = true;
			}
			else if (naltype == 0x06)//SEI
			{
				//Do we need SEI??
			}
			else
			{
				memcpy(pRealData+nRealDataSize, &outlen, 4);
				//写入头4个字节==nal内容的长度(H264数据的长度)
				unsigned char byte0 = pRealData[nRealDataSize+3];
				unsigned char byte1 = pRealData[nRealDataSize+2];
				pRealData[nRealDataSize+3] = pRealData[nRealDataSize+0];
				pRealData[nRealDataSize+2] = pRealData[nRealDataSize+1];
				pRealData[nRealDataSize+1] = byte1;
				pRealData[nRealDataSize+0] = byte0;

				nRealDataSize += 4;

				memcpy(pRealData+nRealDataSize, pout, outlen);
				nRealDataSize += outlen;
			}

			inlen=inlen-outlen-(pout-pin);
			pin=pout+outlen;
		}
	} while (bend!=true);

	if (m_bwritevideoinfo==false&&m_ppps&&m_psps)
	{
		// PPS末尾的0过滤,否则VLC可能播放不出来 [12/22/2015 Dingshuai]
		int nPPSSize = m_ppslen;
		int nZeroCount = 0;
		for (int nI = nPPSSize-1; nI>=0; nI--)
		{
			if (m_ppps[nI] == 0x00)
			{
				nZeroCount++;
			}
			else
			{
				break;
			}
		}
		m_ppslen = m_ppslen-nZeroCount;
		WriteH264SPSandPPS(m_psps,m_spslen,m_ppps,m_ppslen,nWidth,nHeight);
		m_bwritevideoinfo = true;
	}
	if (m_bwritevideoinfo==true && nRealDataSize>0 )
	{
		WriteVideoFrame(pRealData, nRealDataSize, keyframe, nTimestamp);//左移4单位，加上数据长度头？
	}

	if (pRealData)
	{	
		delete []pRealData;
		pRealData = NULL;
	}

	return true;
}

int EasyMP4Writer::WriteH265ToMp4File(unsigned char* pdata, int datasize, bool keyframe, long nTimestamp, int nWidth, int nHeight)
{
	// for a test [10/10/2016 SwordTwelve]
#if 0
	static int n = 0;
	char filename[64] = { 0, };
	sprintf(filename, "videoframe_%d.txt", n);
	FILE *f = fopen(filename, "wb");
	if (NULL != f)
	{
		fwrite(pdata, 1, datasize, f);
		fclose(f);
	}
	n++;
#endif
	if (nTimestamp == 0 || (pdata == NULL) || datasize <= 0)
	{
		return -1;
	}
	int inlen = datasize;
	unsigned char*pin = pdata;
	int outlen = 0;
	unsigned char*pout = NULL;
	bool bend = false;

	int datalen = 0;
	bool bFindVPS = false;
	bool bFindPPS = false;
	bool bFindSPS = false;
	bool bFindSEI = false;

	int iOutLen = datasize;
	unsigned char* pRealData = new unsigned char[datasize << 1];
	int nRealDataSize = 0;
	memset(pRealData, 0x00, datasize + 4);
	do
	{
		//		pout=FindNal(pin,inlen,outlen,bend);
		int nal_start = 0;
		int nal_end = 0;
		outlen = find_nal_unit(pin, inlen, &nal_start, &nal_end);
		if (outlen <= 0)
		{
			break;
		}
		pout = pin + nal_start;
		if (pout != NULL)
		{
			//Find h265 naltype
			int naltype = ((pout[0]>>1) & 0x3F);
			//NAL_UNIT_VPS,                   // 32  
			//NAL_UNIT_SPS,                   // 33  
			//NAL_UNIT_PPS,                   // 34  
			if (naltype == 0x20)//VPS
			{
				if (m_bwritevideoinfo == false)
				{
					m_pvps = new unsigned char[outlen];
					memcpy(m_pvps, pout, outlen);
					m_vpslen = outlen;
				}
				bFindVPS = true;
			}
			else if (naltype == 0x21)//SPS
			{
				if (m_bwritevideoinfo == false)
				{
					m_psps = new unsigned char[outlen];
					memcpy(m_psps, pout, outlen);
					m_spslen = outlen;
				}
				bFindSPS = true;
			}
			else if (naltype == 0x22)//PPS
			{
				m_ppps = new unsigned char[outlen];//outlen
				memcpy(m_ppps, pout, outlen);
				m_ppslen = outlen;

				bFindPPS = true;
			}
			else if (naltype == 0x27 || naltype == 0x28)//SEI
			{
				bFindSEI = true;
			}
			//I or P frame data
			else if((naltype >= 0 && naltype <= 9) || (naltype >= 16 && naltype <= 21))
			{
				memcpy(pRealData + nRealDataSize, &outlen, 4);
				//写入头4个字节==nal内容的长度(H264数据的长度)
				unsigned char byte0 = pRealData[nRealDataSize + 3];
				unsigned char byte1 = pRealData[nRealDataSize + 2];
				pRealData[nRealDataSize + 3] = pRealData[nRealDataSize + 0];
				pRealData[nRealDataSize + 2] = pRealData[nRealDataSize + 1];
				pRealData[nRealDataSize + 1] = byte1;
				pRealData[nRealDataSize + 0] = byte0;

				nRealDataSize += 4;
				memcpy(pRealData + nRealDataSize, pout, outlen);
				nRealDataSize += outlen;
			}
			inlen = inlen - outlen - (pout - pin);
			pin = pout + outlen;
		}
	} while (bend != true);

	if (m_bwritevideoinfo == false && m_ppps&&m_psps)
	{
		// PPS末尾的0过滤,否则VLC可能播放不出来 [12/22/2015 Dingshuai]
		int nPPSSize = m_ppslen;
		int nZeroCount = 0;
		for (int nI = nPPSSize - 1; nI >= 0; nI--)
		{
			if (m_ppps[nI] == 0x00)
			{
				nZeroCount++;
			}
			else
			{
				break;
			}
		}
		m_ppslen = m_ppslen - nZeroCount;
		WriteH265VPSSPSandPPS(m_pvps, m_vpslen, m_psps, m_spslen, m_ppps, m_ppslen, nWidth, nHeight);
		m_bwritevideoinfo = true;
	}
	//if (m_bwritevideoinfo == false || nRealDataSize <= 0)
	//{
	//	return 0;//获取sps pps失败
	//}
	//WriteVideoFrame(pRealData, nRealDataSize, keyframe, nTimestamp);//左移4单位，加上数据长度头？
	if (m_bwritevideoinfo == true && nRealDataSize>0)
	{
		WriteVideoFrame(pRealData, nRealDataSize, keyframe, nTimestamp);//左移4单位，加上数据长度头？
	}

	if (pRealData)
	{
		delete[]pRealData;
		pRealData = NULL;
	}

	return true;
}


void EasyMP4Writer::ReleaseMP4Writer()
{
	SaveFile();
#ifdef _WIN32
	if (NULL != m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
#else
	pthread_mutex_destroy(&m_hMutex);
#endif

}

//检测磁盘是否满足剩余的要求， fSpace表示磁盘剩余多少为满(单位：GB)
BOOL EasyMP4Writer::CheckDiskSpacePlenty(const char* strPath, float fSpace)
{
	//CreateDirectory(strPath, NULL);
	BOOL bSuccess = FALSE;
	//检查磁盘空间
	ULARGE_INTEGER nFreeBytesAvailable;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER  ulFree;
	float  fTempFree = 0.0f;
	ULONG  lTempFree = 0;
	bSuccess = ::GetDiskFreeSpaceExA(strPath, &nFreeBytesAvailable, &nTotalNumberOfBytes, &ulFree);

	if (!bSuccess)
	{
		printf("GetDiskFreeSpaceEx %s Failed！\n", strPath);
		return 2;
	}
	lTempFree = (ULONG)(ulFree.QuadPart / 1024);
	fTempFree = (float)lTempFree;
	fTempFree = fTempFree / 1024 / 1024; //GB

	if (fTempFree <= fSpace)
	{
		printf("磁盘空间剩余 %f GB,不足 %f GB！\n", fTempFree, fSpace);
		return FALSE;
	}
	return TRUE;
}

int EasyMP4Writer::EnsureDirExist(string dir)
{
	int len = 0;
	char temp_dir[MAX_PATH_LENGTH];
	char * pindex = NULL;
	int access_flag = 0;
	const char * p_dir = dir.c_str();

	if (!access(p_dir, 0))
	{
		return 1;
	}
	//
	len = dir.size();
	if (len >= MAX_PATH_LENGTH)
	{
		return 0;
	}

	//
	memset(temp_dir, 0, MAX_PATH_LENGTH);
	memcpy(temp_dir, p_dir, len);

	//
	pindex = temp_dir;
	pindex = strrchr(pindex, gap_char);
	if (pindex >= (temp_dir + len - 1))
	{
		*pindex = 0;
	}

	//
	pindex = temp_dir;
	pindex = strchr(pindex, gap_char);
	if (!pindex)
	{
		return 0;
	}
	pindex++;

	//
	access_flag = 0;
	do
	{
		pindex = strchr(pindex, gap_char);

		if (pindex)
		{
			*pindex = 0;
		}

		access_flag = access(temp_dir, 0);

		if (access_flag)
		{
#ifdef _WIN32
			if (mkdir(temp_dir))
#else
			if (mkdir(temp_dir, 0x777))
#endif
			{
				return 0;
			}
		}

		if (pindex)
		{
			*pindex = gap_char;
			pindex++;
		}
	} while (pindex);

	//
	if (access(p_dir, 0))
	{
		return 0;
	}
	return 1;
}


int EasyMP4Writer::ResetStreamCache(string strRootDir, string strSubDir, string strMediaName, int nTargetDuration, int nFreeDisk)
{
	m_nCapacity = 0;
	m_strRootDir = strRootDir;
	m_strSubDir = strSubDir;
	m_strWholeDir = m_strRootDir + m_strSubDir;
	m_strMediaName = strMediaName;
	m_nTargetDuration = nTargetDuration * 1000;
	m_nFreeDisk = nFreeDisk;

	if (!EnsureDirExist(m_strWholeDir))
	{
		//PrintSysLog("CMediaPlaylist::UpdateM3u8(%s) => %s isnot exist .\n", m_strMediaName.c_str(), m_strWholeDir.c_str());
		return -1;
	}

	//判断磁盘空间是否足够
	BOOL bIsDiskSpacePlenty = CheckDiskSpacePlenty(m_strWholeDir.c_str(), m_nFreeDisk);
	if (!bIsDiskSpacePlenty)
	{
		printf("请重新设置录像存放路径！");
		return  -1;
	}
	
	return 1;
}

int EasyMP4Writer::VideoMux(int nEncType, unsigned int uiFrameType, const u8 *data, int dataLength, unsigned long long dts,
	int width, int height)
{
	MP4Writer_Lock();
	if (uiFrameType == 1)//I帧
	{
		if (!m_bRecordStart)
		{
			m_bRecordStart = true;
		}

		if (/*m_nCurMediaFirstDTS==-1 || */
			(m_nCurMediaFirstDTS >= 0 && ((dts - m_nCurMediaFirstDTS) >= m_nTargetDuration))
			)
		{
			//关掉上一个文件
			if (p_file)
			{
				SaveFile();
			}

			m_nMediaLastIndex++;

			//判断磁盘空间是否足够
			BOOL bIsDiskSpacePlenty = CheckDiskSpacePlenty(m_strWholeDir.c_str());
			//2. 更新MP4文件列表
			if (m_nCapacity>0 && ((m_nMediaLastIndex - m_nMediaFirstIndex >= m_nCapacity)|| !bIsDiskSpacePlenty))
			{
#if 0
				char szStreamCacheName[1024] = { 0 };
				sprintf(szStreamCacheName, "%s_%d.mp4", m_strMediaName.c_str(), m_nMediaFirstIndex);
				string strFilePath = m_strWholeDir + szStreamCacheName;
				//删除文件
				//DeleteFileA(tmp);  
				unlink(strFilePath.c_str());
#endif
				m_nMediaFirstIndex++;
			}
		}
		//3. 创建新的mp4文件
		if (p_file == NULL)
		{
			m_nCurMediaFirstDTS = dts;
			unsigned int timestamp = (unsigned int)time(NULL);
			time_t tt = timestamp;
			struct tm *_time = localtime(&tt);
			char szTime[64] = {0,};

			//strftime(szTime, 32, "%Y%m%d%H0000", _time);//【23/06/2020  Seven】
			strftime(szTime, 32, "%Y%m%d%H%M%S", _time);
			char szStreamCacheName[512] = { 0 };
			sprintf(szStreamCacheName, "%s_%s.mp4", m_strMediaName.c_str(), szTime);
			string strFilePath = m_strWholeDir + szStreamCacheName;
			CreateMP4File((char*)strFilePath.c_str(), ZOUTFILE_FLAG_FULL);
		}
	}
	if (p_file)
	{
		if (nEncType == 1)//h265
		{
			WriteH265ToMp4File((u8*)data, dataLength, (uiFrameType == 1) ? true : false, dts, width, height);
		}
		else//h264
		{
			WriteH264ToMp4File((u8*)data, dataLength, (uiFrameType == 1) ? true : false, dts, width, height);
		}
	}
	MP4Writer_UnLock();
	return 0;
}

int EasyMP4Writer::AudioMux(const u8 *data, int dataLength, unsigned long long dts, int nSampleRate, int nChannel, int nBitsPerSample)
{
	MP4Writer_Lock();
	if (p_file&&CanWrite())
	{
		WriteAACToMp4File((u8*)data, dataLength, dts, nSampleRate, nChannel, nBitsPerSample);
	}
	MP4Writer_UnLock();
	return 0;
}

