#pragma once
#ifndef	_CT_CARM_AGENT_DEFINE_H_
#define	_CT_CARM_AGENT_DEFINE_H_

#ifdef CTAGENT_OS_WINDOWS
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#endif
#else
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CARMAgent.h"

#ifndef NCTAGENT_USE_OBFUSCATION

#define CTAGENT_GlobalInfo CTHVAR0000FE01

#endif // #ifndef NCTAGENT_USE_OBFUSCATION

///////////////////////////////////////////////////////////////////////////////
// device type
#ifndef SERVICE_CODE
#define SERVICE_CODE 0x00
#endif

#define VERSION_CODE 0x24

#define CTAGENT_VERSION (0x15000000+(SERVICE_CODE*0x10000) + VERSION_CODE)

#ifndef CTAGENT_VERSION_SUB_MESSAGE
#define CTAGENT_VERSION_SUB_MESSAGE ""
#endif
// device type
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// define
//#ifndef CTAGENT_VERSION_INFO
//	#define CTAGENT_VERSION_INFO "CT CARMAgent VERSION_15000002"
//#endif

#ifdef CTAGENT_MULTICAST_EMM_JOIN
#define CTAGENT_MULTICAST_EMM "multicast_emm_join"
#else
#define CTAGENT_MULTICAST_EMM ""
#endif

#ifdef CTAGENT_TEST_OFFLINE
#define CTAGENT_STRING_TEST_OFFLINE "offline"
#else
#define CTAGENT_STRING_TEST_OFFLINE ""
#endif

#ifndef CTAGENT_HDMI_BLOCK_CHECK_MSEC
#define CTAGENT_HDMI_BLOCK_CHECK_MSEC 2000
#endif //#ifndef CTAGENT_HDMI_BLOCK_CHECK_MSEC

#ifndef CTAGENT_EXPIRED_TIME
//	#define CTAGENT_EXPIRED_TIME "714B60824BD5E4C1D900DF6164830878FE78030DF5FD7246A61CB916D1A47AC2" // 2016-04-01T13:58:40
#define CTAGENT_EXPIRED_TIME "F52CD1EB8C2EFCA63E399729E92C7A7F463F3FB85257360453189E70878007A3" // 2016-04-01T14:16:33
#endif

#ifdef IOS_DEVICE
#define CTCOM_AGENT_BUILD_ENVIRONMENT_ID "B60C137929DB6AF1D3A18010B428C2AF43DFB75CCEE935E3ABE88BF7BC3EB053"
#elif ANDROID_DEVICE
#define CTCOM_AGENT_BUILD_ENVIRONMENT_ID "B60C137929DB6AF1D3A18010B428C2AF43DFB75CCEE935E3ABE88BF7BC3EB053"
#endif

#ifdef CTAGENT_OS_WINDOWS
#define snprintf _snprintf
#define vsnprintf _vsnprintf
typedef SOCKET   CT_SOCKET;
#else
typedef int   CT_SOCKET;
#endif

#define CTAGENT_MAX_OLDVERSION_ERROR_STRING_LENGTH 2048

#ifndef NULL
#define NULL 0
#endif

#define CTAGENT_URL_PROTOCOL_PREFIX "CTLP://"
#define CTAGENT_MAX_CONTENT_ID_LENGTH 512

#ifdef TYPEDEF_TIME_T
//typedef int time_t;
#endif

#ifdef CTAGENT_OS_WINDOWS
#define THREAD_PROCESS DWORD WINAPI
typedef HANDLE CTAGENT_THREAD;
typedef CRITICAL_SECTION CTAGENT_MUTEX;
typedef void *CTAGENT_SIGNAL;
#else
#define THREAD_PROCESS void *
typedef pthread_t CTAGENT_THREAD;
typedef pthread_mutex_t CTAGENT_MUTEX;
typedef sem_t CTAGENT_SIGNAL;
#endif

#ifdef CTAGENT_TEST_OFFLINE
#define CTAGENT_DUMMY_DESCRIPTOR 5 
#endif

#ifndef CTAGENT_OS_WINDOWS
#define TRACE fprintf
#else
#ifndef TRACE
#define TRACE CTAGENT_OLD_TRACE
#endif
#endif

#define CTAGENT_SOCKET_WAIT_TIME_MS	10000
#define CTAGENT_SOCKET_RETRY_COUNT 3
#define CTAGENT_TS_DECRYPT_LOG_CYCLE_COUNT 1000000
#define CTAGENT_CARMAGENT_LOADER_ERROR_CODE_DEFAULT 0

#define CTAGENT_MEMM_CACHE_USE_FLAG_DEFAULT CTAGENT_FALSE
#define CTAGENT_MEMM_CACHE_VALID_TIME_SEC_DEFAULT 604800

#define CTAGENT_CHANNEL_LICENSE_USE_FLAG_DEFAULT CTAGENT_TRUE
#define CTAGENT_LICENSE_MANAGER_USE_FLAG_DEFAULT CTAGENT_TRUE
#define CTAGENT_CHANNEL_PROTOCOL_CONFIG_REQ_USE_FLAG_DEFAULT CTAGENT_TRUE
#define CTAGENT_CERTIFICATE_REMOVE_FLAG_DEFAULT CTAGENT_FALSE
#define CTAGENT_TIME_CHECK_ENABLE_FLAG_DEFAULT CTAGENT_FALSE

#define CTAGENT_HE_CHECK_TIMEOUT_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_MEMM_CACHE_USE_FLAG_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_OFFLINE_MODE_USE_FLAG_DEFAULT 1
#define CTAGENT_HE_SOCKET_RETRY_COUNT_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_SOCKET_TIME_OUT_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_DECRYPT_LOG_COUNT_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_VERIFY_AGENT_FLAG_DEFAULT 0
#define CTAGENT_HE_VERIFY_PLAYER_FLAG_DEFAULT 0
//#define CTAGENT_HE_CONTENT_FORMAT_COUNT_DEFAULT 0xFFFFFFFF
#define CTAGENT_HE_CONTENT_FORMAT_COUNT_DEFAULT 0
#define CTAGENT_LOG_DIRECTORY_MANAGING_SIZE_DEFAULT 0
//#define CTAGENT_LOG_DIRECTORY_MANAGING_TIME_SEC 3600
#define CTAGENT_LOG_DIRECTORY_MANAGING_TIME_SEC 10

#define CTAGENT_AUTHENTICATION_FLAG_DEFAULT 0
#define CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE 1024
#define CTAGENT_AUTHENTICATION_TOKEN_TIME_SIZE 16
#define CTAGENT_AUTHENTICATION_TOKEN_TIME_EXPIRED_SEC 60
#define CTAGENT_AUTHENTICATION_TIME_EXPIRED_SEC 3600

#define CTAGENT_TS_PTS_ONE_SEC (90000)
#define CTAGENT_AUDIO_TS_PTS_TERM_DEFAULT (0.5 * CTAGENT_TS_PTS_ONE_SEC)
#define CTAGENT_AUDIO_TS_PTS_VOD_MIN_DEFAULT (5* CTAGENT_TS_PTS_ONE_SEC)
#define CTAGENT_AUDIO_TS_PTS_VOD_MAX_DEFAULT (5* CTAGENT_TS_PTS_ONE_SEC)

// /define
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// struct
#pragma pack (push, 1)

// Channel Preview
//typedef void(*CARM_FUNCP_ChannelPreviewCallback)(BYTE *ConID,					// 채널 맛보기 채널의 Content ID
//												 unsigned int ConIDSize,		// 채널 맛보기 채널의 Content ID의 크기
//												 int lPreviewSec);				// 채널 맛보기를 시청한 시간(초)

typedef struct _CTAGENT_PACKAGED_PREVIEW_INFO_
{
	int		ChannelPreviewUseFlag;
	CARM_FUNCP_ChannelPreviewCallback	FUNCP_PreviewCallback;
	int		PreviewCallbackFlag;
	int		PreviewCallbackTimeSec;
	int		PreviewCallbackTimeMSec;
	int		ChannelPreviewTimeSec;
	int		ChannelPreviewTimeMSec;
	int		PlayerChannelPreviewTimeSec;
	int		PlayerChannelPreviewTimeMSec;
	unsigned int 	ChannelPreviewWatchingTimeMs;
	struct timeval	ChannelPreviewOldTimeTv;
} CTAGENT_PACKAGED_PREVIEW_INFO;

typedef struct _CTAGENT_PREVIEW_INFO_
{
	int		PreviewUseFlag;
	CARM_FUNCP_PreviewCallback	FUNCP_PreviewCallback;
	int		PreviewCallbackFlag;
	int		PreviewCallbackTimeSec;			// 미리보기 종료 몇 초전에 Callback을 호출
	int		PreviewCallbackTimeMSec;
	int		PreviewTimeSec;					// 미리보기 허용 시간
	int		PreviewTimeMSec;				// 미리보기 허용 시간
	int		PlayerPreviewTimeSec;
	int		PlayerPreviewTimeMSec;
	unsigned int 	PreviewWatchingTimeMs;  // 미리보기 시청시간 + 미리보기 정지시간
	struct timeval	PreviewOldTimeTv;		// 미리보기를 시작한 시각
	int		PreviewPauseTimeMSec;
	int		AudioPtsUseFlag;
	int		AudioStartPts;
	int		AudioPtsLastCheckReturn;
	int		AudioPtsMin;
	int		AudioPtsMax;
	time_t	AudioPtsCheckTime;
} CTAGENT_PREVIEW_INFO;

typedef struct _CTAGENT_LICENSE_USE_INFO_
{
	int		ChannelLicenseUseFlag;
	int		LicenseManagerUseFlag;
} CTAGENT_LICENSE_USE_INFO;

typedef struct _CTAGENT_PROTOCOL_USE_INFO_
{
	int		ConfigReqUseFlag;
} CTAGENT_PROTOCOL_USE_INFO;

typedef struct _CTAGENT_CERTIFICATE_USE_INFO_
{
	int		CertificateRemoveFlag;
} CTAGENT_CERTIFICATE_USE_INFO;

typedef struct _CTAGENT_TIME_COMPARE_USE_INFO_
{
	int		TimeCompareEnableFlag;
	int		TimeRange;
} CTAGENT_TIME_COMPARE_USE_INFO;

typedef struct _CTAGENT_DEVICE_CONTROL_INFO_
{
	unsigned int	CheckTimeout;
	char			EMMCacheFlag;
	char			OfflineOnFlag;
	unsigned int	SocketRetryCount;
	unsigned int	SocketTimeoutMs;
	unsigned int	DecryptLogCount;
	unsigned char	VerifyAgentFlag;
	unsigned char	VerifyPlayerFlag;
	unsigned int	ContentFormatCount;
	unsigned int	ContentFormatList[100];
	unsigned int	LogDirManagingSize;
	int				LastLogDirManagingTime;
	int             PollTimeOut;
} CTAGENT_DEVICE_CONTROL_INFO;

typedef struct
{
	int UseFlag;

	// TokenAlpha
	int PathSize;
	char EncryptedFilePath[CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE];
	char FilePath[CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE];
	unsigned char Hash[20];

	// TokenBeta
	int TokenSize;
	char EncryptedToken[CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE];
	//char Token[CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE];
} CTAGENT_AUTHENTICATE_TOKEN_INFO;

typedef struct
{
	char *AuthenticationServerUrl;
	char AuthenticationServerStrIP[20];
	unsigned int AuthenticationServerIP;
	unsigned short AuthenticationServerPort;

	int AuthenticationFlag;
	int AuthenticationUsedFlag;

	CTAGENT_AUTHENTICATE_TOKEN_INFO AgentTokenInfo;

	// TokenGamma
	unsigned int	AuthenticateTypeTokenSize;	// AuthenticateTypeToken Size
	char 			AuthenticateTypeToken[CTAGENT_AUTHENTICATION_TOKEN_MAX_SIZE];		// AuthenticateTypeToken

	CTAGENT_AUTHENTICATE_TOKEN_INFO ApplicationTokenInfo;

	int	PlayerTokenCount;				// PlayerToken개수
	CTAGENT_AUTHENTICATE_TOKEN_INFO PlayerTokenInfo[10];
} CTAGENT_AUTHENTICATE_INFO;

typedef struct
{
	char BuildEnvironment[50];
} CTAGENT_AGENT_INFO;

/// \brief Callback log 사용
/// UseFlag가 true인 경우 callback을 사용함
/// \author kohen_i
/// \date 2019-08-29
typedef struct
{
	int UseFlag;
	CTAGENT_CALLBACK_PARAM LogCallBackParam;
} CTAGENT_CALLBACK_LOG_INFO;

typedef struct _CTAGENT_GLOBAL_INFO_
{
	CTAGENT_AGENT_INFO AgentInfo;
	CTAGENT_PACKAGED_PREVIEW_INFO PackagedPreviewInfo;
	CTAGENT_LICENSE_USE_INFO LicenseUseInfo;
	CTAGENT_PROTOCOL_USE_INFO ProtocolUseInfo;
	CTAGENT_CERTIFICATE_USE_INFO CertificateUseInfo;
	CTAGENT_PREVIEW_INFO VodPreviewInfo;
	CTAGENT_TIME_COMPARE_USE_INFO TimeCompareUseInfo;
	CTAGENT_DEVICE_CONTROL_INFO DeviceControlInfo;
	CTAGENT_AUTHENTICATE_INFO AuthenticationInfo;
	CTAGENT_CALLBACK_LOG_INFO CallbackLogInfo;
} CTAGENT_GLOBAL_INFO;

#pragma pack(pop)
// struct
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// variable
extern CTAGENT_GLOBAL_INFO CTAGENT_GlobalInfo;
// variable
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// function

// function
///////////////////////////////////////////////////////////////////////////////

#endif


