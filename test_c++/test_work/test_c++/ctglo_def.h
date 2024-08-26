#pragma once
#ifndef _CTGLO_DEF_H_
#define _CTGLO_DEF_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctcom_def.h"
//#include "ctglo_err.h"
//#include "csa/csa.h"

// 통합 CARM Agent
// {
//#include "CARMLicense.h"
//#include "hevc_parser/CARMHevcNalParser.h"
// }

#ifdef H264ES_DECRYPT
#include "4d_replay/H264ESCryptoAlgorithm.h"
#endif


#ifdef IOS_DEVICE
#include <math.h>
#endif

#ifdef WM_INSERT
#include "FPAgent.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

	//																						
	// 변수 타입 정의			//////////////////////////////////////////////////////////////
	//
#ifndef _DEF_VARTYPE	// mgsong 20210407
	typedef unsigned int       DWORD;
	typedef unsigned int       ULONG;
	typedef unsigned int       *PDWORD;
	typedef unsigned int       *PULONG;
	typedef int                 BOOL;
	typedef int                 *PBOOL;
	// typedef int                 bool;
	typedef int                 *pbool;
	//typedef unsigned char       BYTE;
	typedef unsigned char       *PBYTE;
	typedef char       			CHAR;
	typedef char       			*PCHAR;
	typedef unsigned short      WORD;
	typedef unsigned short      *PWORD;
#endif




	//																						
	// ECM structure				//////////////////////////////////////////////////////////////
	//
	typedef struct _CORE_PMT_DATA
	{
		unsigned char	chAccessCriteria;				/* 이벤트, 재난 방송(0xFF) */
		unsigned char	chKCFlag;						/* Service Key Version */
		unsigned short 	usChannelPort;					/* Channel SubID */
		unsigned int	ulChannelIP;					/* Channel ID */
		//unsigned int 	ulCAServiceID;					/* CA Service ID */
		int	 		tCurrentTime;					/* PMT에 들어갈 현재 시간 */
		unsigned short 	usCWVersion;					/* Control Wordㅓ의 Version */
		unsigned char	ucPVRFlag;
		unsigned char	chPadding;
		unsigned char	ucEcmSign[16];
		unsigned char	chControlWord[2][16];
		unsigned int	ulAreaCodeHH;
		unsigned int	ulAreaCodeHL;
		unsigned int	ulAreaCodeLH;
		unsigned int	ulAreaCodeLL;
#ifdef PRE_VIEW_MODE
		unsigned short	usPreViewTime;
		unsigned char	ucPreViewKey[16];
#endif
		unsigned char	ucCheckSum;
	}CORE_PMT_DATA, *PCORE_PMT_DATA;


	typedef struct _CORE_PMT_DATA_V1
	{
		unsigned char	chAccessCriteria;				/* 이벤트, 재난 방송(0xFF) */
		unsigned char	chKCFlag;						/* Service Key Version */
		unsigned short 	usChannelPort;					/* Channel SubID */
		unsigned int	ulChannelIP;					/* Channel ID */
		//unsigned int 	ulCAServiceID;					/* CA Service ID */
		int	 		tCurrentTime;					/* PMT에 들어갈 현재 시간 */
		unsigned short 	usCWVersion;					/* Control Word의 Version */
		unsigned char	ucPVRFlag;
		unsigned char	chPadding;
		unsigned char	ucEcmSign[16];
		unsigned char	chControlWord[2][16];
		unsigned int	ulAreaCodeHH;
		unsigned int	ulAreaCodeHL;
		unsigned int	ulAreaCodeLH;
		unsigned int	ulAreaCodeLL;
		unsigned char	ucCheckSum;
	}CORE_PMT_DATA_V1, *PCORE_PMT_DATA_V1;

#pragma pack(push, 1)
	typedef struct _CORE_PMT_DATA_V1_FIX
	{
		unsigned char	chAccessCriteria;				/* 이벤트, 재난 방송(0xFF) */
		unsigned char	chKCFlag;						/* Service Key Version */
		unsigned short 	usChannelPort;					/* Channel SubID */
		unsigned int	ulChannelIP;					/* Channel ID */
		//unsigned int 	ulCAServiceID;					/* CA Service ID */
		int	 			tCurrentTime;					/* PMT에 들어갈 현재 시간 */
		unsigned short 	usCWVersion;					/* Control Word의 Version */
		unsigned char	ucPVRFlag;
		unsigned char	chPadding;
		unsigned char	ucEcmSign[16];
		unsigned char	chControlWord[2][16];
		unsigned int	ulAreaCodeHH;
		unsigned int	ulAreaCodeHL;
		unsigned int	ulAreaCodeLH;
		unsigned int	ulAreaCodeLL;
		unsigned char	ucCheckSum;
	}CORE_PMT_DATA_V1_FIX, *PCORE_PMT_DATA_V1_FIX;
#pragma pack(pop)
	//																						
	// Package EMM structure				//////////////////////////////////////////////////////////////
	//
	/* Write By NODONG */
	/* 2007-11-05 */
	/* For Package EMM */
#define MAX_CENCINF					20


#ifdef CAMESSAGE_CHANNEL
	typedef struct _CA_EMM_HEADER_
	{
		unsigned char		ucSuperCasId[2];
		unsigned short		usEmmSize;
		unsigned char 		ucEmmType;
		unsigned char		ucEmmVersion;
		unsigned char 		ucCheckSum;
		unsigned char		ucNonce;
	}CA_EMM_HEADER;
#endif

	typedef struct _EMM_HEADER_
	{
		unsigned char 	ucEmmType;
		unsigned char	ucEmmVersion;
		unsigned char	ucEmmSize;
		unsigned char	ucCheckSum;
	}EMM_HEADER;

#define CARM_EMM_VERSION_1 1 
#define CARM_EMM_VERSION_2 2
#define COMMAND_EMM	0
#define PACKAGE_EMM	1

	//																						
	// License Infomation structure		//////////////////////////////////////////////////////////////
	//
#ifdef VOD_LICENSE_SAVE
//typedef struct _CTLIC_SAVE_CONTENT_INFO
//{
//	int			lLicStartTime;
//	int			lLicEndTime;
//	int			lPlayCount;
//	unsigned int	ulScrambleLevel;
//	unsigned int	ulContentFormat;
//	unsigned char	ucKC[16];
//	unsigned char	szContentID[256];
//	unsigned char	ucEncryptType;
//	char			szPadding[11];
//}CTLIC_SAVE_CONTENT_INFO, *PCTLIC_SAVE_CONTENT_INFO;


	typedef struct _CTLIC_SAVE_CONTENT_INFO
	{
		int			lLicStartTime;
		int			lLicEndTime;
		int			lPlayCount;
		unsigned int	ulScrambleLevel;
		unsigned int	ulContentFormat;
		unsigned char	ucKC[16]; // video key
		unsigned char	szContentID[256];
		unsigned char	ucEncryptType;
		unsigned char	ucKC2[16]; // audio key
		char			szPadding[11];
	}CTLIC_SAVE_CONTENT_INFO, *PCTLIC_SAVE_CONTENT_INFO;
#endif

	//typedef struct _CTLIC_CONTENT_INFO
	//{
	//	char			chKC[17];
	//	unsigned char	szContentID[256];	
	//	unsigned int	ulScrambleLevel;
	//	unsigned int	ulContentFormat;
	//	unsigned char	ucEncryptType;						// 암호화 알고리즘 타입 
	//	unsigned short	usLicenseSaveTerm;
	//	char			szPadding[4];
	//}CTLIC_CONTENT_INFO, *PCTLIC_CONTENT_INFO;



	/* [8/28/2008 NODONG] */
	/*					  */
	// {
	typedef struct _VALID_CHANNEL_LIST
	{
		unsigned int	ulChannelIp;
		unsigned short	usChannelPort;
	}VALID_CHANNEL_LIST;
	// }


	// }

	//																						
	// Define							//////////////////////////////////////////////////////////////
	//
	/* Write By NODONG */
	/* 2007-10-12 */
#define					RECEIVE_SOCKET_TIME_WAIT				10000

#define					MAX_TRANS_BUFFER						1024
#define					SIZEOF_PMT_DATA							sizeof(CORE_PMT_DATA)
#define					SIZEOF_PMT_DATA_V1						sizeof(CORE_PMT_DATA_V1)
#define					SIZEOF_PMT_DATA_V1_FIX					sizeof(CORE_PMT_DATA_V1_FIX)
#define					CTGLO_MAXLIVECHANNEL					200
#define					CTGLO_MAXEMMLIST						500
#define					CTGLO_EXCEPTION_HANDLE_COUNT			2
#define					CTGLO_TIME_OUT_HANDLE_COUNT				3
#define					MAX_PACKAGE_EMM_COUNT					2
// Content Type
#define					CTGLO_CUR_CONTENT_UNDEFINED				0
#define					CTGLO_CUR_CONTENT_NORMAL				1
#define					CTGLO_CUR_CONTENT_LIVE					2

// Content Format 구분
#ifdef USE_IPTV30_CONTENT_FORMAT

#define					CTGLO_CONTENT_TYPE_MPEG2_TS_DEFAULT		0x00000000 // 사용 안함
#define					CTGLO_CONTENT_TYPE_MPEG2_TS				0x00000001 // 사용 안함
#define					CTGLO_CONTENT_TYPE_MPEG4				0x02010100

#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS		0x01010100
#define					CTGLO_CONTENT_TYPE_H264_MPEG4			0x00000002 // 사용 안함
#define					CTGLO_CONTENT_TYPE_AES128				0x01010110

#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS		0x00000003 // 사용 안함
#define					CTGLO_CONTENT_TYPE_H265_MPEG4			0x00000004 // 사용 안함
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS_I		0x00000005 // 사용 안함

#define					CTGLO_CONTENT_TYPE_H265_MPEG4_I			0x02020100
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS_I_2KEY	0x01020100

#define					CTGLO_CONTENT_TYPE_H264_MPEG4_WIDEVINE	0x00000006 // 사용 안함
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS_KT		0x00000007 // 사용 안함
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS_CSA	0x00000008 // 사용 안함

#else

#define					CTGLO_CONTENT_TYPE_MPEG2_TS_DEFAULT		0x00000000
#define					CTGLO_CONTENT_TYPE_MPEG2_TS				0x00010100
#define					CTGLO_CONTENT_TYPE_MPEG4				0x00020200
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS		0x00030100
#define					CTGLO_CONTENT_TYPE_H264_MPEG4			0x00030201
#define					CTGLO_CONTENT_TYPE_AES128				0x00000405
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS		0x00050100
#define					CTGLO_CONTENT_TYPE_H265_MPEG4			0x00050200
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS_I		0x00050106
#define					CTGLO_CONTENT_TYPE_H265_MPEG4_I			0x00050206
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS_I_2KEY	0x00050107
#define					CTGLO_CONTENT_TYPE_H264_MPEG4_WIDEVINE	0x00030510
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS_KT		0x00030108
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS_CSA	0x00030109
#define		            CTGLO_CONTENT_TYPE_H264_MP4_4DREPLAY 	0x00030306
#define					CTGLO_CONTENT_TYPE_H264_MPEG2_TS_DOLBY	0x00030110
#define					CTGLO_CONTENT_TYPE_H265_MPEG2_TS_DOLBY	0x00050110

#endif


// Multicasting License Type
#define					CTGLO_LIVE_CASERVICE_LIC_TYPE			0
#define					CTGLO_LIVE_LIC_TYPE						1

// Function Type	
#define					CTGLO_INIT_FUNTCTION_TYPE				0
#define					CTGLO_UPDATE_FUNTCTION_TYPE				1

#define					KCDSA_PK_KEY_LEN						512
//#define					PUB_KEY_LEN								(188 + 1 + 3)	
//#define					PRI_KEY_LEN								1024
#define					SYMMETRY_KEY_LEN						16	

#ifndef CTCOM_OS_WINCE
#define					LOGFILE									"./Corecrypt_Log.txt"
#else
#define					LOGFILE									"\\hard disk\\Corecrypt_Log.txt"
#endif

#define					LOGFILENAME								"log.txt"
#define					CONFIGFILENAME							"\\hard disk\\config.txt"

/* [4/21/2008 NODONG] */
/* CAMessage Channel Mode 추가 */
// {
#ifdef CAMESSAGE_CHANNEL
#define					RECV_BUF_SIZE							2048
#endif
// }

/* [3/11/2009 NODONG] */
/* Function Call Status */
// {
	typedef enum
	{
		CALLED = 0x00000001,
		PARAMETER_SET = 0x00000002,
		USER_INFO_SET = 0x00000004,
		SOCKET_CONNECT = 0x00000008,
		GET_SERV_CERT = 0x00000010,
		GET_AGNT_CERT = 0x00000020,
		GET_CONFIG = 0x00000040,
		GET_SECRET = 0x00000080,
		GET_LICENSE = 0x00000100,
		LICENSE_VALID = 0x00000200
	};

	// }

	// CAS
#define					WHOLE_MESSAGE_ENC_KEY		"CORECRYPTFORIPTV"
#define					COMMAND_EMM_ENC_SIZE		64
#define					MAX_COMMAND					2
#define					COMMAND_EMM_HISTORY_COUNT	10
#define					ECM_SIGN					"CORETRUST-CARMTV" 

//																						
// Declaration Function				//////////////////////////////////////////////////////////////
//																						
	extern int						CTAGENT_OLD_CTGLO_strncmp_nc(char *src, char *dest, int n);
	extern int						CTAGENT_OLD_CTGLO_strcmp_nc(char *src, char *dest);
	extern int						CTAGENT_OLD_CTGLO_HexToBin(char *strHex, char *strBin, int size);
	extern int						CTAGENT_OLD_CTGLO_BinToHex(char *strBin, char *strHex, int size);
	extern DWORD					CTAGENT_OLD_CTGLO_Get1Bit(unsigned char * chBuffer, DWORD lBufPos, int Pos);
	extern DWORD					CTAGENT_OLD_CTGLO_Get3Bytes(unsigned char * chBuffer, DWORD lBufPos);
	extern DWORD					CTAGENT_OLD_CTGLO_GetBytes(unsigned char * chBuffer, DWORD lBufPos, unsigned char * szGetBuf2, int n);

#ifdef DEBUG_LOG
	extern void				CTAGENT_OLD_CTGLO_DebugLog(int CARMIndex, unsigned int level, const char *pLogString, ...);
	extern void				CTAGENT_OLD_CTGLO_DebugLog_s(int CARMIndex, unsigned int level, const char *pLogString, ...);
#endif

	/* Modifyed By NODONG */
	// 서버로에서 내려오는 파일 로그 플레그를 이용해서 로그 남기게 .....
	//#ifdef HISTORY_LOG
	extern void				CTAGENT_OLD_CTGLO_HistoryLog(int CARMIndex, const char *pLogString, ...);
	extern void CTAGENT_OLD_CTGLO_HistoryLog_s(int ServiceSiteIdIndex, const char *pLogString, ...);
	extern void				CTAGENT_OLD_CTGLO_HistoryLogManager(int CARMIndex);
#ifdef DUMP_TS_BUFFER
	extern void				CTAGENT_OLD_CTGLO_DumpData(unsigned char *pData, unsigned int nDataLen, int err);
#endif
	//#endif

#ifdef CTAGENT_TEST_OFFLINE
	extern void				CTAGENT_OLD_CTGLO_InterfaceLog(int CARMIndex, const char *pLogString, ...);
#endif

	extern unsigned int	CTAGENT_OLD_CTGLO_ActiveChannel(int channel);
	extern unsigned int	CTAGENT_OLD_CTGLO_DeleteChannel(int channel);
	extern unsigned int	CTAGENT_OLD_CTGLO_CreateChannel(int *channel);

	/* [4/21/2008 NODONG] */
	/* RC4 ECM, EMM 암호화 */
	// {
	void CTAGENT_OLD_CTCIF_DSCRAMBLE_DATA(unsigned char *buffer, int size, unsigned char seed);
	void CTAGENT_OLD_CTCIF_SCRAMBLE_DATA(unsigned char *buffer, int size, unsigned char seed);
	// }

	//																						
	// Declaration Variable				//////////////////////////////////////////////////////////////
	//

	/* [7/21/2008 NODONG] */
	/* 멀티로 복호화 하기 위한 Global 모음(PVR 때문에) */
	// {
#define MAX_PVR_CHANNEL	2

/*
CURKEYINST				CTGLO_PVR_ServiceKey[MAX_PVR_CHANNEL], CTGLO_PVR_CurKeyInst[MAX_PVR_CHANNEL], CTGLO_PVR_NextKeyInst[MAX_PVR_CHANNEL], CTGLO_PVR_ContentDecryptKey[MAX_PVR_CHANNEL];
bool					CTGLO_PVR_Lic_Valid[MAX_PVR_CHANNEL];

// 일단 뽑아 낸거는 다음과 같고 이를 구조체로 묶어서 사용 하자
unsigned int			CTGLO_PVR_CurEncryptType[MAX_PVR_CHANNEL];
unsigned int			CTGLO_PVR_CurContentType[MAX_PVR_CHANNEL];
unsigned int			CTGLO_PVR_ContentFormat[MAX_PVR_CHANNEL];
unsigned short			CTGLO_PVR_CWVersion[MAX_PVR_CHANNEL];
unsigned char			CTGLO_PVR_Before_State_AC[MAX_PVR_CHANNEL];
char					CTGLO_PVR_chVideoPid[MAX_PVR_CHANNEL][2];
char					CTGLO_PVR_chAudioPid[MAX_PVR_CHANNEL][2];

#ifdef CAMESSAGE_CHANNEL
unsigned char			CTGLO_PVR_chpPackageEmmIndex[MAX_PVR_CHANNEL];
#endif


//밑에 두 개는 필요 없는 듯.....
PACKAGE_EMM_INFO		CTGLO_stDualPVRPackageEMM[MAX_PVR_CHANNEL];
CTLIC_CHANNEL_INFO		*CTGLO_pDualPVRCurLiveChannel[MAX_PVR_CHANNEL];
// }
*/



#ifdef __cplusplus
}
#endif

#endif
