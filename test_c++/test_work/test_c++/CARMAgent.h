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
// VOD_PREVIEW�� Open�� CARM Descriptor�� �̸����� �ð��� �� �̻� ���� �ȵǰ� Pause��û. CT_Decrypt()ȣ��� �ٽ� �̸����� �ð��� ����ȴ�.
// Parameter : NULL

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_VIDEO_PID 0x00000002
// CT_OpenCARM() �� Stream���� PMT ã���� Video PID�� ���� �����Ѵ�.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short ������ �ּ� 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_AUDIO_PID 0x00000003
// CT_OpenCARM() �� Stream���� PMT ã���� Audio PID�� ���� �����Ѵ�.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short ������ �ּ� 

#define CTAGENT_DESCRIPTOR_CMD_SET_ECM_PID 0x00000004
// CT_OpenCARM() �� Stream���� PMT ã���� ECM PID�� ���� �����Ѵ�.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : unsigned short ������ �ּ� 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_PREVIEW_START_SEC 0x00000005
// CT_OpenCARM() �� VOD �̸����� ���� �ð��� �����Ѵ�.
// �ش� �ɼ� �����ϸ� ����̽� �ð� ������ �̸����� ������ ���� ���� �ʴ´�.
// �ش� �ɼ� �����ϸ� ��Ʈ�� �ð� ������ �̸����� ������ ���� �Ѵ�.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : int ������ �ּ� 

#define CTAGENT_DESCRIPTOR_CMD_SET_VOD_PREVIEW_PTS_RESET 0x00000006
// CT_OpenCARM() �� VOD �̸����� �ð��� reset�ϴ� ��� �����Ѵ�.
// ��Ʈ�� �ð� ������ �̸����⸦ ������ ��쿡�� �����Ѵ�.
// CT_OpenCARM() -> CT_SetDescriptorCommand() -> CT_Decrypt() ... CT_SetDescriptorCommand() -> CT_Decrypt() ...
// Parameter : NULL

// CARM Service Site Command //

#define CTAGENT_SERVICESITE_CMD_SET_OLD_DEVICE_ID 0x10000001 
// VOD ���� ��� ���� ������ ���̼����� ȣȯ�� �����ϵ��� ������ ������ Device Key�� �����ϱ����� ������ ������ DeviceID�� �����Ѵ�.
// Parameter : DeviceID ���ڿ�
// ��) "01-23-45-67-89-AB"

#define CTAGENT_SERVICESITE_CMD_SET_BLOCK_HDMI 0x10000002
// HDMI ��� ���� (Android�� ����). �⺻ FALSE
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_SERVICESITE_CMD_SET_SOCKET_RETRY_COUNT 0x10000003
// Protocol ��Ž� Socket ���� ���� �߻��Ұ�� Retry�� ��� �� ������ �����Ѵ�. �⺻ 3ȸ.
// Parameter : unsigned int ������ �ּ� (4bytes), ���� Ƚ��

#define CTAGENT_SERVICESITE_CMD_SET_SOCKET_WAIT_TIME 0x10000004 
// Protocol ��Ž� Socket�� WaitTime�� �� milli seconds�� �� ������ �����Ѵ�. �⺻ 10��.
// Parameter : unsigned int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_CACHING 0x10000005 
// Multicast EMM cache ��� ���θ� �����Ѵ�. �⺻ FALSE
// TRUE ������ �⺻ ���� �Ⱓ 604800�� (7��)
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_CACHING_VALID_TIME_SECONDS 0x10000006 
// Multicast EMM cache ��� �� cache���� ��ȿ �Ⱓ�� �����Ѵ�. �⺻ 604800�� (7��)
// ������
// Parameter : int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_SET_TS_DECRYPT_LOG_CYCLE 0x10000007 
// CT_Decrypt() ��� �α��� ��� �ֱ⸦ �����Ѵ�.(������ �׻� ����Ѵ�.)  �⺻ 1000000�� ȣ��� ���� �� ��
// Parameter : int ������ �ּ� (4bytes), �ֱⰪ�� 0���� Ŀ����, ���� Ƚ��
// ��) "1000000"  <- TS 1000000�� ȣ��� ���� �α� ���

#define CTAGENT_SERVICESITE_CMD_SET_CHANNEL_PREVIEW_CALLBACK 0x10000008
// �̸����� ����� �޽����� ������ �Լ� �����͸� �����Ѵ�. NULL�� ���޵Ǹ� ���� ���Ѵ�. �⺻ NULL.
// Parameter : �Լ� ������


typedef void(*CARM_FUNCP_ChannelPreviewCallback)(BYTE *ConID,					// ä�� ������ ä���� Content ID
	unsigned int ConIDSize,		// ä�� ������ ä���� Content ID�� ũ��
	int lPreviewSec);				// ä�� �����⸦ ��û�� �ð�(��)

#define CTAGENT_SERVICESITE_CMD_SET_CHANNEL_PREVIEW_CALLBACK_SEC 0x10000009 
// �̸����� ���� �� �� ���� Callback �Լ��� ȣ������ �����Ѵ�. �̸����� �ð����� ũ�ų� ������ ���� ����. �⺻ 0��.
// Parameter : int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_SET_PLAYER_CHANNEL_PREVIEW_SEC 0x1000000A 
// Player���� �̸����� �ð��� �����Ѵ�.
// DRM ���� ������ �̸����� �ð��� Player�� ������ �̸����� �ð��� �� ���� ���� �̸����� �ð����� �����ȴ�.
// Parameter : int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_SET_VOD_PREVIEW_CALLBACK 0x1000000B
// VOD �̸����� ����� �޽����� ������ �Լ� �����͸� �����Ѵ�. NULL�� ���޵Ǹ� ���� ���Ѵ�. �⺻ NULL.
// Parameter : �Լ� ������
typedef void(*CARM_FUNCP_PreviewCallback)(BYTE *ConID,					// VOD �̸����� �������� Content ID
	unsigned int ConIDSize,		// VOD �̸����� �������� Content ID�� ũ��
	int lPreviewSec);			// VOD �̸����� �������� ��û�� �ð�(��)

#define CTAGENT_SERVICESITE_CMD_SET_AGENT_LOADER_ERROR 0x1000000C
// CARM Agent Loader���� �߻��� �����ڵ� ����
// Parameter : int ������ �ּ� (4bytes)

#define CTAGENT_SERVICESITE_CMD_STOP_UPDATE_KEY_THREAD 0x1000000D
// CTAGENT_UpdateKc Thread�� �����ϱ� ���� ȣ��.
// Parameter : NULL

#define CTAGENT_SERVICESITE_CMD_SET_OFFLINE_AUTHINFO 0x1000000E
// offline�� �� ���Ǵ� AuthInfo�� �����Ѵ�.
// offline�� AuthInfo�� ���� �Ǿ� ������ GetVODLicense(), OpenVODLicense()���� �� ������ ���ȴ�.

#define CTAGENT_SERVICESITE_CMD_SET_OFFLINE_CONTENT_ID 0x1000000F
// offline�� �� ���Ǵ� Content ID�� �����Ѵ�.
// offline�� Content ID�� ���� �Ǿ� ������ GetVODLicense(), OpenVODLicense()���� �� ������ ���ȴ�.


#define CTAGENT_SERVICESITE_CMD_GET_DRM_CHANNEL_PREVIEW_SEC 0x11000001
// DRM ���� ������ �̸����� �ð��� �����Ѵ�.
// DRM ���� ������ �̸����� �ð��� Player�� ������ �̸����� �ð��� �� ���� ���� �̸����� �ð����� �����ȴ�.
// Parameter : int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_GET_PLAYER_CHANNEL_PREVIEW_SEC 0x11000002
// Player�� ������ �̸����� �ð��� �����Ѵ�.
// DRM ���� ������ �̸����� �ð��� Player�� ������ �̸����� �ð��� �� ���� ���� �̸����� �ð����� �����ȴ�.
// Parameter : int ������ �ּ� (4bytes), ���� ��

#define CTAGENT_SERVICESITE_CMD_GET_CHANNEL_PREVIEW_INFO 0x11000003
// Ư�� ä���� �̸����� ��û������ ä�������� Ȯ���Ѵ�.
// Parameter : PREVIEW_CHANNEL_INFO ����ü ���� �ּ�(4bytes)

typedef struct _PREVIEW_CHANNEL_INFO
{
	BYTE *ConID;                 // in  : ex) "233.18.145.121,55492"
	unsigned int ConIDSize;      // in  : ex) 20
	int IsPreviewChannel;        // out : CTAGENT_TRUE or CTAGENT_FALSE
} PREVIEW_CHANNEL_INFO;

#define CTAGENT_SERVICESITE_CMD_SET_LI_URL 0x11000004
// LicenseIssuer URL �ּҸ� ����/�����Ѵ�.
// Parameter : char ���ڿ� �ּ�, LicenseIssuer URL (null terminated string)
//             ex) "CTLP://61.34.112.42:7845"

#define CTAGENT_SERVICESITE_CMD_SET_UEMM_URL 0x11000005
// UnicastEMM URL �ּҸ� ����/�����Ѵ�.
// Parameter : char ���ڿ� �ּ�, UnicastEMM URL (null terminated string)
//             ex) "CTLP://61.34.112.42:5490"

#define CTAGENT_SERVICESITE_CMD_SET_MEMM_URL 0x11000006
// MulticastUEMM URL �ּҸ� ����/�����Ѵ�.
// Parameter : char ���ڿ� �ּ�, MulticastUEMM URL (null terminated string)
//             ex) "CTLP://233.18.145.129:5490"

#define CTAGENT_SERVICESITE_CMD_RELEASE_SERVICESITE 0x11000007
// SERVICE SITE�� �Ҵ�� �ڿ��� �ʱ�ȭ �Ѵ�.
// Parameter : NULL


// CARM Agent Info Command //
#define CTAGENT_AGENT_CMD_SET_PACKAGED_CHANNEL_PREVIEW 0x20000001 
// CARM Agent���� Packaged channel preview ��� ���� ���θ� �����Ѵ�. �⺻ FALSE
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_CHANNEL_LICENSE_USE_FLAG 0x20000002
// CARM Agent���� Channel License ��� ���� ���θ� �����Ѵ�. �⺻ TRUE
// CT_SetCARM() ���� �ݿ��ȴ�.
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_PROTOCOL_CONFIG_USE_FLAG 0x20000003
// CARM Agent���� �̹� ������ CONFIG ������ �����Ұ�쿡�� CONFIG ������ ���� ��� ������ ���θ� �����Ѵ�. �⺻ TRUE
// CT_SetCARM() ���� �ݿ��ȴ�.
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_CERTIFICATE_REMOVE_FLAG 0x20000004
// CARM Agent���� �������� �����Ѵ�. �⺻ FALSE
// CT_SetCARM() ���� �ݿ��� �� �ٽ� �⺻ ���� �����ȴ�. (1ȸ�� FLAG)
// Parameter : int ������ �ּ� (4Bytes), TRUE or FALSE

#define CTAGENT_AGENT_CMD_SET_TIME_COMPARE_USE_FLAG 0x20000005
// H/E�� Agent �ð� �� ��� ��� ����. �⺻ 0
// Parameter : int ������ �ּ� (4Bytes), ���� �� (0�� ��� ��� Disable)

#define CTAGENT_AGENT_CMD_SET_AUTHENTICATION_SERVER_URL 0x20000006
// Authentication Server URL �ּҸ� ����/�����Ѵ�.
// Parameter : char ���ڿ� �ּ�, Authentication Server URL (null terminated string)
//             ex) "CTLP://61.34.112.42:5490"

#define CTAGENT_AGENT_CMD_SET_AGENT_TOKEN 0x20000007
// Coretrust���� �߱��� CARM Agent Token�� �����Ѵ�.
// Parameter : CTAGENT_AGENT_TOKEN ����ü ���� �ּ� 
typedef struct
{
	char TokenAlpha[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_AGENT_TOKEN;

#define CTAGENT_AGENT_CMD_SET_APPLICATION_TOKEN 0x20000008
// Coretrust���� �߱��� Application Token�� �����Ѵ�.
// Parameter : CTAGENT_APPLICATION_TOKEN ����ü ���� �ּ� 
typedef struct
{
	char TokenAlpha[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenGamma[CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_APPLICATION_TOKEN;

#define CTAGENT_AGENT_CMD_SET_PLAYER_TOKEN 0x20000009
// Coretrust���� �߱��� Player Token�� �����Ѵ�.
// Parameter : CTAGENT_PLAYER_TOKEN ����ü ���� �ּ� 
typedef struct
{
	int TokenCount; // max count is 10
	char TokenAlpha[10][CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
	char TokenBeta[10][CTAGENT_MAX_TOKEN_SIZE]; // (null terminated string)
} CTAGENT_PLAYER_TOKEN;

#define CTAGENT_CMD_SET_LOG_CALLBACK 0x2000000A
// .
// Parameter : CALLBACK_PARM ����ü ���� �ּ� 
typedef void(*CARMLogCallback_t_)(void* context, int level, const char* message);
typedef struct
{
	CARMLogCallback_t_ LogCall;    // User callback called for every message to be logged
	void *LogContext;               // Opaque pointer passed in every log callback
} CTAGENT_CALLBACK_PARAM;

#define CTAGENT_AGENT_CMD_SET_POLL_TIMEOUT 0x2000000B
// CARM Agent���� POLL_TIMEOUT �ð��� 1000ms�� �ƴ� �ٸ� ������ �����ϰ� ���� �� CT_SetCARMAgentCommand�� command�� ����.
// CT_SetCARM() ���� �ݿ��ȴ�.
// Parameter : 1000�̿��� ����(ms)

// export function
CARMAGENT_EXPORT int CT_SetCARM(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *AuthInfo,                // ����� ������ ���� BYTE String. Service Site�� Oriented �� Value
	unsigned int AuthInfoSize,     // ����� ���������� ũ��
	char *LiUrl,                   // License Issuer URL
	char *UEMMUrl,                 // Unicast EMM URL
	char *MEMMUrl,                 // Multicast EMM URL
	char *CertPath,                // CARM Agent���� ����ϴ� ������ ���� ���
	char *LogPath,                 // CARM Agent�� ���� Log ���� ���
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_OpenCARM(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *ConID,                   // Content �ĺ�����
	unsigned int ConIDSize,        // Content �ĺ������� ũ��
	unsigned int ConType,          // Content Type. Channel or VOD content ������
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT void CT_CloseCARM(
	int CARMDesc);                 // CARM Agent���� �����ϴ� CARM Descriptor Number

CARMAGENT_EXPORT int CT_GetVODLicense(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *ConID,                   // Content �ĺ�����
	unsigned int ConIDSize,        // Content �ĺ������� ũ��
	BYTE *LicenseBuf,              // ��ȣȭ�� VOD License�� ��� Buffer
	unsigned int LicenseBufSize,   // License Buffer�� ũ��
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_OpenVODLicense(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *ConID,                   // Content �ĺ�����
	unsigned int ConIDSize,        // Content �ĺ������� ũ��
	BYTE *LicenseBuf,              // ��ȣȭ�� VOD License�� ��� Buffer
	unsigned int LicenseBufSize,   // License Buffer�� ũ��
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_IsCARM(
	BYTE *ConID,                   // Content �ĺ�����
	unsigned int ConIDSize,        // Content �ĺ������� ũ��
	int *CARMDesc,                 // CARM Descriptor Number
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_Decrypt(
	int CARMDesc,                  // CARM Descriptor Number
	BYTE *Buf,                     // ��ȣȭ �ؾߵ� Data�� ��� Buffer ������
	unsigned int BufSize,          // Buffer�� Size
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_Decrypt2(
	int CARMDesc,                  // CARM Descriptor Number
	BYTE *Buf,                     // ��ȣȭ �ؾߵ� Data�� ��� Buffer ������
	unsigned int BufSize,          // Buffer�� Size
	void *Val1,                    // ��ȣȭ �� �ʿ��� �߰� ����, ���� ��� NULL ����
	void *Val2,                    // ��ȣȭ �� �ʿ��� �߰� ����, ���� ��� NULL ����
	void *Val3,                    // ��ȣȭ �� �ʿ��� �߰� ����, ���� ��� NULL ����
	void *Val4,                    // ��ȣȭ �� �ʿ��� �߰� ����, ���� ��� NULL ����
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_PutEMM(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *Buf,                     // EMM�� ��� ���� ������
	unsigned int BufSize,          // EMM�� ��� ������ ũ��
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT char *CT_GetCARMVer();

CARMAGENT_EXPORT int CT_GetValidChannel(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	BYTE *ConID,                   // Content �ĺ�����
	unsigned int ConIDSize,        // Content �ĺ������� ũ��
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_SetDescriptorCommand(
	int CARMDesc,                  // CARM Descriptor Number
	unsigned int Command,          // CARM Descriptor�� ������ ���
	BYTE *Parameter,               // CARM Descriptor�� ������ Parameter
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_SetServiceSiteCommand(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	unsigned int Command,          // CARM Descriptor�� ������ ���
	BYTE *Parameter,               // CARM Descriptor�� ������ Parameter
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_SetCARMAgentCommand(
	unsigned int Command,          // CARM Agent ������ ���
	BYTE *Parameter,               // CARM Agent ������ Parameter
	unsigned int *ErrCode);        // Error Code return ��

CARMAGENT_EXPORT int CT_GetServiceSiteCommand(
	BYTE *SrvID,                   // Service Provider ������
	unsigned int SrvIDSize,        // Service Provider �������� ũ��
	unsigned int Command,          // CARM Descriptor�� ������ ���
	BYTE *Parameter,               // CARM Descriptor�� ������ Parameter
	unsigned int *ErrCode);        // Error Code return ��