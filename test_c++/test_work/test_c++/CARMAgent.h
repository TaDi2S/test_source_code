#pragma once
#include"header.h"
#include "CARMAgent_error.h"
#ifdef WIN32
#ifdef CTAGENT_EXPORT
#include <windows.h>
#define CARMAGENT_EXPORT __declspec(dllexport)
#else
#define CARMAGENT_EXPORT
#endif
#endif

#ifndef WIN32
#define CARMAGENT_EXPORT
typedef unsigned char BYTE;
#endif

// function return value //
#define CTRET_SUCCESS 1
#define CTRET_FAIL 0

#define CTRET_YES 1
#define CTRET_NO 0

#define CTAGENT_TRUE 1
#define CTAGENT_FALSE 0

// content type value //
#define CTAGENT_CONTENT_TYPE_CHANNEL 1
#define CTAGENT_CONTENT_TYPE_VOD 2
#define CTAGENT_CONTENT_TYPE_VOD_PREVIEW 3
#define CTAGENT_CONTENT_TYPE_CHANNEL_PREVIEW 4
#define CTAGENT_CONTENT_TYPE_CHANNEL_PACKAGE_BASED_PREVIEW 5
#define CTAGENT_CONTENT_TYPE_H264_ES_CHANNEL 6
#define CTAGENT_CONTENT_TYPE_H264_ES_VOD 7

// content encrypt algorithm //
#define	CTAGENT_CONTENT_FORMAT_MPEG2_TS_DEFAULT "00000000"
#define CTAGENT_CONTENT_FORMAT_MPEG2_TS "00010100"
#define CTAGENT_CONTENT_FORMAT_MPEG4 "00020200"
#define CTAGENT_CONTENT_FORMAT_H264_MPEG2_TS "00030100"
#define CTAGENT_CONTENT_FORMAT_H264_MPEG4 "00030201"
#define CTAGENT_CONTENT_FORMAT_AES128 "00000405"
#define CTAGENT_CONTENT_FORMAT_H265_MPEG2_TS "00050100"
#define CTAGENT_CONTENT_FORMAT_H265_MPEG4 "00050200"
#define CTAGENT_CONTENT_FORMAT_H265_MPEG2_TS_I "00050106"
#define CTAGENT_CONTENT_FORMAT_H265_MPEG4_I "00050206"
#define CTAGENT_CONTENT_FORMAT_H265_MPEG2_TS_I_2KEY "00050107"
#define CTAGENT_CONTENT_FORMAT_H264_MPEG2_TS_KT "00030108"
#define	CTAGENT_CONTENT_FORMAT_H264_MPEG2_TS_DOLBY	"00030110"
#define	CTAGENT_CONTENT_FORMAT_H265_MPEG2_TS_DOLBY	"00050110"

#define CTAGENT_DATATYPE_VIDEO_DATA 1
#define CTAGENT_DATATYPE_AUDIO_DATA 2

#define CTAGENT_MAX_TOKEN_SIZE 1024

// CARM Descriptor Command //

#define CTAGENT_DESCRIPTOR_CMD_VOD_PREVIEW_PAUSE 0x00000001 
// VOD_PREVIEW로 Open된 CARM Descriptor에 미리보기 시간이 더 이상 진행 안되게 Pause요청. CT_Decrypt()호출시 다시 미리보기 시간이 진행된다.
// Parameter : NULL

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_VIDEO_PID 0x00000002
// CT_OpenCARM() 후 Stream에서 PMT 찾기전 Video PID를 먼저 설정한다.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short 변수의 주소 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_AUDIO_PID 0x00000003
// CT_OpenCARM() 후 Stream에서 PMT 찾기전 Audio PID를 먼저 설정한다.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short 변수의 주소 

#define CTAGENT_DESCRIPTOR_CMD_SET_ECM_PID 0x00000004
// CT_OpenCARM() 후 Stream에서 PMT 찾기전 ECM PID를 먼저 설정한다.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short 변수의 주소 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_PREVIEW_START_SEC 0x00000005
// CT_OpenCARM() 후 VOD 미리보기 시작 시간을 설정한다.
// 해당 옵션 설정하면 디바이스 시간 기준의 미리보기 동작은 동작 하지 않는다.
// 해당 옵션 설정하면 스트림 시간 기준의 미리보기 동작이 동작 한다.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : int 변수의 주소 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_PREVIEW_PTS_RESET 0x00000006
// CT_OpenCARM() 후 VOD 미리보기 시간을 reset하는 경우 설정한다.
// 스트림 시간 기준의 미리보기를 설정한 경우에만 동작한다.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ... CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : NULL

// CARM Service Site Command //

#define CTAGENT_SERVICESITE_CMD_SET_OLD_DEVICE_ID 0x10000001 
// VOD 저장 모드 사용시 구버전 라이선스와 호환이 가능하도록 구버전 형식의 Device Key를 생성하기위해 구버전 형태의 DeviceID를 세팅한다.
// Parameter : DeviceID 문자열
// 예) "01-23-45-67-89-AB"

#define CTAGENT_SERVICESITE_CMD_SET_BLOCK_HDMI 0x10000002
// HDMI 출력 차단 (Android만 지원). 기본 FALSE
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_SERVICESITE_CMD_SET_SOCKET_RETRY_COUNT 0x10000003
// Protocol 통신시 Socket 관련 에러 발생할경우 Retry를 몇번 할 것인지 설정한다. 기본 3회.
// Parameter : unsigned int 변수의 주소 (4bytes), 단위 횟수

#define CTAGENT_SERVICESITE_CMD_SET_SOCKET_WAIT_TIME 0x10000004 
// Protocol 통신시 Socket의 WaitTime을 몇 milli seconds로 줄 것인지 설정한다. 기본 10초.
// Parameter : unsigned int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_CACHING 0x10000005 
// Multicast EMM cache 사용 여부를 설정한다. 기본 FALSE
// TRUE 설정시 기본 유지 기간 604800초 (7일)
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_CACHING_VALID_TIME_SECONDS 0x10000006 
// Multicast EMM cache 사용 시 cache파일 유효 기간을 설정한다. 기본 604800초 (7일)
// 삭제됨
// Parameter : int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_SET_TS_DECRYPT_LOG_CYCLE 0x10000007 
// CT_Decrypt() 결과 로그의 출력 주기를 설정한다.(에러는 항상 출력한다.)  기본 1000000번 호출시 마다 한 번
// Parameter : int 변수의 주소 (4bytes), 주기값은 0보다 커야함, 단위 횟수
// 예) "1000000"  <- TS 1000000번 호출시 마다 로그 출력

#define CTAGENT_SERVICESITE_CMD_SET_CHANNEL_PREVIEW_CALLBACK 0x10000008
// 미리보기 종료시 메시지를 전달할 함수 포인터를 전달한다. NULL이 전달되면 동작 안한다. 기본 NULL.
// Parameter : 함수 포인터


typedef void(*CARM_FUNCP_ChannelPreviewCallback)(BYTE *ConID,					// 채널 맛보기 채널의 Content ID
	unsigned int ConIDSize,		// 채널 맛보기 채널의 Content ID의 크기
	int lPreviewSec);				// 채널 맛보기를 시청한 시간(초)

#define CTAGENT_SERVICESITE_CMD_SET_CHANNEL_PREVIEW_CALLBACK_SEC 0x10000009 
// 미리보기 종료 몇 초 전에 Callback 함수를 호출할지 설정한다. 미리보기 시간보다 크거나 같으면 동작 안함. 기본 0초.
// Parameter : int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_SET_PLAYER_CHANNEL_PREVIEW_SEC 0x1000000A 
// Player에서 미리보기 시간을 설정한다.
// DRM 에서 설정된 미리보기 시간과 Player가 설정한 미리보기 시간중 더 작은 값이 미리보기 시간으로 설정된다.
// Parameter : int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_SET_VOD_PREVIEW_CALLBACK 0x1000000B
// VOD 미리보기 종료시 메시지를 전달할 함수 포인터를 전달한다. NULL이 전달되면 동작 안한다. 기본 NULL.
// Parameter : 함수 포인터
typedef void(*CARM_FUNCP_PreviewCallback)(BYTE *ConID,					// VOD 미리보기 콘텐츠의 Content ID
	unsigned int ConIDSize,		// VOD 미리보기 콘텐츠의 Content ID의 크기
	int lPreviewSec);			// VOD 미리보기 콘텐츠를 시청한 시간(초)

#define CTAGENT_SERVICESITE_CMD_SET_AGENT_LOADER_ERROR 0x1000000C
// CARM Agent Loader에서 발생한 오류코드 설정
// Parameter : int 변수의 주소 (4bytes)

#define CTAGENT_SERVICESITE_CMD_STOP_UPDATE_KEY_THREAD 0x1000000D
// CTAGENT_UpdateKc Thread를 종료하기 위해 호출.
// Parameter : NULL

#define CTAGENT_SERVICESITE_CMD_SET_OFFLINE_AUTHINFO 0x1000000E
// offline일 때 사용되는 AuthInfo를 설정한다.
// offline용 AuthInfo가 설정 되어 있으면 GetVODLicense(), OpenVODLicense()에서 이 값으로 사용된다.

#define CTAGENT_SERVICESITE_CMD_SET_OFFLINE_CONTENT_ID 0x1000000F
// offline일 때 사용되는 Content ID를 설정한다.
// offline용 Content ID가 설정 되어 있으면 GetVODLicense(), OpenVODLicense()에서 이 값으로 사용된다.


#define CTAGENT_SERVICESITE_CMD_GET_DRM_CHANNEL_PREVIEW_SEC 0x11000001
// DRM 에서 설정된 미리보기 시간을 전달한다.
// DRM 에서 설정된 미리보기 시간과 Player가 설정한 미리보기 시간중 더 작은 값이 미리보기 시간으로 설정된다.
// Parameter : int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_GET_PLAYER_CHANNEL_PREVIEW_SEC 0x11000002
// Player가 설정한 미리보기 시간을 전달한다.
// DRM 에서 설정된 미리보기 시간과 Player가 설정한 미리보기 시간중 더 작은 값이 미리보기 시간으로 설정된다.
// Parameter : int 변수의 주소 (4bytes), 단위 초

#define CTAGENT_SERVICESITE_CMD_GET_CHANNEL_PREVIEW_INFO 0x11000003
// 특정 채널이 미리보기 시청가능한 채널인지를 확인한다.
// Parameter : PREVIEW_CHANNEL_INFO 구조체 변수 주소(4bytes)

typedef struct _PREVIEW_CHANNEL_INFO
{
	BYTE *ConID;                 // in  : ex) "233.18.145.121,55492"
	unsigned int ConIDSize;      // in  : ex) 20
	int IsPreviewChannel;        // out : CTAGENT_TRUE or CTAGENT_FALSE
} PREVIEW_CHANNEL_INFO;

#define CTAGENT_SERVICESITE_CMD_SET_LI_URL 0x11000004
// LicenseIssuer URL 주소를 설정/변경한다.
// Parameter : char 문자열 주소, LicenseIssuer URL (null terminated string)
//             ex) "CTLP://61.34.112.42:7845"

#define CTAGENT_SERVICESITE_CMD_SET_UEMM_URL 0x11000005
// UnicastEMM URL 주소를 설정/변경한다.
// Parameter : char 문자열 주소, UnicastEMM URL (null terminated string)
//             ex) "CTLP://61.34.112.42:5490"

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_URL 0x11000006
// MulticastUEMM URL 주소를 설정/변경한다.
// Parameter : char 문자열 주소, MulticastUEMM URL (null terminated string)
//             ex) "CTLP://233.18.145.129:5490"

#define CTAGENT_SERVICESITE_CMD_RELEASE_SERVICESITE 0x11000007
// SERVICE SITE에 할당된 자원을 초기화 한다.
// Parameter : NULL


// CARM Agent Info Command //
#define CTAGENT_AGENT_CMD_SET_PACKAGED_CHANNEL_PREVIEW 0x20000001 
// CARM Agent에서 Packaged channel preview 사용 할지 여부를 설정한다. 기본 FALSE
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_CHANNEL_LICENSE_USE_FLAG 0x20000002
// CARM Agent에서 Channel License 사용 할지 여부를 설정한다. 기본 TRUE
// CT_SetCARM() 사용시 반영된다.
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_PROTOCOL_CONFIG_USE_FLAG 0x20000003
// CARM Agent에서 이미 수신한 CONFIG 정보가 존재할경우에도 CONFIG 정보를 새로 얻어 오는지 여부를 설정한다. 기본 TRUE
// CT_SetCARM() 사용시 반영된다.
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_CERTIFICATE_REMOVE_FLAG 0x20000004
// CARM Agent에서 인증서를 삭제한다. 기본 FALSE
// CT_SetCARM() 사용시 반영된 후 다시 기본 값이 설정된다. (1회성 FLAG)
// Parameter : int 변수의 주소 (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_TIME_COMPARE_USE_FLAG 0x20000005
// H/E와 Agent 시간 비교 기능 사용 여부. 기본 0
// Parameter : int 변수의 주소 (4Bytes), 단위 초 (0일 경우 기능 Disable)

#define CTAGENT_AGENT_CMD_SET_AUTHENTICATION_SERVER_URL 0x20000006
// Authentication Server URL 주소를 설정/변경한다.
// Parameter : char 문자열 주소, Authentication Server URL (null terminated string)
//             ex) "CTLP://61.34.112.42:5490"

#define CTAGENT_AGENT_CMD_SET_AGENT_TOKEN 0x20000007
// Coretrust에서 발급한 CARM Agent Token을 설정한다.
// Parameter : CTAGENT_AGENT_TOKEN 구조체 변수 주소 
typedef struct
{
	char TokenAlpha[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_AGENT_TOKEN;

#define CTAGENT_AGENT_CMD_SET_APPLICATION_TOKEN 0x20000008
// Coretrust에서 발급한 Application Token을 설정한다.
// Parameter : CTAGENT_APPLICATION_TOKEN 구조체 변수 주소 
typedef struct
{
	char TokenAlpha[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenGamma[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_APPLICATION_TOKEN;

#define CTAGENT_AGENT_CMD_SET_PLAYER_TOKEN 0x20000009
// Coretrust에서 발급한 Player Token을 설정한다.
// Parameter : CTAGENT_PLAYER_TOKEN 구조체 변수 주소 
typedef struct
{
	int TokenCount; // max count is 10
	char TokenAlpha[10][CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[10][CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_PLAYER_TOKEN;

#define CTAGENT_CMD_SET_LOG_CALLBACK 0x2000000A
// .
// Parameter : CALLBACK_PARM 구조체 변수 주소 
typedef void(*CARMLogCallback_t_)(void* context, int level, const char* message);
typedef struct
{
	CARMLogCallback_t_ LogCall;    // User callback called for every message to be logged
	void *LogContext;               // Opaque pointer passed in every log callback
} CTAGENT_CALLBACK_PARAM;

#define CTAGENT_AGENT_CMD_SET_POLL_TIMEOUT 0x2000000B
// CARM Agent에서 POLL_TIMEOUT 시간을 1000ms가 아닌 다른 값으로 설정하고 싶을 때 CT_SetCARMAgentCommand의 command로 설정.
// CT_SetCARM() 사용시 반영된다.
// Parameter : 1000이외의 숫자(ms)

// export function
CARMAGENT_EXPORT int CT_SetCARM(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *AuthInfo,                // 사용자 인증을 위한 BYTE String. Service Site에 Oriented 된 Value
	unsigned int AuthInfoSize,     // 사용자 인증정보의 크기
	char *LiUrl,                   // License Issuer URL
	char *UEMMUrl,                 // Unicast EMM URL
	char *MEMMUrl,                 // Multicast EMM URL
	char *CertPath,                // CARM Agent에서 사용하는 인증서 저장 경로
	char *LogPath,                 // CARM Agent의 파일 Log 저장 경로
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_OpenCARM(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *ConID,                   // Content 식별정보
	unsigned int ConIDSize,        // Content 식별정보의 크기
	unsigned int ConType,          // Content Type. Channel or VOD content 구분자
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT void CT_CloseCARM(
	int CARMDesc);                 // CARM Agent내에 존재하는 CARM Descriptor Number

CARMAGENT_EXPORT int CT_GetVODLicense(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *ConID,                   // Content 식별정보
	unsigned int ConIDSize,        // Content 식별정보의 크기
	BYTE *LicenseBuf,              // 암호화된 VOD License가 담긴 Buffer
	unsigned int LicenseBufSize,   // License Buffer의 크기
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_OpenVODLicense(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *ConID,                   // Content 식별정보
	unsigned int ConIDSize,        // Content 식별정보의 크기
	BYTE *LicenseBuf,              // 암호화된 VOD License가 담긴 Buffer
	unsigned int LicenseBufSize,   // License Buffer의 크기
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_IsCARM(
	BYTE *ConID,                   // Content 식별정보
	unsigned int ConIDSize,        // Content 식별정보의 크기
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_Decrypt(
	int CARMDesc,                  // CARM Descriptor Number
	BYTE *Buf,                     // 복호화 해야될 Data가 담긴 Buffer 포인터
	unsigned int BufSize,          // Buffer의 Size
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_Decrypt2(
	int CARMDesc,                  // CARM Descriptor Number
	BYTE *Buf,                     // 복호화 해야될 Data가 담긴 Buffer 포인터
	unsigned int BufSize,          // Buffer의 Size
	void *Val1,                    // 복호화 시 필요한 추가 정보, 없는 경우 NULL 전달
	void *Val2,                    // 복호화 시 필요한 추가 정보, 없는 경우 NULL 전달
	void *Val3,                    // 복호화 시 필요한 추가 정보, 없는 경우 NULL 전달
	void *Val4,                    // 복호화 시 필요한 추가 정보, 없는 경우 NULL 전달
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_PutEMM(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *Buf,                     // EMM이 담길 버퍼 포인터
	unsigned int BufSize,          // EMM이 담길 버퍼의 크기
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT char *CT_GetCARMVer();

CARMAGENT_EXPORT int CT_GetValidChannel(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *ConID,                   // Content 식별정보
	unsigned int ConIDSize,        // Content 식별정보의 크기
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_SetDescriptorCommand(
	int CARMDesc,                  // CARM Descriptor Number
	unsigned int Command,          // CARM Descriptor에 전달할 명령
	BYTE *Parameter,               // CARM Descriptor에 전달할 Parameter
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_SetServiceSiteCommand(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	unsigned int Command,          // CARM Descriptor에 전달할 명령
	BYTE *Parameter,               // CARM Descriptor에 전달할 Parameter
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_SetCARMAgentCommand(
	unsigned int Command,          // CARM Agent 전달할 명령
	BYTE *Parameter,               // CARM Agent 전달할 Parameter
	unsigned int *ErrCode);        // Error Code return 용

CARMAGENT_EXPORT int CT_GetServiceSiteCommand(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	unsigned int Command,          // CARM Descriptor에 전달할 명령
	BYTE *Parameter,               // CARM Descriptor에 전달할 Parameter
	unsigned int *ErrCode);        // Error Code return 용