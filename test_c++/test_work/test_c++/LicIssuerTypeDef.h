#pragma once

#ifndef _LICENSE_ISSUER_TYPE_DEF_
#define _LICENSE_ISSUER_TYPE_DEF_

// #include <winsock2.h>
// #include <mswsock.h>
// #include <Ws2tcpip.h>
#include <wtypes.h>
//-----------------------------------------------------------------------------
// Define
//-----------------------------------------------------------------------------
#ifdef _DEBUG
//1214 에서부터 서비스포트를 DB에서 가져온다.		#define SERVER_LISTEN_PORT					7846	// debug
#define PARAMETER_COMPLETION_THREAD_COUNT	12		// parameter of CreateIoCompletionPort function
#define CREATED_COMPLETION_THREAD_COUNT		12		// the number of created completion threads
#define MAX_SOCKET_OBJ_POOL					5000
#define BUFFER_SIZE							16384		// I/O buffer size
#else
//1214 에서부터 서비스포트를 DB에서 가져온다.		#define SERVER_LISTEN_PORT					7845	// release
#define PARAMETER_COMPLETION_THREAD_COUNT	12		// parameter of CreateIoCompletionPort function
#define CREATED_COMPLETION_THREAD_COUNT		12		// the number of created completion threads
#define MAX_SOCKET_OBJ_POOL					5000
#define BUFFER_SIZE							16384		// I/O buffer size
#endif

#define LISTEN_BACK_LOG						1000
#define ACCEPTEX_COUNT                      1000
#define MAX_LICENSE_LIST					200
#define MAX_CONTENT_ID						256			// max length of cspcontentlist.cspcontentlist_cspcode
#define MAX_MEM_ID							50			// max length of cspmember.mem_id
#define MAX_STB_ID							50			// max length of cspterminl.cspterminal_id

#define OP_ACCEPT							1          // AcceptEx
#define OP_READ								2          // WSARecv/WSARecvFrom
#define OP_WRITE							3          // WSASend/WSASendTo
#define OP_REJECT							4

// DOS 해결을 위해 추가
#define SOCK_STAT_CONNECT					1           // SOCKET 이 연결만 된 상태.
#define SOCK_STAT_RECV						2           // 패킷을 Recvieve한 상태.
#define SOCK_STAT_SEND						3           // 패킷을 Send한 상태.

// PostAccept 에서의 리턴값
#define ERROR_INVALID_SOCKET				-1			// WSASocket() 이 INVALID_SOCKET 을 리턴함.
#define ERROR_SOCKET_OBJECT_FULL			-2			// SocketObj 가 full 상태임.
#define ERROR_CALL_ACCEPTEX					-3			// AcceptEx 호출 결과 Fail 리턴됨.

//--- 서비스 프로그램에서 호출 하는 것.
#define	UM_SERVICE_PROGRAM_DATA				(WM_USER + 1773)	// Service Program에 전달할 메시지 번호  
#define	SERVICE_PROGRAM_MESSAGE_EXIT		1

//-- softcom : 2007-10-10
#define LOGIOTHREAD_ID						99
#define LOGMESSAGE_COUNT					60			// [2017.07.27. smpark] Log Queue 의 크기.(Thread 별로 로그 사용). 50 --> 60( 60 보다 큰 경우 오류 발생) 으로 변경.

// Device License 추가.
// Device_Type : Count 를 512개 배열로 선언하여 메모리에서 관리함.
// 한 서비스 사이트에 추가 가능한 최대 Device : 512개.
#define MAX_DEVICE_LIST						512


// [2017.04.05. smpark] KT-CAS 연동을 위한 Client 인증서
#define KTCAS_CERT_FILE "ktcas-client.crt"
#define KTCAS_CERT_KEY_FILE "ktcas-client.key"

// [2017.04.05. smpark] KT-CAS 연동을 위한 CA 인증서
#define KTCAS_CA_FILE "ktcas-ca.crt"	/*Trusted CAs location*/
#define KTCAS_CA_FILE_PATH NULL

/*Password for the key file*/
#define KTCAS_KEY_PASSWD "skdn#!12"

/**/
// #define CIPHER_LIST "AES128-SHA"
// #define CIPHER_LIST "HIGH:!DSS:!aNULL@STRENGTH:!ADH:!MD5:!RC4:!SHA"
#define CIPHER_LIST "AES256-GCM-SHA384" // [2017.07.27. smpark] VKS 연결 오류 테스트를 위해 CIPHER 지정.


enum {
	SOCK_FLAG = 1,
	SOCK_READABLE = SOCK_FLAG,
	SOCK_WRITEABLE = SOCK_FLAG << 1
};

//-----------------------------------------------------------------------
//		Protocol Const Define
//-----------------------------------------------------------------------
#define		PROTOCOL_ID							0x1234
#define		PROTOCOL_DECRYPT_CHECK				0xCC12

#define		PROTOCOL_VERSION					0x0004 // 4
#define		PROTOCOL_VERSION_10					0x000A // 10	  // freeon : 통합 버전

#define		CMD_ERROR							0x0000

#define		CMD_PROXY_REQ						0x0009	// [2017.02.06. smpark] Proxy 서버를 통한 요청일 경우.

#define		CMD_PROXY_INIT_REQ					0x1901	// [2017.02.14. smpark] Proxy 서버를 Sesion Key 요청
#define		CMD_PROXY_INIT_RES					0x1902	// [2017.02.14. smpark] Proxy 서버를 Sesion Key 응답

#define		CMD_INIT_REQ						0x1001
#define		CMD_INIT_RES						0x1002

#define		CMD_SERVER_CERT_REQ					0x2001
#define		CMD_SERVER_CERT_RES					0x2002

#define		CMD_AGENT_CERT_REQ					0x3001
#define		CMD_AGENT_CERT_RES					0x3002

#define		CMD_PC_AGENT_CERT_REQ				0x3101
#define		CMD_PC_AGENT_CERT_RES				0x3102

#define		CMD_AGENT_CERT_REQ_V2				0x3201	// [2015.03.10. smpark] BO 도입에 따른 프로토콜 추가.
#define		CMD_AGENT_CERT_RES_V2				0x3202	// [2015.03.10. smpark] BO 도입에 따른 프로토콜 추가.

#define		CMD_CONFIG_REQ						0x4001
#define		CMD_CONFIG_RES						0x4002
#define		CMD_IMG_UPDATE_RES					0x4003
#define		CMD_CERT_UPDATE_RES					0x4004
#define		CMD_SECRET_KEY_UPDATE_RES			0x4005
#define		CMD_DEVICE_ID_DISABLE_RES			0x4006 // freeon

#define		CMD_CONFIG_P_REQ					0x4101		// 미리보기 추가 - 디바이스 type 별 총 미리보기 시간 요청
#define		CMD_CONFIG_P_RES					0x4102		// 미리보기 추가 - 디바이스 type 별 총 미리보기 시간 응답


#define		CMD_SECRET_KEY_REQ					0x5001
#define		CMD_SECRET_KEY_RES					0x5002

// freeon :
// Personalizer <-> LI간 프로토콜 추가
#define		CMD_WEB_LI_PARAM_REQ				0x5003
#define		CMD_WEB_LI_PARAM_RES				0x5004

// [2016.12.13. smpark] VOD 미리보기 프로토콜 추가 for iOS Device
#define		CMD_WEB_LI_PREVIEW_PARAM_REQ		0x5006
#define		CMD_WEB_LI_PREVIEW_PARAM_RES		0x5007


#define		CMD_LIC_CHECK_REQ					0x6001
#define		CMD_LIC_CHECK_RES					0x6002

#define		CMD_LIC_ARRAY_REQ					0x7001
#define		CMD_LIC_ARRAY_RES					0x7002

#define		CMD_LIC_ARRAY_PCTV_REQ				0x7101

#define		CMD_LIC_REQ							0x8001
#define		CMD_LIC_RES							0x8002
#define		CMD_LIC_INVALID_RES					0x8003
#define		CMD_LIC_DISABLE_RES					0x8004

#define		CMD_LIC_PCTV_REQ					0x8101

/* 1.2.1.4 에서 사라짐.
#define		CMD_ENCKEY_UPDATE_RES				0x8005

#define		CMD_ENCKEY_REQ						0x9001
#define		CMD_ENCKEY_RES						0x9002

#define		CMD_ENCKEY_CONFIRM_REQ				0xA001
#define		CMD_ENCKEY_CONFIRM_RES				0xA002
*/

#define		CMD_VOD_LIC_REQ						0x9001
#define		CMD_VOD_LIC_RES						0x9002

#define		CMD_VOD_ALBUM_LIC_REQ				0x9003
#define		CMD_VOD_ALBUM_LIC_RES				0x9004

// 2011-07 PCTV device용. 
// request 구조체, response 구조체 CMD_VOD_LIC_REQ와 완전 동일함. 
// 서버에서 발급로그를 구분하여 저장하여야 하기 때문에(라이센스 문제) command만 따로 구분함.
#define		CMD_VOD_LIC_PCTV_REQ				0x9005

#define		CMD_VOD_LIC_REQ_V2					0x9301
#define		CMD_VOD_LIC_RES_V2					0x9302

// [2016.12.13. smpark] VOD 미리보기 . 구매을 하지 않은 상태에서 VOD 라이센스 요청시 처리.
#define		CMD_VOD_PREVIEW_LIC_REQ				0x9006
#define		CMD_VOD_PREVIEW_LIC_RES				0x9007

// 2014-12-08 VOD 콘텐츠 검수를 위한 VOD 라이센스 요청시 처리를 위한 프로토콜 추가.
#define		CMD_VOD_VERIFICATION_LIC_REQ		0x9008
#define		CMD_VOD_VERIFICATION_LIC_RES		0x9009

// 2010-07 mobile device용 라이센스 저장 시간을 내려주기 위한 vod 프로토콜 추가.
// request 구조체는 CMD_VOD_LIC_REQ와 동일하나, response 구조체는 틀림.
#define		CMD_VOD_LIC_SAVE_REQ				0x9101
#define		CMD_VOD_LIC_SAVE_RES				0x9102

// 2011-05 NScreen device용. Bundle ID로 라이센스 요청 및 응답을 위한 vod 프로토콜 추가.
// request 구조체, response 구조체 CMD_VOD_SAVE_LIC_REQ와 동일함. 
// 서버에서 키를 찾는 로직이 달라 command만 따로 구분함.
#define		CMD_VOD_ALBUM_LIC_SAVE_REQ			0x9103
#define		CMD_VOD_ALBUM_LIC_SAVE_RES			0x9104

#define		CMD_VOD_PREVIEW_LIC_REQ_V2			0x9306
#define		CMD_VOD_PREVIEW_LIC_RES_V2			0x9307


#define		CMD_HEARTBEAT_FROM_L4				0x9999

// [smpark, 140731] Unicast EMMG 에서 사용하던 Protocol 추가.
//----------------------------------------------------------------
#define		CMD_EMM_REQ							0xA001
#define		CMD_EMM_RES							0xA002
#define		CMD_EMM_EMEG_RES					0xA003		// 비상키 응답. 서비스키를 패키지키로 암호화 하지 않고 RC4로만 암호화하여 응답함.
//----------------------------------------------------------------

//-------------------------------
//		Sign 관련 Define
//-------------------------------
#define PUB_KEY_LENGTH				512		// KCDSA Public Key Length
#define PRI_KEY_LENGTH				512		// KCDSA Private Key Length
#define CERT_VERSION				121		//


//-------------------------------
//		Protocol 관련 구조체들 
//-------------------------------
// HEADER
// size = 12
typedef struct _PACKET_HEADER					// header는 전송시 암호화 하지 않음.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID 를 명시  0x1234 : 고정값.
	unsigned short	sh_ProtocolVer;				// 프로토콜 버젼  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 일 경우 등록되지 않은 초기상태)
	unsigned short	us_ProtocolCommand;			// 명령어 및 리턴코드.
	unsigned short	us_ProtocolDataSize;		// 전송 데이터 크기.
} PACKET_HEADER;




//////////////////////////////////////////////////////////////////////////////////////////////
// 인증서
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _PACKET_SERV_CERT_RES				// 전송시 ul_DecryptCheck 이후를 암호화 함.
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	long			lCertVersion;					// 인증서 버전
	char			szPadding[4];					// byte ordering Padding
	char			szCertOwner[64];				// 인증서 소유자 "CoreTrust License Issuer" 고정
	char			szCertIssuer[64];				// 인증서 발급자 "CoreTrust License Issuer" 고정
	char			szServPubkey[PUB_KEY_LENGTH];	// 서버 Public Key - DB로 부터 전달/변경되면 안됨.
	unsigned long	ulSignLength;					// sign 길이
	char			szPadding2[12];					// AES 암호화를 위한 패딩
	char			chServCertSign[128];			// 
} PACKET_SERV_CERT_RES;


typedef struct _PACKET_CERT_SIGN
{
	long			lCertVersion;					// 인증서 버전
	unsigned long	ulAgentID;						// byte ordering Padding or Agent ID
	char			szCertOwner[64];				// 인증서 소유자 "CoreTrust License Issuer" 고정
	char			szCertIssuer[64];				// 인증서 발급자 "CoreTrust License Issuer" 고정
	char			szPublicKey[PUB_KEY_LENGTH];	// 서버 Public Key - DB로 부터 전달/변경되면 안됨.
} PACKET_CERT_SIGN;


typedef struct _PACKET_AGENT_CERT_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	char			szMemberID[32];					// cspmember.mem_id (D&P경우 HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P경우 Home gateway password)
	char			szSTBID[20];					// MAC address 또는 기타 STB에 설정된 ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			szPadding[4];
} PACKET_AGENT_CERT_REQ;

// [2015.03.10. smpark] BO 도입에 따른 구조체 추가.
typedef struct _PACKET_AGENT_CERT_REQ_V2
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	char			szMemberID[32];					// cspmember.mem_id (D&P경우 HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P경우 Home gateway password)
	char			szSTBID[20];					// MAC address 또는 기타 STB에 설정된 ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			szSUID[32];						// [2015.03.10. smpark] SUID
	char			szSTBOS[32];					// [2015.03.10. smpark] User Device OS
	char			szPadding[4];
} PACKET_AGENT_CERT_REQ_V2;


typedef struct _PACKET_PC_AGENT_CERT_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	char			szMemberID[32];					// cspmember.mem_id (D&P경우 HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P경우 Home gateway password)
	char			szSTBID[20];					// MAC address 또는 기타 STB에 설정된 ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	unsigned int 	ulCDID;							// 2009-10-29 PC Agent 용 CDID
} PACKET_PC_AGENT_CERT_REQ;


typedef struct _PACKET_AGENT_CERT_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	long			lCertVersion;					// 인증서 버전
	unsigned long	ulAgentID;						// STB PK
	char			szCertOwner[64];				// 인증서 소유자 "CoreTrust License Issuer" 고정
	char			szCertIssuer[64];				// 인증서 발급자 "CoreTrust License Issuer" 고정
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key - DB로 부터 전달.
	unsigned long	ulSignLength;					// sign 길이
	char			szPadding[12];					// AES 암호화를 위한 패딩
	char			chServCertSign[128];			// 
} PACKET_AGENT_CERT_RES;


typedef struct _PACKET_CONFIG_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	char			szSTBType[32];
	unsigned long	ulAgentVersion;
	char			szPadding[4];
} PACKET_CONFIG_REQ;


typedef struct _PACKET_CONFIG_RES
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ulAreaCode;
	unsigned char	ucAreaCodeOrder;
	unsigned char	ucPVRFlag;
	unsigned char   ucFileLog;
	char			chPadding;
} PACKET_CONFIG_RES;


typedef struct _PACKET_SECRET_KEY_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	char			szSTBID[20];
	unsigned long	ulESignatureLength;
	char			szESignature[128];
} PACKET_SECRET_KEY_REQ;


typedef struct _PACKET_SECRET_KEY_RES
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	char			chSecretKey[16];
	unsigned long	ulSignLength;
	char			chESignature[128];
	char			szPadding[4];
} PACKET_SECRET_KEY_RES;


//////////////////////////////////////////////////////////////////////////////////////////////
// Agent Image 독립적 Update 기능 구현을 위한 구조체..
// 2007-07-01 현재 구조체만 선언되어 있는 상태임
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _PACKET_IMG_UPDATE_RES
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	int				nUpdataMethod;
	char			szUpdateServerIP[16];
	unsigned short  usUpdateServerPort1;
	unsigned short  usUpdateServerPort2;
} PACKET_IMG_UPDATE_RES;



//////////////////////////////////////////////////////////////////////////////////////////////
// VoD License
//////////////////////////////////////////////////////////////////////////////////////////////
// VOD 라이센스 요청 구조체
typedef struct _VOD_LIC_REQ_INFO
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	long			l_PinNumber;					// 
	long			l_PaymentType;					// 
	unsigned long	ul_ContentIDLength;				// 뒤에 붙는 sz_ContentID의 length
	char			sz_ContentID[MAX_CONTENT_ID];	// content.content_name
	char			sz_Padding[4];					// 암호화를 위해 16 배수로 맞추기
} VOD_LIC_REQ_INFO, *pVOD_LIC_REQ_INFO;

// [2015.03.10. smpark] BO 도입에 따라 VOD 라이센스 요청 구조체 추가.
typedef struct _VOD_LIC_REQ_INFO_V2
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	long			l_PinNumber;					// 
	long			l_PaymentType;					// 
	unsigned long	ul_ContentIDLength;				// 뒤에 붙는 sz_ContentID의 length
	unsigned char	*sz_ContentID;					// [2015.03.10. smpark] BO 도입에 따라 배열에서 char * 로 변경. content.content_name
	unsigned long	ul_OMSParamLen;					// [2015.03.10. smpark] BO 도입에 따라 파라미터 길이 추가.
	unsigned char	*sz_OMSParam;					// [2015.03.10. smpark] BO 도입에 따라 파라미터 추가.(가변)
	// char			sz_Padding[16];					// 암호화를 위해 16 배수로 맞추기
} VOD_LIC_REQ_INFO_V2, *pVOD_LIC_REQ_INFO_V2;


// client 에서 해석하는 uc_EncryptType
//#define		CTDEC_ALGORITHM_AES			0xFF
//#define		CTDEC_ALGORITHM_RC4			0xFE
//#define		CTDEC_ALGORITHM_RC5			0xFD
//#define		CTDEC_ALGORITHM_SAY			0xFC
//#define		CTDEC_ALGORITHM_DEFAULT		CTDEC_ALGORITHM_AES
// VOD 라이센스 응답 구조체
typedef struct _VOD_LIC_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	char			ch_Kc[16];						// 암호화키.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// 위의 define 주석 참조.
	unsigned short	us_SaveDay;						// VoD 라이센스 저장 일수 0:저장불가.
	char			sz_Padding[1];					// 암호화를 위해 16 배수로 맞추기
} VOD_LIC_RES_INFO, *pVOD_LIC_RES_INFO;


// Database에서 가져오는 라이센스 응답 구조체 정보 (bin->hexa 때문에)
// typedef struct _VOD_LIC_RES_INFO_IN_DB
// {
// 	VOD_LIC_INTEG_RES_INFO	lic_res;
// 	unsigned long	ul_LicenseID;					// license.license_pk
// 	char			ch_KcHexa[32];					// filecontent.filecontent_kc
// 	char			ch_ContentFormat[8];			// content.ctype_exe .XXXXXXXX 로 표현된 문자열.
// } VOD_LIC_RES_INFO_IN_DB, *pVOD_LIC_RES_INFO_IN_DB; 


// VoD 라이센스 발급 로그 저장을 위한 구조체.
// 
// 20081106 기존 License Log 관련 Thread 모델을 변경하지 않으면서, 
// Log DB 로의 12개의 Connection을 따불시키지 않기 위해 
// 인증서 발급 로그도 아래 구조체를 이용하여 Log DB 에 등록할 수 있게 기능 변경함.
//
typedef struct _VOD_LIC_LOG_INFO
{                                                   // VoD 발급로그 이용시                         인증서 발급로그 이용시
	unsigned char	ucNeedToWrite;					// reaload추가 log쓰기 operation을 수행했는지 여부. (if not zero : DB에 써야할 로그)
	unsigned long	ulAgentPK;						// cspterminal.cspterminal_pk                  Agent PK (로그Thread에서 사용)
	unsigned long 	ulLicensePK;					// license.license_pk                          null
	DWORD			dwResultCode;					// 발급 결과코드.                              pubkeylog_state
	unsigned long 	ulFlag;							// 유료:1 무료:0                               pubkeylog_new_flag, STB재사용:0 추가:1
	unsigned long 	ulLogFlag;                      // 로그구분 - 0으로 세팅                       1로 세팅
	char			szClientIp[33];					// cspterminal.cspterminal_ip                  pubkeylog_stb_ip
	unsigned long	ulMemberPK;						// cspmember.mem_pk                            pubkeylog_mem_pk
	char			szMemberID[51];					// cspmember.mem_id   (D&P경우 HDS sa ID)      pubkeylog_mem_id
	char			szMemberName[65];				// cspmember.mem_name (D&P경우 항상 null 임)   pubkeylog_mem_pwd	// [2015.05.08. smpark] BO 도입에 따라 길이 변경 64 -> 65.
	char			szSTBID[21];					// cspterminal.cspterminal_id.                 pubkeylog_stb_id
	char			szCPID[21];						// contentprivider.cp_id                       null
	char			szCPName[64];					// contentprivider.cp_name                     null
	char			szContentID[MAX_CONTENT_ID];	// content.content_name                        null
	char			szContentName[129];				// filecontent.filecontent_filename            null
	char			szLogDatetime[20];				// YYYY-MM-DD HH:Mi:SS 라이센스 발급시간.      pubkeylog_wdate
	char			szMessage[512];					// 라이센스 발급 메세지.                       null
	char			szSUID[51];						// 로그 기록시 SUID 추가 기록
	float			dwProcessTime;					// [2017.06.26. smpark] 처리 시간 추가.
	unsigned long	ulDeviceType;					// 0: skyLife, 1: OTP Player, 2: Pad TV, 3: Skylife 키즈랜드
} VOD_LIC_LOG_INFO, *pVOD_LIC_LOG_INFO;



//////////////////////////////////////////////////////////////////////////////////////////////
// Live License
//////////////////////////////////////////////////////////////////////////////////////////////
// 라이센스 저장-라이센스 counter 요청
// size = 16
typedef struct _LIC_CHECK_REQ_INFO				// 전송시 ul_DecryptCheck 이후를 암호화 함.
{
	unsigned long	ul_DecryptCheck;			// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	unsigned short	us_LicenseCounter;			// 요청하는 STB의 license counter
	long			l_Nonce;					// 암호화된 값이 할때마다 다르도록. 랜덤 수
	char			sz_Padding[4];				// 암호화를 위해 16 배수로 맞추기
} LIC_CHECK_REQ_INFO, *pLIC_CHECK_REQ_INFO;


// 라이센스 저장 응답 구조체
// size = 10
typedef struct _LIC_CHECK_RES_INFO
{
	unsigned long	ul_DecryptCheck;			// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	unsigned short	us_LicenseCounter;			// 요청하는 STB에 대한 DB상의 license counter
	unsigned short	us_UpdateCount;				// 추가/수정된 라이센스 갯수
	unsigned short	us_RemoveCount;				// 삭제된 라이센스 갯수
//	us_RemoveCount 갯수만큼 unsigned long 이 반복된 후
//	그 갯수에 따라 16의 배수로 맞추기 위해 ch_Padding으로 채워야 한다.
//	unsigned long	ul_LicenseID;				// license.license_pk
//	char			sz_Padding[4];				// 암호화를 위해 16 배수로 맞추기
} LIC_CHECK_RES_INFO, *pLIC_CHECK_RES_INFO;


// 라이센스 요청 구조체
typedef struct _LIVE_LIC_REQ_INFO
{
	unsigned long	ul_DecryptCheck;			// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	unsigned long	ul_LicenseID;				// license.license_pk
	long			l_Nonce;					// 암호화된 값이 할때마다 다르도록. 랜덤 수
	char			sz_Padding[4];				// 암호화를 위해 16 배수로 맞추기
} LIVE_LIC_REQ_INFO, *pLIVE_LIC_REQ_INFO;


// 라이센스 응답 구조체
// size = 80
typedef struct _LIVE_LIC_RES_INFO
{
	unsigned long	ul_DecryptCheck;	//                             복호화 성공여부 판단을 위한 고정값. 0xCC12.
	char			ch_CurKc[16];		// ServiceKey.servicekey_kc    현재키.
	char			ch_NextKc[16];		// ServiceKey.servicekey_kc    다음키.
	long			l_CurKcExpire;		// ServiceKey.servicekey_edate 현재키 만료시간. (현재부터 몇초 후)
	long			l_NextKcExpire;		// ServiceKey.servicekey_edate 다음키 만료시간. (현재부터 몇초 후)
	long			l_KcReqTime;		//                             다음 키 요청시간. (현재부터 몇초 후)
	long			l_LicStartTime;		// license.rights_startdate    라이센스 유효 시작시간.
	long			l_LicEndTime;		// license.rights_enddate      라이센스 유효 만료시간.
	unsigned long	ul_LicenseID;		// license.license_pk
	unsigned long	ul_CAServiceID;		// caservice.caservice_pk
	unsigned long	uc_ContentFormat;	// channel.xxxxx               스트림 형식 Format. 현시점 기본값 1.
	unsigned char	uc_ChannelIP[4];	// channel.channel_ip          license_type=PPV 일때 channel output IP
	unsigned short	us_ChannelPort;		// channel.channel_port        license_type=PPV 일때 channel output Port
	unsigned char	uc_KcFlag;			// servicekey.servickey_flag   key toggle
	unsigned char	uc_LicenseType;		// license.license_type        CA Service Lic / PPV Lic 여부
	char			sz_Padding[4];		//                             암호화를 위해 16 배수로 맞추기
} LIVE_LIC_RES_INFO, *pLIVE_LIC_RES_INFO;


// Database에서 가져오는 라이센스 응답 구조체 정보 (bin->hexa 때문에)
typedef struct _LIVE_LIC_RES_INFO_IN_DB
{
	LIVE_LIC_RES_INFO	lic_res;
	char			ch_CurKcHexa[32];
	char			ch_NextKcHexa[32];
} LIVE_LIC_RES_INFO_IN_DB, *pLIVE_LIC_RES_INFO_IN_DB;




// 에러 전송 구조체
typedef struct _ERROR_RES_INFO
{
	long			l_ErrorCode;				// 에러코드
	char			sz_ErrorMsg[512];			// 에러 메시지
} ERROR_RES_INFO, *pERROR_RES_INFO;


// Device License 추가.
typedef struct _DEVICE_LICENSE
{
	char			szDeviceType[32];
	unsigned long	ulDeviceCount;
} DEVICE_LICENSE_INFO, *pDeviceLicenseInfo;


// [smpark, 140731] Unicast EMMG 에서 사용하던 Protocol 관련 구조체 추가.
//----------------------------------------------------------------
typedef struct _PACKET_EMM_REQ
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	unsigned long	ul_IssuerID;					// 인증서 발급한 Server ID
	unsigned long	ul_ServiceIDLength;				// 뒤에 붙는 sz_ServiceID의 length
	char			sz_ServiceID[MAX_CONTENT_ID];	// channel.channel_serviceid
	char			sz_Padding[16];					// 암호화를 위해 16 배수로 맞추기
} PACKET_EMM_REQ, *pPACKET_EMM_REQ;

// 라이센스 응답 구조체
// size = 80
typedef struct _PACKET_EMM_RES
{
	unsigned long	ul_DecryptCheck;	//                             복호화 성공여부 판단을 위한 고정값. 0xCC12.
	unsigned long	ul_CAServiceID;		// caservice.caservice_pk
	unsigned char	uc_PackageKeyFlag;	// packagekey.packagekey_flag   key toggle

	char			ch_CurKc[16];		// ServiceKey.servicekey_kc    packagekey로 암호화한 현재키.
	char			ch_NextKc[16];		// ServiceKey.servicekey_kc    packagekey로 암호화한 다음키.
	unsigned char	uc_ServiceKeyFlag;	// servicekey.servickey_flag   key toggle

	long			l_CurKcExpire;		// ServiceKey.servicekey_edate 현재키 만료시간. (현재부터 몇초 후)
	long			l_NextKcExpire;		// ServiceKey.servicekey_edate 다음키 만료시간. (현재부터 몇초 후)
	long			l_KcReqTime;		//                             다음 키 요청시간. (현재부터 몇초 후)

	long			l_LicStartTime;		// license.rights_startdate    라이센스 유효 시작시간.
	long			l_LicEndTime;		// license.rights_enddate      라이센스 유효 만료시간.

	char			sz_Padding[9];		//                             암호화를 위해 16 배수로 맞추기
} PACKET_EMM_RES, *pACKET_EMM_RES;

// Database에서 가져오는 라이센스 응답 구조체 정보 (bin->hexa 변환 위함)
typedef struct _UEMMG_LIVE_LIC_RES_INFO_IN_DB
{
	PACKET_EMM_RES	lic_res;
	char			ch_CurKcHexa[32];
	char			ch_NextKcHexa[32];

	char			ch_CurPackageKcHexa[32];

	char			ch_CurPackageKcBin[16];
} UEMMG_LIVE_LIC_RES_INFO_IN_DB, *pUEMMG_LIVE_LIC_RES_INFO_IN_DB;



/* 1214 - 1215 제거
// EncKey Update 구조체
typedef struct _ENCKEY_REQ_INFO
{
	unsigned char	uc_X[16];
	int				n_SignSize;
	unsigned char   uc_Sign[128];
} ENCKEY_REQ_INFO, *pENCKEY_REQ_INFO;
*/


//-----------------------------------------------------------------------------
//		결제 Type 
//-----------------------------------------------------------------------------
#define SUBSCRIBE_TYPE		0							// 가입제 (정액제)
#define PPV_TYPE			1							// PPV


// 파일 로그의 레벨. 비트 단위 on/off 설정 가능토록 define 되어야 함.
// 레지스트리 값 설정 : 1100 : critical, error 만 로깅 실시
// 레지스트리 값 설정 : 1110 : critical, error, warning까지 로깅 실시
// 레지스트리 값 설정 : 1111 : critical, error, warning, notice 까지 로깅 실시
#define		LOG_CRITICAL	0x00000080		// 심각한 오류. 서버 중단등.
#define		LOG_ERROR		0x00000040		// 오류. 상황에 대한 즉각 대처필요.
#define		LOG_WARNING		0x00000020		// 경고.
#define		LOG_NOTICE		0x00000010		// 알림. 알아둘만한 상황
#define		LOG_INFO		0x00000008		// 알림. 중요하지 않은 정보들
#define		LOG_DEBUG		0x00000004		// 기본 디버그
#define		LOG_DEBUG2		0x00000002		// 디버그2
#define		LOG_DEBUG3		0x00000001		// 디버그3

#define		SUCCESS_LICENSEISSUER			0x10000000	// 성공 
#define		ERROR_LICENSEISSUER				0x20000000	// 에러 

#define		ERROR_PROTOCOL_VERSION			0x20000027	// protocol version이 PROTOCOL_VERSION define과 상이함.
#define		ERROR_PROTOCOL_ID				0x20000028	// protocol id가 PROTOCOL_ID define과 상이함.
#define		ERROR_PROTOCOL_COMMAND			0x20000029	// protocol CMD가 정의된 cmd가 아님.
#define		ERROR_PROTOCOL_DATA_SIZE		0x20000030	// protocol cmd에 따라 필요한 데이터 사이즈가 틀림.
#define		ERROR_PROTOCOL_DECRYPTION		0x20000031	// 암호화된 프로토콜 구조체의 복호화 실패.
//사용하지 않음.#define		ERROR_PROTOCOL_FLOW				0x20000032	// (선행 request가 필요한 상황에서) 프로트콜 흐름 오류.  


#define		ERROR_DATABASE_EXCEPTION 					0x20000002
#define		ERROR_CSPMEMBER_NOT_EXIST					0x20000010		// INIT_REQ 에 대한 mem_id 가 존재하지 않음.
#define		ERROR_CSPTERMINAL_ID_ALREADY_EXIST			0x20000033
#define		ERROR_CSPTERMINAL_AUTHKEY_EXPIRED			0x20000034

#define		ERROR_CSPMEMBER_STATE_DISABLE				0x20000004
#define		ERROR_CSPTERMINAL_STATE_DISABLE				0x20000005
#define		ERROR_CSPCONTENT_NOT_EXIST					0x20000006
#define		ERROR_CONTENT_STATE_DISABLE					0x20000007
#define		ERROR_CSPCONTENTLIST_STATE_DISABLE			0x20000008
#define		ERROR_CSPINFO_NOT_EXIST						0x2000000C		
#define		ERROR_CSPINFO_STATE_DISABLE					0x2000000D
// freeon 추가
#define		ERROR_DEVICE_STATE_DISABLED					0x2000000E		// STB_type 에 대하여 license_device 테이블에 데이터 없거나, status disable 상태
#define		ERROR_DEVICE_ID_DISABLED					0x20000022		// STB_type 에 대하여 license_device 테이블에 데이터 없거나, status disable 상태


#define		ERROR_PK_AUTHKEY_ERROR						0x20000024
#define		ERROR_DIGITAL_SIGN_FAIL						0x20000026

#define		ERROR_MIDDLEWARE_LINK						0x20000035
#define		ERROR_CONTENT_NOT_EXIST						0x20000036		
#define		ERROR_LICENSE_NOT_EXIST						0x20000037		// 개별적 라이센스 요청에 라이센스가 없음.
#define		ERROR_LICENSE_STATE_DISABLE					0x20000038
#define		ERROR_LICENSE_NOT_ACTIVATED					0x20000039
#define		ERROR_LICENSE_EXPIRED						0x20000040
#define		ERROR_LICENSE_INFO_NOT_EXIST				0x20000041		// 수정/변경된 라이센스 없음.

#define		ERROR_AUTH_REQUESTER_CONNECT				0x20001401		// KT-D&P HDS 연동 서버 접속 실패
#define		ERROR_AUTH_REQUESTER_FAIL					0x20001402		// KT-D&P HDS 인증 실패

#define		ERROR_MIDDLEWARE_REQUEST_FAIL				0x20001403		// KT-D&P 통합관리 stored-procedure 실행 실패
#define		ERROR_MIDDLEWARE_DB_EXCEPTION				0x20001404		// KT-D&P 통합관리 stored-procedure 인증 실패
#define		ERROR_MIDDLEWARE_RESPONSE_TIMEOUT_FAIL		0x20001405		// [2017.08.30. smpark] OMS 연동시 쿼리 실행시 Timeout 발생에 대한 오류 코드 추가.

#define		ERROR_CDID_NOT_EXIST						0x20001407		// CDID가 cdid 테이블에 존재하지 않아 인증서 발급불가.	// [2017.08.30. smpark] OMS 쿼리 Timeout 값 추가로, 0x20001405 --> 0x20001407 로 변경.
#define		ERROR_CDID_CHECK_FAIL						0x20001406		// CDID를 암호화한 스트링이 db의 cdid 암호화 스트링과 상이하여 인증서 발급불가.

#define		ERROR_IMCS_PACKAGE_NOT_EXIST				0x20001501		// LG-Dacom 고정가입제 요청에 패키지 없음.
#define		ERROR_IMCS_SUBSCRIBE_NOT_EXIST				0x20001502		// LG-Dacom 고정가입제 요청에 결제정보 없음.

#define		ERROR_IMCS_USER_NOT_EXIST					0x20001503		// LG-Dacom PPV 요청에 사용자 정보 없음
#define		ERROR_IMCS_CONTENT_NOT_EXIST				0x20001504		// LG-Dacom PPV 요청에 콘텐츠 정보 없음
#define		ERROR_IMCS_PPV_SUBSCRIBE_NOT_EXIST			0x20001505		// LG-Dacom PPV 요청에 가입자 정보 없음


#define		ERROR_AMOC_HTTP_SERVER_CONNECT_FAIL			0x20002401		// AMOC 서버 연결 실패
#define		ERROR_AMOC_REQUESTER_FAIL					0x20002402		// AMOC 인증 실패
#define		ERROR_AMOC_REQUESTER_TIMEOUT_FAIL			0x20002403		// AMOC 인증 실패. Timeout
#define		ERROR_AMOC_TICKET_VALIDATE_FAIL				0x20002404		// AMOC 티켓유효성 인증 실패.

// [2017.06.25. smpark] KT-CAS 관련 오류 코드 추가.
#define		ERROR_KTCAS_CONNTECTION_FAIL				0x20003401		// KT-CAS 연결 실패.
#define		ERROR_KTCAS_REQUEST_FAIL					0X20003402		// KT-CAS 연동 실패


//-------- DRM Server License 상태 정의
#define		LICENSEISSUER_ENABLE						0x10000000
#define		LICENSEISSUER_DISABLED						0x20000000

#define		LICENSEISSUER_DISABLE_TERM					0x20010000
#define		LICENSEISSUER_DISABLE_TERMINAL_CNT			0x20020000
#define		LICENSEISSUER_DISABLE_TIMESYNC				0x20040000
#define		LICENSEISSUER_DISABLE_FILEERROR				0x20080000


#define		ERROR_DRMSERVER_LICENSE						0x20000023
#define		ERROR_CSPTERMINAL_NOT_EXIST					0x2000000F
#define		ERROR_DRMSERVER_ID_NOT_EXIST				0x20000018

//--------------------------------------------------------------------
// freeon 통합 protocol 에러코드 추가
#define		ERROR_INVALID_SERVICE_ID					0x30000001
#define		ERROR_INVALID_AUTH_INFO						0x30000002
#define		ERROR_INVALID_CONTENT_INFO					0x30000003
#define		ERROR_INVALID_CRC							0x30000004	// CRC Check fail.

//	CMD_WEB_LI_PARAM	Protoco Error
#define		ERROR_INVALID_SECRET_KEY					0x30000004

// [2015.03.11. smpark] BO 도입에 따른 OMS 파라미터 오류 코드 추가.
#define		ERROR_INVALID_OMSPARAM						0x30000005


#define		LICENSE_FILE_SIZE	320 // [smpark, 140729] IPTV2.0.0 에서 라이센스 파일의 사이즈가 128에서 320으로 변경됨.
#define		ERROR_COMPOSITION_LICENSE_DISABLE			0x90000001  // Composition License 사용 불가 상태.

#define		ERROR_LICENSEISSUER_CONNECT_FAIL			0x80000001	// Proxy Server 에서 License issuer 서버 연결 오류.
#define		ERROR_PROXY_NOT_EXIST						0x80000002	// Proxy Server 정보가 존재 하지 않음
#define		ERROR_PROXY_DISABLED						0x80000003	// Proxy Server 가 비활성 되어 있는 상태

//-----------------------------------------------------------------------
//		Protocol Const Define
//-----------------------------------------------------------------------
// #define		LIC_PROTOCOL_ID								0x12345678
// #define		LIC_PROTOCOL_VERSION						2

//#define		LICENSE_RIGHTS_EPSC_NONE					0x01000000
//#define		LICENSE_RIGHTS_EDIT							0x01000001
//#define		LICENSE_RIGHTS_PRINT						0x01000002
//#define		LICENSE_RIGHTS_SAVE							0x01000004
//#define		LICENSE_RIGHTS_CANCEL						0x01000008

/*
#define		ERROR_DATABASE_OPEN_FAIL					0x20000001
#define		ERROR_CSPMEMBERTERMINAL_NOT_EXIST			0x20000003
#define		ERROR_CONTENT_GRADE_AGE						0x20000009
#define		ERROR_CONTENT_GRADE_SEX						0x2000000A
#define		ERROR_CONTENT_LIVE_KEY_NOT_EXIST			0x2000000B
#define		ERROR_CSPINFO_TERMINALTYPE_NOVALUE			0x2000000E

#define		ERROR_CSPLICENSE_NOT_EXIST					0x20000011
//-------- 에러 메시지 추가
#define		ERROR_LICENSETERMINAL_DISABLE				0x20000013
#define		ERROR_LICENSE_ISSUE_COUNT_DISABLE			0x20000014
#define		ERROR_LICENSE_ISSUE_TERM_DISABLE			0x20000015
#define		ERROR_LICENSE_ISSUE_KNOWN					0x20000016
#define		ERROR_LICENSE_ISSUE_SDATE_TERM_DISABLE		0x20000017
//-------- 에러 메시지 추가 kyooni
#define		ERROR_DRMSERVER_KEY_PAIR_NOT_EXIST			0x20000019
#define		ERROR_TERMINAL_KEY_PAIR_NOT_EXIST			0x20000020
#define		ERROR_TERMINAL_AUTH							0x20000021
#define		ERROR_PK_AUTH								0x20000022
//-------- 에러 메시지 추가 jong myoung
#define		ERROR_DIGITAL_SIGN_NOT_EXIST				0x20000025
*/



/*
#define		MK_CONST_LICENSE_PA_UNLIMIT					0
#define		MK_CONST_LICENSE_PA_COUNT					1
#define		MK_CONST_LICENSE_PA_TERM					2
#define		MK_CONST_LICENSE_PA_TERM_COUNT				3
#define		MKCONST_TERMINAL_NOT_REGIST					0
#define		MKCONST_TERMINAL_REGIST						1
*/

/*
#define		LICENSE_ENABLE_TERMINAL_NONE				0x04000000
#define		LICENSE_ENABLE_TERMINAL_PC					0x04000001
#define		LICENSE_ENABLE_TERMINAL_PDA					0x04000002
#define		LICENSE_ENABLE_TERMINAL_STB					0x04000004
*/


typedef struct _DRM_SERVER_INFO
{
	long lDRMSerLic;
	//	long lRsaUseFlag; 
	char szHid[17];
	char szLicFilePath[MAX_PATH];
	char szDRMServerId[17];
	char szStartDay[17];
	char szEndDay[17];
	char szRecentDay[17];
	char szMD5[33];

	char szDRMSPubKey[PUB_KEY_LENGTH + 1];			// Server public key
	char szDRMSPriKey[PRI_KEY_LENGTH + 1];			// Server private key
	char szDRMSSign[128];							// Server public key Sign
	unsigned long ulSignLength;						// Sign 길이

	short shServicePort;							// 서비스 포트번호

	char szDBProvider[MAX_PATH];					// ADO DB Connection 정보 
	char szDBProviderMW[MAX_PATH];					// ADO DB Connection 정보 
	// freeon
	// OMS DB Connection 정보.
	char szDBProviderOMS[MAX_PATH];

	// [2015.03.09. smpark] BO 도입에 따른 연동 정보 설정
	char	szDBProviderOTM[MAX_PATH];			// 신규 OTM DB Connection string.
	char	szDBProviderMIPG[MAX_PATH];			// 신규 OTT(MIPG) DB Connection string.

	char szDBProviderLOG[MAX_PATH];				// ADO Log DB Connection 정보 

	//------------------------------------------------------------
	//		하영수 추가 : STB 대수 라이센스에 대한 정보 
	//------------------------------------------------------------
	long	lServerLicense_STBCount;
	long	lServerLicense_UseSTBCount;
	long	lServerLicense_DBSTBCount;
	DWORD	dwLicenseState;
	//------------------------------------------------------------
	//		박종명 추가 : STB GARBAGE 대수 라이센스에 대한 정보 
	//------------------------------------------------------------
	long	lServerLicense_GarbageMaxCount;
	long	lServerLicense_LastUpdateDate;
	long	lServerLicense_GarbageTerminlaCount;

	long	l_auth_server_check_flag;				// KT:통합인증서버 연동 여부, Dacom:HTTP-Get 결제 연동 여부 
	char	sz_auth_server_webservice_name[256];	// KT:통합인증서버 WebService Name, Dacom:HTTP-Get 결제 연동 Web URL

	char szL4_IP1[32];								// IPS Server의 Heart-bit을 체크하는 L4 장비의 IP
	char szL4_IP2[32];								// 네대까지 지원
	char szL4_IP3[32];								// 
	char szL4_IP4[32];								// 

	// DNP
	long	l_ppv_check_flag;							// PPV에 대한 Middeware 연동 여부 
	long	l_subscribe_check_flag;						// 정액제에 대한 Middleware 연동 여부
	char	sz_middleware_db_ip[32];					// Middleware DB IP
	char	sz_middleware_db_catalog[64];				// Middleware DB Catalog
	char	sz_middleware_db_user[32];					// Middleware DB User
	char	sz_middleware_db_pw[32];					// Middleware DB Password
	char	sz_middleware_stored_procedure_name[64];	// Middleware Stored Procedure Name
	long	l_configuration_ver;						// DRM Server 환경 설정 버전  (ctsinfo.IPS_Policy_counter 컬럼)

	// freeon
	// OMS AS
	long	l_oms_ppv_check_flag;
	long	l_oms_subscribe_check_flag;
	char	sz_oms_db_ip[32];
	char	sz_oms_db_catalog[64];
	char	sz_oms_db_user[32];
	char	sz_oms_db_pw[32];
	char	sz_oms_db_stored_procedure_name[64];

	// [2015.03.09. smpark] - BO 도입에 따른 OTM 연동 정보 추가.
	// OTM
	short	sh_otm_integ_fixed_flag;
	short	sh_otm_integ_ppv_flag;
	char	sz_otm_db_ip[32];
	short	sh_otm_db_Port;
	char	sz_otm_db_catalog[64];
	char	sz_otm_user_id[32];
	char	sz_otm_user_pwd[32];
	char	sz_otm_sp_name[64];

	// [2015.03.09. smpark] - BO 도입에 따른 OTT 연동 정보 추가.
	// OTT	
	short	sh_ott_integ_fixed_flag;
	short	sh_ott_integ_ppv_flag;
	char	sz_ott_db_ip[32];
	short	sh_ott_db_Port;
	char	sz_ott_db_catalog[64];
	char	sz_ott_user_id[32];
	char	sz_ott_user_pwd[32];
	char	sz_ott_sp_name[64];

	// [2015.03.09. smpark] - BO 도입에 따른 사용자(단말) 연동 연동 정보 추가.
	long	l_auth_integ_check_flag;			// 통합인증서버 연동 여부
	char	sz_auth_integ_webservice_url[256];	// 통합인증서버 WebService URL

	// [2022.05.19. mgsong] - OpenCAS-MEIN인증에 따른 사용자(단말) 연동 연동 정보 추가.
	long	l_auth_integ_mein_check_flag;			// MEIN인증서버 연동 여부
	char	sz_auth_integ_mein_url[256];	// MEIN인증서버 WebService URL

	/// OTP 구매연동을 위한 url 주소 추가 필요?
	// [2017.02.24. smpark] KT 프라임무비팩 OTT 에서 AMOC 연동 URL
	char	sz_amoc_integ_url[256];	///< 통합인증서버 WebService URL @details 파일정보연동
	char	sz_amoc_integ_ocas_url[256];	///< 통합인증서버 WebService URL @details 티켓유효성검증
	int	sh_amoc_timeout;			// 통합인증서버 연동 Timeout(ms)

	// [2017.04.04] KT-CAS 연동을 위한 서버 정보
	char sz_apppath[128];			// KT-CAS 와 연동을 위한 CA, Client 인증서가 있는 위치.

	char sz_ktcas_hostname[256];
	int  sh_ktcas_port;

	char sz_ktcas_back_hostname[256];	// [2017.06.25. smpark] KT-CAS 2차 접속 IP
	int  sh_ktcas_back_port;			// [2017.06.25. smpark] KT-CAS 2차 접속 Port

	int sh_ktcas_con_timeout;			// [2017.07.21. smpark] VKS Connection Timeout
	int sh_ktcas_recv_timeout;			// [2017.07.21. smpark] VKS Receive Timeout


	// Host Name - for Process Up/Down Log
	char host_name[32];				// Host Name
	char server_name[51];			// Server ID (LI 는 딱히 서버 ID 가 없으므로 HostName을 쓴다.
	char process_event[51];			// SERVICE START,  SERVICE STOP
	char process_message[257];		// 

	// KT 는 비상모드와 Channel 서비스가 없으므로 제외.
	// Emergency
	// bool	bFileDBMode;	
	// char	szBaseDateTime[32];		// [smpark, 140731] Unicast EMMG에만 존재하는 변수로 Service Key 발급의 기준 시간. (DB 장애 시간)

	// Device License 추가.
	//CPtrList		*ptrDeviceList;

	unsigned int		gDeviceTypeCount;
	DEVICE_LICENSE_INFO	gDeviceLicenseArray[MAX_DEVICE_LIST];

	// [smpark, 140730] Composition License 를 위한 변수 추가.
	char szComLicFilePath[MAX_PATH];
	bool bFlagPackageKeyIssuer;
	bool bFlagVODLicenseIssuer;
	bool bFlagChannelKeyIssuer;
	bool bFlagAppleKeyIsser;

	/// [2022.06.16. mgsong] - OpenCAS-MEIN인증 HTTP Request에 사용할 정보
	char mein_appkey[32 + 1];	///< HTTP Request 헤더 applicationKey값	

} DRM_SERVER_INFO, *PDRM_SERVER_INFO;

// KT 개방형 CAS 장애 대응 로직 추가. addby dhseo 2022.12.07
typedef struct _EMERG_MODE_INFO
{
	bool bUseEmerg;												// Emergency Mode 사용 여부
	char szDBProvider[MAX_PATH];								// Emergency DB 접속
	char szDBIP[64];											// Emergency DB IP
	char szDBPort[16];											// Emergency DB Port
	bool bIsConnect;											// Emergency DB 접속 상태
	bool bDBReConnect[CREATED_COMPLETION_THREAD_COUNT];			// Emergency DB 정보가 변경 되었을때 재 접속 확인 여부
	bool bLogDBReConnect[CREATED_COMPLETION_THREAD_COUNT];		// Emergency Log DB 정보가 변경 되었을때 재 접속 확인 여부
	bool bUseFileLog;											// Log 파일 생성 여부
	char szFileLogPath[MAX_PATH];								// Log 파일 경로
	char szLogDBProvider[MAX_PATH];								// Emergency Log DB 접속
} EMERG_MODE_INFO;
// === 2022.12.07

//------------------------------------------------------------------------------------------------------------------------------------------------
// freeon : 
// 통합 프로토콜용 구조체 및 변수 선언
//------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct _PACKET_HEADER_INTEG					// header는 전송시 암호화 하지 않음.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID 를 명시  0x1234 : 고정값.
	unsigned short	sh_ProtocolVer;				// 프로토콜 버젼  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 일 경우 등록되지 않은 초기상태)
	unsigned short	us_ProtocolCommand;			// 명령어 및 리턴코드.
	unsigned short	us_ProtocolDataSize;		// 전송 데이터 크기.
	short			s_Nonce;
	unsigned short	us_crc;						// CRC Check
} PACKET_HEADER_INTEG;

typedef struct _PACKET_INIT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	unsigned char	uc_X[16];						// Key 교환 알고리즘 : X 수신 후 y 송신
}PACKET_INIT_INTEG_REQ;

typedef struct _PACKET_INIT_INTEG_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned char	uc_Y[16];						// Key 교환 알고리즘 : X 수신 후 y 송신
}PACKET_INIT_INTEG_RES;

typedef struct _PACKET_SERV_CERT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
}PACKET_SERV_CERT_INTEG_REQ;

typedef struct _PACKET_SERV_CERT_INTEG_RES			// 전송시 ul_DecryptCheck 이후를 암호화 함.
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	lCertVersion;						// 인증서 버전
	char			szPadding[4];					// byte ordering Padding
	char			szCertOwner[64];				// 인증서 소유자 "CoreTrust License Issuer" 고정
	char			szCertIssuer[64];				// 인증서 발급자 (Server ID)
	char			szServPubkey[PUB_KEY_LENGTH];	// 서버 Public Key - DB로 부터 전달/변경되면 안됨.
	unsigned long	ulSignLength;					// sign 길이
	char			szPadding2[12];					// AES 암호화를 위한 패딩
	char			chServCertSign[128];			// 
} PACKET_SERV_CERT_INTEG_RES;

typedef struct _PACKET_AGENT_CERT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device 인증 정보
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			sz_DeviceType[32];				// Device Type, MKT에서 발급
	//char			sz_DeviceSerial[32];	
} PACKET_AGENT_CERT_INTEG_REQ;

typedef struct _PACKET_AGENT_CERT_INTEG_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	long			lCertVersion;					// 인증서 버전
	unsigned long	ulAgentID;						// STB PK
	char			szCertOwner[64];				// 인증서 소유자 "CoreTrust License Issuer" 고정
	char			szCertIssuer[64];				// 인증서 발급자 (Server ID)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key - DB로 부터 전달.
	unsigned long	ulSignLength;					// sign 길이
	char			szPadding[12];					// AES 암호화를 위한 패딩
	char			chServCertSign[128];			// 
} PACKET_AGENT_CERT_INTEG_RES;

typedef struct _PACKET_CONFIG_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	char			sz_DeviceType[32];				// Device Type, MKT에서 발급
	unsigned long	ulAgentVersion;
} PACKET_CONFIG_INTEG_REQ;


typedef struct _PACKET_CONFIG_INTEG_RES
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ulAreaCode;
	unsigned char	ucAreaCodeOrder;
	unsigned char	ucPVRFlag;
	unsigned char   ucFileLog;
	// TODO: freeon	
	// DB SP 수정 필요할 듯 
	unsigned int	usPreViewTime;					// 미리보기 시간
	char			chEMMMFlag;						// MulticastEMM 0:미사용 1:사용
	char			chUEMMGFlagOnError;				// MulticastEMM오류시 UEMM 0:미사용 1:사용
	char			chPadding;
} PACKET_CONFIG_INTEG_RES;

typedef struct _PACKET_SECRET_KEY_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device 인증 정보
	unsigned long	ulESignatureLength;
	char			szESignature[128];
} PACKET_SECRET_KEY_INTEG_REQ;


typedef struct _PACKET_SECRET_KEY_INTEG_RES
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	char			chSecretKey[16];
	unsigned long	ulSignLength;
	char			chESignature[128];
	char			szPadding[4];
} PACKET_SECRET_KEY_INTEG_RES;

typedef struct _VOD_LIC_INTEG_REQ_INFO
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	unsigned long	ul_ContentIDLength;				// 뒤에 붙는 sz_ContentID의 length
	unsigned char	*sz_ContentID;					// content.content_name
	unsigned long	ul_IssuerID;
} VOD_LIC_INTEG_REQ_INFO, *pVOD_LIC_INTEG_REQ_INFO;

typedef struct _VOD_LIC_INTEG_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	char			ch_Kc[16];						// 암호화키.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// 위의 define 주석 참조.
	unsigned char	us_SaveDay;						// VoD 라이센스 저장 일수 0:저장불가.
	char			sz_Padding[9];					// 암호화를 위해 16 배수로 맞추기
} VOD_LIC_INTEG_RES_INFO, *pVOD_LIC_INTEG_RES_INFO;

// typedef struct _VOD_LIC_INTEG_RES_INFO_IN_DB
// {
// 	VOD_LIC_INTEG_RES_INFO	lic_res;
// 	unsigned long	ul_LicenseID;			// license.license_pk
// 	char			ch_KcHexa[32];			// filecontent.filecontent_kc
// 	char			ch_ContentFormat[8];	// content.ctype_exe .XXXXXXXX 로 표현된 문자열.
// 	unsigned short  us_zero;				// 중요!! ch_ContentFormat 을 string_to_hex 처리할 때 null-terminaltor 역할  !!
// 	unsigned short	us_PackagePullFlag;		// IMCS_packagelist.product_type 에 해당하는 연동 플래그
// 	unsigned short	us_DevicePullFlag;		// license_device.pull_flag
// 	char			sz_DevicePullURL[256];	// license_device.pull_url
// } VOD_LIC_INTEG_RES_INFO_IN_DB, *pVOD_LIC_INTEG_RES_INFO_IN_DB; 

// Database에서 가져오는 라이센스 응답 구조체 정보 (bin->hexa 때문에)
typedef struct _VOD_LIC_RES_INFO_IN_DB
{
	VOD_LIC_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];					// filecontent.filecontent_kc
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX 로 표현된 문자열.	// [2016.12.15. smpark] Null 문자 처리를 위해 배열 1 증가.
} VOD_LIC_RES_INFO_IN_DB, *pVOD_LIC_RES_INFO_IN_DB;

typedef struct _VOD_LIC_INTEG_RES_INFO_IN_DB
{
	VOD_LIC_INTEG_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];				// filecontent.filecontent_kc	// [2016.12.15. smpark] Null 문자 처리를 위해 배열 1 증가.
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX 로 표현된 문자열.	// [2016.12.15. smpark] Null 문자 처리를 위해 배열 1 증가.
} VOD_LIC_RES_INTEG_INFO_IN_DB, *pVOD_LIC_RES_INTEG_INFO_IN_DB;

// [2016.12.13. smpark] VOD 미리보기 Response 추가
typedef struct _VOD_PREVIEW_LIC_INTEG_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			l_Nonce;						// 암호화된 값이 할때마다 다르도록. 랜덤 수
	// char			ch_Kc[16];						// 암호화키.
	char			ch_Kc[17];						// 암호화키.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// 위의 define 주석 참조.
	unsigned long	ul_PreviewTimeSec;				// VoD 미리보기 시간.(초)
	char			sz_Padding[9];					// 암호화를 위해 16 배수로 맞추기
} VOD_PREVIEW_LIC_INTEG_RES_INFO, *pVOD_PREVIEW_LIC_INTEG_RES_INFO;

typedef struct _VOD_PREVIEW_LIC_INTEG_RES_INFO_IN_DB
{
	VOD_PREVIEW_LIC_INTEG_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];					// filecontent.filecontent_kc
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX 로 표현된 문자열.
} VOD_PREVIEW_LIC_RES_INTEG_INFO_IN_DB, *pVOD_PREVIEW_LIC_RES_INTEG_INFO_IN_DB;
///////////////////////////////////////////////////////

// freeon 
// Web License Issuer 보안 강화를 위해 추가된 프로토콜
typedef struct _PACKET_WEB_LI_PARAM_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// 가변길이의 ServiceID를 받기위함
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device 인증 정보	
} PACKET_WEB_LI_PARAM_REQ;

typedef struct _PACKET_WEB_LI_PARAM_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_KeyServerURLSize;
	unsigned char	*sz_KeyServerURL;
	unsigned long	ul_EncParameterSize;			// EncParameter Size
	unsigned char	*sz_EncParameter;				// SecretKey 로 암호화된 파라미터 전달
} PACKET_WEB_LI_PARAM_RES;

// [2016.12.13. smpark] VOD 미리보기 기능 추가에 따른 WEB_LI_PARAM 에 미리보기(초) 추가
typedef struct _PACKET_WEB_LI_PREVIEW_PARAM_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_KeyServerURLSize;
	unsigned char	*sz_KeyServerURL;
	unsigned long	ul_EncParameterSize;			// EncParameter Size
	unsigned char	*sz_EncParameter;				// SecretKey 로 암호화된 파라미터 전달
	unsigned int	ul_preview_time;				// [2016.12.13. smpark] 미리보기 시간(초) 추가
} PACKET_WEB_LI_PREVIEW_PARAM_RES;

// [2015.03.11. smpark] BO 도입에 따라 OMSParam 정보를 저장할 구조체 
typedef struct _OMS_PARAM_INFO
{
	char			ch_service_type[10];
	char			ch_suid[32];
	char			ch_said[32];
	unsigned long	ul_content_id;
	unsigned char	*sz_co_content_id;
	unsigned char	*sz_pack_id;
	char			ch_sign_yn;
	unsigned short	us_is_test;
	unsigned char	*sz_param1;
	unsigned char	*sz_param2;
} OMS_PARAM_INFO, *pOMS_PARAM_INFO;

#define	SERVICE_ID_FOR_NONE_VALUE	0x00000000			// reserved(2) site(2) service(2) version(2)

#define SIZE_PACKET_HEADER_INTEG		16
#define SIZE_SERV_CERT_INTEG_RES		800
#define SIZE_AGENT_CERT_INTEG_RES		800
#define SIZE_CONFIG_INTEG_RES			32
#define SIZE_SECRET_KEY_INTEG_RES		160

#define	ENCRYPT_KEY						"CORECRYPTINITKEY"

#define	SERVICE_ID_FOR_KTOTN			"KT-OTN-01"
#define SERVICE_ID_FOR_KTOTN_LEN		9
#define	SERVICE_ID_FOR_KTOTN_VALUE		0x00030101			// reserved(2) site(2) service(2) version(2)

// [2015.03.11. smpark] BO 도입에 따라 define 추가.
#define	SERVICE_TYPE_OTT				"OTT"
#define	SERVICE_TYPE_OTM				"OTM"
#define	SERVICE_TYPE_OM					"OM"
#define	SERVICE_TYPE_KIBOT				"KIBOT"

#define	SERVICE_ID_FOR_KTOTN2			"KT-OTN-02"
#define SERVICE_ID_FOR_KTOTN2_LEN		9
#define	SERVICE_ID_FOR_KTOTN2_VALUE		0x00030102			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOTN3			"KT-OTN-03"			// KT OTT 프라임무비팩 용.
#define SERVICE_ID_FOR_KTOTN3_LEN		9
#define	SERVICE_ID_FOR_KTOTN3_VALUE		0x00030103			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOTN4			"KT-OTN-04"			// KT OTP Player 용.
#define SERVICE_ID_FOR_KTOTN4_LEN		9
#define	SERVICE_ID_FOR_KTOTN4_VALUE		0x00030104	

#define	SERVICE_ID_FOR_KTOTN5			"KT-OTN-05"			// KT 초등랜드 용
#define SERVICE_ID_FOR_KTOTN5_LEN		9
#define	SERVICE_ID_FOR_KTOTN5_VALUE		0x00030105			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOCAS			"KT-OCAS-01"		// [20220406 mgsong] OCAS (신규 AMOC 적용)
#define SERVICE_ID_FOR_KTOCAS_LEN		10
#define	SERVICE_ID_FOR_KTOCAS_VALUE		0x00030106			// reserved(2) site(2) service(2) version(2)


// AuthInfo 및 ContentID type 별 유효성 검사 하기 위해 정의
#define AUTHINFO_IPTV					1
#define AUTHINFO_NSCR					2
#define CONTENT_ID_IPTV_CH				1
#define CONTENT_ID_IPTV_VOD				2
#define CONTENT_ID_NSCR_CH				0
#define CONTENT_ID_NSCR_VOD				3

#define CONTENT_ID_VOD					5 // 2014.12.03. By smpark. ex) PIN,Payment,Content ID,Album ID, Content Format, ASSET ID

// [smpark, 140730] Composition License 정의 추가.
#define	PACKAGE_KEY_ISSUER	0x00000001
#define VOD_LICENSE_ISSUER	0x00000002
#define CHANNEL_KEY_ISSUER	0x00000003
#define APPLE_KEY_ISSUER	0x00000004

/* [2017.02.14. smpark] Proxy 인증처리를 위한 메시지 */
typedef struct _PROXY_INIT_REQ
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ProxyNameSize;				// ProxyName  Length
	unsigned char	*sz_ProxyName;					// 가변길이의 ProxyName를 받기위함
	unsigned char	uc_X[16];						// Key 교환 알고리즘 : X 를 생성하여 전달. 이후 수신되는 Y 와 조합하여 Session Key 생성.
}PROXY_INIT_REQ;

typedef struct _PROXY_INIT_RES
{
	unsigned long 	ulDecryptCheck;					// 복호화 성공여부 판단을 위한 고정값. 0xCC12.
	long			lNonce;							// 암호화된 값이 전송 할때마다 다르도록. 랜덤 수
	unsigned long	ul_ProxyID;						// proxyserverlist.proxyserver_pk 
	unsigned char	uc_Y[16];						// Key 교환 알고리즘 : X 수신 후 y 송신
}PROXY_INIT_RES;

// [2017.04.06. smpark] KT-CAS 연동을 위한 메시지 정의
typedef struct _KTCAS_MSG_HEADER
{
	char ch_ProtocolVer;
	unsigned short messageType;
	unsigned short messageLength;
} KTCAS_MSG_HEADER;

typedef struct _KTCAS_MSG_BODY
{
	unsigned short	paramType;
	unsigned short	paramLen;
	char			*paramVal;
	//char			paramVal[1024];
} KTCAS_MSG_BODY;

/* *************************************************** */




#endif

