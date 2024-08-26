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
//1214 �������� ������Ʈ�� DB���� �����´�.		#define SERVER_LISTEN_PORT					7846	// debug
#define PARAMETER_COMPLETION_THREAD_COUNT	12		// parameter of CreateIoCompletionPort function
#define CREATED_COMPLETION_THREAD_COUNT		12		// the number of created completion threads
#define MAX_SOCKET_OBJ_POOL					5000
#define BUFFER_SIZE							16384		// I/O buffer size
#else
//1214 �������� ������Ʈ�� DB���� �����´�.		#define SERVER_LISTEN_PORT					7845	// release
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

// DOS �ذ��� ���� �߰�
#define SOCK_STAT_CONNECT					1           // SOCKET �� ���Ḹ �� ����.
#define SOCK_STAT_RECV						2           // ��Ŷ�� Recvieve�� ����.
#define SOCK_STAT_SEND						3           // ��Ŷ�� Send�� ����.

// PostAccept ������ ���ϰ�
#define ERROR_INVALID_SOCKET				-1			// WSASocket() �� INVALID_SOCKET �� ������.
#define ERROR_SOCKET_OBJECT_FULL			-2			// SocketObj �� full ������.
#define ERROR_CALL_ACCEPTEX					-3			// AcceptEx ȣ�� ��� Fail ���ϵ�.

//--- ���� ���α׷����� ȣ�� �ϴ� ��.
#define	UM_SERVICE_PROGRAM_DATA				(WM_USER + 1773)	// Service Program�� ������ �޽��� ��ȣ  
#define	SERVICE_PROGRAM_MESSAGE_EXIT		1

//-- softcom : 2007-10-10
#define LOGIOTHREAD_ID						99
#define LOGMESSAGE_COUNT					60			// [2017.07.27. smpark] Log Queue �� ũ��.(Thread ���� �α� ���). 50 --> 60( 60 ���� ū ��� ���� �߻�) ���� ����.

// Device License �߰�.
// Device_Type : Count �� 512�� �迭�� �����Ͽ� �޸𸮿��� ������.
// �� ���� ����Ʈ�� �߰� ������ �ִ� Device : 512��.
#define MAX_DEVICE_LIST						512


// [2017.04.05. smpark] KT-CAS ������ ���� Client ������
#define KTCAS_CERT_FILE "ktcas-client.crt"
#define KTCAS_CERT_KEY_FILE "ktcas-client.key"

// [2017.04.05. smpark] KT-CAS ������ ���� CA ������
#define KTCAS_CA_FILE "ktcas-ca.crt"	/*Trusted CAs location*/
#define KTCAS_CA_FILE_PATH NULL

/*Password for the key file*/
#define KTCAS_KEY_PASSWD "skdn#!12"

/**/
// #define CIPHER_LIST "AES128-SHA"
// #define CIPHER_LIST "HIGH:!DSS:!aNULL@STRENGTH:!ADH:!MD5:!RC4:!SHA"
#define CIPHER_LIST "AES256-GCM-SHA384" // [2017.07.27. smpark] VKS ���� ���� �׽�Ʈ�� ���� CIPHER ����.


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
#define		PROTOCOL_VERSION_10					0x000A // 10	  // freeon : ���� ����

#define		CMD_ERROR							0x0000

#define		CMD_PROXY_REQ						0x0009	// [2017.02.06. smpark] Proxy ������ ���� ��û�� ���.

#define		CMD_PROXY_INIT_REQ					0x1901	// [2017.02.14. smpark] Proxy ������ Sesion Key ��û
#define		CMD_PROXY_INIT_RES					0x1902	// [2017.02.14. smpark] Proxy ������ Sesion Key ����

#define		CMD_INIT_REQ						0x1001
#define		CMD_INIT_RES						0x1002

#define		CMD_SERVER_CERT_REQ					0x2001
#define		CMD_SERVER_CERT_RES					0x2002

#define		CMD_AGENT_CERT_REQ					0x3001
#define		CMD_AGENT_CERT_RES					0x3002

#define		CMD_PC_AGENT_CERT_REQ				0x3101
#define		CMD_PC_AGENT_CERT_RES				0x3102

#define		CMD_AGENT_CERT_REQ_V2				0x3201	// [2015.03.10. smpark] BO ���Կ� ���� �������� �߰�.
#define		CMD_AGENT_CERT_RES_V2				0x3202	// [2015.03.10. smpark] BO ���Կ� ���� �������� �߰�.

#define		CMD_CONFIG_REQ						0x4001
#define		CMD_CONFIG_RES						0x4002
#define		CMD_IMG_UPDATE_RES					0x4003
#define		CMD_CERT_UPDATE_RES					0x4004
#define		CMD_SECRET_KEY_UPDATE_RES			0x4005
#define		CMD_DEVICE_ID_DISABLE_RES			0x4006 // freeon

#define		CMD_CONFIG_P_REQ					0x4101		// �̸����� �߰� - ����̽� type �� �� �̸����� �ð� ��û
#define		CMD_CONFIG_P_RES					0x4102		// �̸����� �߰� - ����̽� type �� �� �̸����� �ð� ����


#define		CMD_SECRET_KEY_REQ					0x5001
#define		CMD_SECRET_KEY_RES					0x5002

// freeon :
// Personalizer <-> LI�� �������� �߰�
#define		CMD_WEB_LI_PARAM_REQ				0x5003
#define		CMD_WEB_LI_PARAM_RES				0x5004

// [2016.12.13. smpark] VOD �̸����� �������� �߰� for iOS Device
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

/* 1.2.1.4 ���� �����.
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

// 2011-07 PCTV device��. 
// request ����ü, response ����ü CMD_VOD_LIC_REQ�� ���� ������. 
// �������� �߱޷α׸� �����Ͽ� �����Ͽ��� �ϱ� ������(���̼��� ����) command�� ���� ������.
#define		CMD_VOD_LIC_PCTV_REQ				0x9005

#define		CMD_VOD_LIC_REQ_V2					0x9301
#define		CMD_VOD_LIC_RES_V2					0x9302

// [2016.12.13. smpark] VOD �̸����� . ������ ���� ���� ���¿��� VOD ���̼��� ��û�� ó��.
#define		CMD_VOD_PREVIEW_LIC_REQ				0x9006
#define		CMD_VOD_PREVIEW_LIC_RES				0x9007

// 2014-12-08 VOD ������ �˼��� ���� VOD ���̼��� ��û�� ó���� ���� �������� �߰�.
#define		CMD_VOD_VERIFICATION_LIC_REQ		0x9008
#define		CMD_VOD_VERIFICATION_LIC_RES		0x9009

// 2010-07 mobile device�� ���̼��� ���� �ð��� �����ֱ� ���� vod �������� �߰�.
// request ����ü�� CMD_VOD_LIC_REQ�� �����ϳ�, response ����ü�� Ʋ��.
#define		CMD_VOD_LIC_SAVE_REQ				0x9101
#define		CMD_VOD_LIC_SAVE_RES				0x9102

// 2011-05 NScreen device��. Bundle ID�� ���̼��� ��û �� ������ ���� vod �������� �߰�.
// request ����ü, response ����ü CMD_VOD_SAVE_LIC_REQ�� ������. 
// �������� Ű�� ã�� ������ �޶� command�� ���� ������.
#define		CMD_VOD_ALBUM_LIC_SAVE_REQ			0x9103
#define		CMD_VOD_ALBUM_LIC_SAVE_RES			0x9104

#define		CMD_VOD_PREVIEW_LIC_REQ_V2			0x9306
#define		CMD_VOD_PREVIEW_LIC_RES_V2			0x9307


#define		CMD_HEARTBEAT_FROM_L4				0x9999

// [smpark, 140731] Unicast EMMG ���� ����ϴ� Protocol �߰�.
//----------------------------------------------------------------
#define		CMD_EMM_REQ							0xA001
#define		CMD_EMM_RES							0xA002
#define		CMD_EMM_EMEG_RES					0xA003		// ���Ű ����. ����Ű�� ��Ű��Ű�� ��ȣȭ ���� �ʰ� RC4�θ� ��ȣȭ�Ͽ� ������.
//----------------------------------------------------------------

//-------------------------------
//		Sign ���� Define
//-------------------------------
#define PUB_KEY_LENGTH				512		// KCDSA Public Key Length
#define PRI_KEY_LENGTH				512		// KCDSA Private Key Length
#define CERT_VERSION				121		//


//-------------------------------
//		Protocol ���� ����ü�� 
//-------------------------------
// HEADER
// size = 12
typedef struct _PACKET_HEADER					// header�� ���۽� ��ȣȭ ���� ����.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID �� ���  0x1234 : ������.
	unsigned short	sh_ProtocolVer;				// �������� ����  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 �� ��� ��ϵ��� ���� �ʱ����)
	unsigned short	us_ProtocolCommand;			// ��ɾ� �� �����ڵ�.
	unsigned short	us_ProtocolDataSize;		// ���� ������ ũ��.
} PACKET_HEADER;




//////////////////////////////////////////////////////////////////////////////////////////////
// ������
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _PACKET_SERV_CERT_RES				// ���۽� ul_DecryptCheck ���ĸ� ��ȣȭ ��.
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	long			lCertVersion;					// ������ ����
	char			szPadding[4];					// byte ordering Padding
	char			szCertOwner[64];				// ������ ������ "CoreTrust License Issuer" ����
	char			szCertIssuer[64];				// ������ �߱��� "CoreTrust License Issuer" ����
	char			szServPubkey[PUB_KEY_LENGTH];	// ���� Public Key - DB�� ���� ����/����Ǹ� �ȵ�.
	unsigned long	ulSignLength;					// sign ����
	char			szPadding2[12];					// AES ��ȣȭ�� ���� �е�
	char			chServCertSign[128];			// 
} PACKET_SERV_CERT_RES;


typedef struct _PACKET_CERT_SIGN
{
	long			lCertVersion;					// ������ ����
	unsigned long	ulAgentID;						// byte ordering Padding or Agent ID
	char			szCertOwner[64];				// ������ ������ "CoreTrust License Issuer" ����
	char			szCertIssuer[64];				// ������ �߱��� "CoreTrust License Issuer" ����
	char			szPublicKey[PUB_KEY_LENGTH];	// ���� Public Key - DB�� ���� ����/����Ǹ� �ȵ�.
} PACKET_CERT_SIGN;


typedef struct _PACKET_AGENT_CERT_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	char			szMemberID[32];					// cspmember.mem_id (D&P��� HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P��� Home gateway password)
	char			szSTBID[20];					// MAC address �Ǵ� ��Ÿ STB�� ������ ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			szPadding[4];
} PACKET_AGENT_CERT_REQ;

// [2015.03.10. smpark] BO ���Կ� ���� ����ü �߰�.
typedef struct _PACKET_AGENT_CERT_REQ_V2
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	char			szMemberID[32];					// cspmember.mem_id (D&P��� HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P��� Home gateway password)
	char			szSTBID[20];					// MAC address �Ǵ� ��Ÿ STB�� ������ ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			szSUID[32];						// [2015.03.10. smpark] SUID
	char			szSTBOS[32];					// [2015.03.10. smpark] User Device OS
	char			szPadding[4];
} PACKET_AGENT_CERT_REQ_V2;


typedef struct _PACKET_PC_AGENT_CERT_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	char			szMemberID[32];					// cspmember.mem_id (D&P��� HDS sa ID)
	char			szMemberPWD[64];				// cspmember.mem_pwd (D&P��� Home gateway password)
	char			szSTBID[20];					// MAC address �Ǵ� ��Ÿ STB�� ������ ID (cspterminal.cspterminal_id)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	unsigned int 	ulCDID;							// 2009-10-29 PC Agent �� CDID
} PACKET_PC_AGENT_CERT_REQ;


typedef struct _PACKET_AGENT_CERT_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	long			lCertVersion;					// ������ ����
	unsigned long	ulAgentID;						// STB PK
	char			szCertOwner[64];				// ������ ������ "CoreTrust License Issuer" ����
	char			szCertIssuer[64];				// ������ �߱��� "CoreTrust License Issuer" ����
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key - DB�� ���� ����.
	unsigned long	ulSignLength;					// sign ����
	char			szPadding[12];					// AES ��ȣȭ�� ���� �е�
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
// Agent Image ������ Update ��� ������ ���� ����ü..
// 2007-07-01 ���� ����ü�� ����Ǿ� �ִ� ������
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
// VOD ���̼��� ��û ����ü
typedef struct _VOD_LIC_REQ_INFO
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	long			l_PinNumber;					// 
	long			l_PaymentType;					// 
	unsigned long	ul_ContentIDLength;				// �ڿ� �ٴ� sz_ContentID�� length
	char			sz_ContentID[MAX_CONTENT_ID];	// content.content_name
	char			sz_Padding[4];					// ��ȣȭ�� ���� 16 ����� ���߱�
} VOD_LIC_REQ_INFO, *pVOD_LIC_REQ_INFO;

// [2015.03.10. smpark] BO ���Կ� ���� VOD ���̼��� ��û ����ü �߰�.
typedef struct _VOD_LIC_REQ_INFO_V2
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	long			l_PinNumber;					// 
	long			l_PaymentType;					// 
	unsigned long	ul_ContentIDLength;				// �ڿ� �ٴ� sz_ContentID�� length
	unsigned char	*sz_ContentID;					// [2015.03.10. smpark] BO ���Կ� ���� �迭���� char * �� ����. content.content_name
	unsigned long	ul_OMSParamLen;					// [2015.03.10. smpark] BO ���Կ� ���� �Ķ���� ���� �߰�.
	unsigned char	*sz_OMSParam;					// [2015.03.10. smpark] BO ���Կ� ���� �Ķ���� �߰�.(����)
	// char			sz_Padding[16];					// ��ȣȭ�� ���� 16 ����� ���߱�
} VOD_LIC_REQ_INFO_V2, *pVOD_LIC_REQ_INFO_V2;


// client ���� �ؼ��ϴ� uc_EncryptType
//#define		CTDEC_ALGORITHM_AES			0xFF
//#define		CTDEC_ALGORITHM_RC4			0xFE
//#define		CTDEC_ALGORITHM_RC5			0xFD
//#define		CTDEC_ALGORITHM_SAY			0xFC
//#define		CTDEC_ALGORITHM_DEFAULT		CTDEC_ALGORITHM_AES
// VOD ���̼��� ���� ����ü
typedef struct _VOD_LIC_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	char			ch_Kc[16];						// ��ȣȭŰ.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// ���� define �ּ� ����.
	unsigned short	us_SaveDay;						// VoD ���̼��� ���� �ϼ� 0:����Ұ�.
	char			sz_Padding[1];					// ��ȣȭ�� ���� 16 ����� ���߱�
} VOD_LIC_RES_INFO, *pVOD_LIC_RES_INFO;


// Database���� �������� ���̼��� ���� ����ü ���� (bin->hexa ������)
// typedef struct _VOD_LIC_RES_INFO_IN_DB
// {
// 	VOD_LIC_INTEG_RES_INFO	lic_res;
// 	unsigned long	ul_LicenseID;					// license.license_pk
// 	char			ch_KcHexa[32];					// filecontent.filecontent_kc
// 	char			ch_ContentFormat[8];			// content.ctype_exe .XXXXXXXX �� ǥ���� ���ڿ�.
// } VOD_LIC_RES_INFO_IN_DB, *pVOD_LIC_RES_INFO_IN_DB; 


// VoD ���̼��� �߱� �α� ������ ���� ����ü.
// 
// 20081106 ���� License Log ���� Thread ���� �������� �����鼭, 
// Log DB ���� 12���� Connection�� ���ҽ�Ű�� �ʱ� ���� 
// ������ �߱� �α׵� �Ʒ� ����ü�� �̿��Ͽ� Log DB �� ����� �� �ְ� ��� ������.
//
typedef struct _VOD_LIC_LOG_INFO
{                                                   // VoD �߱޷α� �̿��                         ������ �߱޷α� �̿��
	unsigned char	ucNeedToWrite;					// reaload�߰� log���� operation�� �����ߴ��� ����. (if not zero : DB�� ����� �α�)
	unsigned long	ulAgentPK;						// cspterminal.cspterminal_pk                  Agent PK (�α�Thread���� ���)
	unsigned long 	ulLicensePK;					// license.license_pk                          null
	DWORD			dwResultCode;					// �߱� ����ڵ�.                              pubkeylog_state
	unsigned long 	ulFlag;							// ����:1 ����:0                               pubkeylog_new_flag, STB����:0 �߰�:1
	unsigned long 	ulLogFlag;                      // �αױ��� - 0���� ����                       1�� ����
	char			szClientIp[33];					// cspterminal.cspterminal_ip                  pubkeylog_stb_ip
	unsigned long	ulMemberPK;						// cspmember.mem_pk                            pubkeylog_mem_pk
	char			szMemberID[51];					// cspmember.mem_id   (D&P��� HDS sa ID)      pubkeylog_mem_id
	char			szMemberName[65];				// cspmember.mem_name (D&P��� �׻� null ��)   pubkeylog_mem_pwd	// [2015.05.08. smpark] BO ���Կ� ���� ���� ���� 64 -> 65.
	char			szSTBID[21];					// cspterminal.cspterminal_id.                 pubkeylog_stb_id
	char			szCPID[21];						// contentprivider.cp_id                       null
	char			szCPName[64];					// contentprivider.cp_name                     null
	char			szContentID[MAX_CONTENT_ID];	// content.content_name                        null
	char			szContentName[129];				// filecontent.filecontent_filename            null
	char			szLogDatetime[20];				// YYYY-MM-DD HH:Mi:SS ���̼��� �߱޽ð�.      pubkeylog_wdate
	char			szMessage[512];					// ���̼��� �߱� �޼���.                       null
	char			szSUID[51];						// �α� ��Ͻ� SUID �߰� ���
	float			dwProcessTime;					// [2017.06.26. smpark] ó�� �ð� �߰�.
	unsigned long	ulDeviceType;					// 0: skyLife, 1: OTP Player, 2: Pad TV, 3: Skylife Ű���
} VOD_LIC_LOG_INFO, *pVOD_LIC_LOG_INFO;



//////////////////////////////////////////////////////////////////////////////////////////////
// Live License
//////////////////////////////////////////////////////////////////////////////////////////////
// ���̼��� ����-���̼��� counter ��û
// size = 16
typedef struct _LIC_CHECK_REQ_INFO				// ���۽� ul_DecryptCheck ���ĸ� ��ȣȭ ��.
{
	unsigned long	ul_DecryptCheck;			// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	unsigned short	us_LicenseCounter;			// ��û�ϴ� STB�� license counter
	long			l_Nonce;					// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	char			sz_Padding[4];				// ��ȣȭ�� ���� 16 ����� ���߱�
} LIC_CHECK_REQ_INFO, *pLIC_CHECK_REQ_INFO;


// ���̼��� ���� ���� ����ü
// size = 10
typedef struct _LIC_CHECK_RES_INFO
{
	unsigned long	ul_DecryptCheck;			// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	unsigned short	us_LicenseCounter;			// ��û�ϴ� STB�� ���� DB���� license counter
	unsigned short	us_UpdateCount;				// �߰�/������ ���̼��� ����
	unsigned short	us_RemoveCount;				// ������ ���̼��� ����
//	us_RemoveCount ������ŭ unsigned long �� �ݺ��� ��
//	�� ������ ���� 16�� ����� ���߱� ���� ch_Padding���� ä���� �Ѵ�.
//	unsigned long	ul_LicenseID;				// license.license_pk
//	char			sz_Padding[4];				// ��ȣȭ�� ���� 16 ����� ���߱�
} LIC_CHECK_RES_INFO, *pLIC_CHECK_RES_INFO;


// ���̼��� ��û ����ü
typedef struct _LIVE_LIC_REQ_INFO
{
	unsigned long	ul_DecryptCheck;			// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	unsigned long	ul_LicenseID;				// license.license_pk
	long			l_Nonce;					// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	char			sz_Padding[4];				// ��ȣȭ�� ���� 16 ����� ���߱�
} LIVE_LIC_REQ_INFO, *pLIVE_LIC_REQ_INFO;


// ���̼��� ���� ����ü
// size = 80
typedef struct _LIVE_LIC_RES_INFO
{
	unsigned long	ul_DecryptCheck;	//                             ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	char			ch_CurKc[16];		// ServiceKey.servicekey_kc    ����Ű.
	char			ch_NextKc[16];		// ServiceKey.servicekey_kc    ����Ű.
	long			l_CurKcExpire;		// ServiceKey.servicekey_edate ����Ű ����ð�. (������� ���� ��)
	long			l_NextKcExpire;		// ServiceKey.servicekey_edate ����Ű ����ð�. (������� ���� ��)
	long			l_KcReqTime;		//                             ���� Ű ��û�ð�. (������� ���� ��)
	long			l_LicStartTime;		// license.rights_startdate    ���̼��� ��ȿ ���۽ð�.
	long			l_LicEndTime;		// license.rights_enddate      ���̼��� ��ȿ ����ð�.
	unsigned long	ul_LicenseID;		// license.license_pk
	unsigned long	ul_CAServiceID;		// caservice.caservice_pk
	unsigned long	uc_ContentFormat;	// channel.xxxxx               ��Ʈ�� ���� Format. ������ �⺻�� 1.
	unsigned char	uc_ChannelIP[4];	// channel.channel_ip          license_type=PPV �϶� channel output IP
	unsigned short	us_ChannelPort;		// channel.channel_port        license_type=PPV �϶� channel output Port
	unsigned char	uc_KcFlag;			// servicekey.servickey_flag   key toggle
	unsigned char	uc_LicenseType;		// license.license_type        CA Service Lic / PPV Lic ����
	char			sz_Padding[4];		//                             ��ȣȭ�� ���� 16 ����� ���߱�
} LIVE_LIC_RES_INFO, *pLIVE_LIC_RES_INFO;


// Database���� �������� ���̼��� ���� ����ü ���� (bin->hexa ������)
typedef struct _LIVE_LIC_RES_INFO_IN_DB
{
	LIVE_LIC_RES_INFO	lic_res;
	char			ch_CurKcHexa[32];
	char			ch_NextKcHexa[32];
} LIVE_LIC_RES_INFO_IN_DB, *pLIVE_LIC_RES_INFO_IN_DB;




// ���� ���� ����ü
typedef struct _ERROR_RES_INFO
{
	long			l_ErrorCode;				// �����ڵ�
	char			sz_ErrorMsg[512];			// ���� �޽���
} ERROR_RES_INFO, *pERROR_RES_INFO;


// Device License �߰�.
typedef struct _DEVICE_LICENSE
{
	char			szDeviceType[32];
	unsigned long	ulDeviceCount;
} DEVICE_LICENSE_INFO, *pDeviceLicenseInfo;


// [smpark, 140731] Unicast EMMG ���� ����ϴ� Protocol ���� ����ü �߰�.
//----------------------------------------------------------------
typedef struct _PACKET_EMM_REQ
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_IssuerID;					// ������ �߱��� Server ID
	unsigned long	ul_ServiceIDLength;				// �ڿ� �ٴ� sz_ServiceID�� length
	char			sz_ServiceID[MAX_CONTENT_ID];	// channel.channel_serviceid
	char			sz_Padding[16];					// ��ȣȭ�� ���� 16 ����� ���߱�
} PACKET_EMM_REQ, *pPACKET_EMM_REQ;

// ���̼��� ���� ����ü
// size = 80
typedef struct _PACKET_EMM_RES
{
	unsigned long	ul_DecryptCheck;	//                             ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	unsigned long	ul_CAServiceID;		// caservice.caservice_pk
	unsigned char	uc_PackageKeyFlag;	// packagekey.packagekey_flag   key toggle

	char			ch_CurKc[16];		// ServiceKey.servicekey_kc    packagekey�� ��ȣȭ�� ����Ű.
	char			ch_NextKc[16];		// ServiceKey.servicekey_kc    packagekey�� ��ȣȭ�� ����Ű.
	unsigned char	uc_ServiceKeyFlag;	// servicekey.servickey_flag   key toggle

	long			l_CurKcExpire;		// ServiceKey.servicekey_edate ����Ű ����ð�. (������� ���� ��)
	long			l_NextKcExpire;		// ServiceKey.servicekey_edate ����Ű ����ð�. (������� ���� ��)
	long			l_KcReqTime;		//                             ���� Ű ��û�ð�. (������� ���� ��)

	long			l_LicStartTime;		// license.rights_startdate    ���̼��� ��ȿ ���۽ð�.
	long			l_LicEndTime;		// license.rights_enddate      ���̼��� ��ȿ ����ð�.

	char			sz_Padding[9];		//                             ��ȣȭ�� ���� 16 ����� ���߱�
} PACKET_EMM_RES, *pACKET_EMM_RES;

// Database���� �������� ���̼��� ���� ����ü ���� (bin->hexa ��ȯ ����)
typedef struct _UEMMG_LIVE_LIC_RES_INFO_IN_DB
{
	PACKET_EMM_RES	lic_res;
	char			ch_CurKcHexa[32];
	char			ch_NextKcHexa[32];

	char			ch_CurPackageKcHexa[32];

	char			ch_CurPackageKcBin[16];
} UEMMG_LIVE_LIC_RES_INFO_IN_DB, *pUEMMG_LIVE_LIC_RES_INFO_IN_DB;



/* 1214 - 1215 ����
// EncKey Update ����ü
typedef struct _ENCKEY_REQ_INFO
{
	unsigned char	uc_X[16];
	int				n_SignSize;
	unsigned char   uc_Sign[128];
} ENCKEY_REQ_INFO, *pENCKEY_REQ_INFO;
*/


//-----------------------------------------------------------------------------
//		���� Type 
//-----------------------------------------------------------------------------
#define SUBSCRIBE_TYPE		0							// ������ (������)
#define PPV_TYPE			1							// PPV


// ���� �α��� ����. ��Ʈ ���� on/off ���� ������� define �Ǿ�� ��.
// ������Ʈ�� �� ���� : 1100 : critical, error �� �α� �ǽ�
// ������Ʈ�� �� ���� : 1110 : critical, error, warning���� �α� �ǽ�
// ������Ʈ�� �� ���� : 1111 : critical, error, warning, notice ���� �α� �ǽ�
#define		LOG_CRITICAL	0x00000080		// �ɰ��� ����. ���� �ߴܵ�.
#define		LOG_ERROR		0x00000040		// ����. ��Ȳ�� ���� �ﰢ ��ó�ʿ�.
#define		LOG_WARNING		0x00000020		// ���.
#define		LOG_NOTICE		0x00000010		// �˸�. �˾ƵѸ��� ��Ȳ
#define		LOG_INFO		0x00000008		// �˸�. �߿����� ���� ������
#define		LOG_DEBUG		0x00000004		// �⺻ �����
#define		LOG_DEBUG2		0x00000002		// �����2
#define		LOG_DEBUG3		0x00000001		// �����3

#define		SUCCESS_LICENSEISSUER			0x10000000	// ���� 
#define		ERROR_LICENSEISSUER				0x20000000	// ���� 

#define		ERROR_PROTOCOL_VERSION			0x20000027	// protocol version�� PROTOCOL_VERSION define�� ������.
#define		ERROR_PROTOCOL_ID				0x20000028	// protocol id�� PROTOCOL_ID define�� ������.
#define		ERROR_PROTOCOL_COMMAND			0x20000029	// protocol CMD�� ���ǵ� cmd�� �ƴ�.
#define		ERROR_PROTOCOL_DATA_SIZE		0x20000030	// protocol cmd�� ���� �ʿ��� ������ ����� Ʋ��.
#define		ERROR_PROTOCOL_DECRYPTION		0x20000031	// ��ȣȭ�� �������� ����ü�� ��ȣȭ ����.
//������� ����.#define		ERROR_PROTOCOL_FLOW				0x20000032	// (���� request�� �ʿ��� ��Ȳ����) ����Ʈ�� �帧 ����.  


#define		ERROR_DATABASE_EXCEPTION 					0x20000002
#define		ERROR_CSPMEMBER_NOT_EXIST					0x20000010		// INIT_REQ �� ���� mem_id �� �������� ����.
#define		ERROR_CSPTERMINAL_ID_ALREADY_EXIST			0x20000033
#define		ERROR_CSPTERMINAL_AUTHKEY_EXPIRED			0x20000034

#define		ERROR_CSPMEMBER_STATE_DISABLE				0x20000004
#define		ERROR_CSPTERMINAL_STATE_DISABLE				0x20000005
#define		ERROR_CSPCONTENT_NOT_EXIST					0x20000006
#define		ERROR_CONTENT_STATE_DISABLE					0x20000007
#define		ERROR_CSPCONTENTLIST_STATE_DISABLE			0x20000008
#define		ERROR_CSPINFO_NOT_EXIST						0x2000000C		
#define		ERROR_CSPINFO_STATE_DISABLE					0x2000000D
// freeon �߰�
#define		ERROR_DEVICE_STATE_DISABLED					0x2000000E		// STB_type �� ���Ͽ� license_device ���̺� ������ ���ų�, status disable ����
#define		ERROR_DEVICE_ID_DISABLED					0x20000022		// STB_type �� ���Ͽ� license_device ���̺� ������ ���ų�, status disable ����


#define		ERROR_PK_AUTHKEY_ERROR						0x20000024
#define		ERROR_DIGITAL_SIGN_FAIL						0x20000026

#define		ERROR_MIDDLEWARE_LINK						0x20000035
#define		ERROR_CONTENT_NOT_EXIST						0x20000036		
#define		ERROR_LICENSE_NOT_EXIST						0x20000037		// ������ ���̼��� ��û�� ���̼����� ����.
#define		ERROR_LICENSE_STATE_DISABLE					0x20000038
#define		ERROR_LICENSE_NOT_ACTIVATED					0x20000039
#define		ERROR_LICENSE_EXPIRED						0x20000040
#define		ERROR_LICENSE_INFO_NOT_EXIST				0x20000041		// ����/����� ���̼��� ����.

#define		ERROR_AUTH_REQUESTER_CONNECT				0x20001401		// KT-D&P HDS ���� ���� ���� ����
#define		ERROR_AUTH_REQUESTER_FAIL					0x20001402		// KT-D&P HDS ���� ����

#define		ERROR_MIDDLEWARE_REQUEST_FAIL				0x20001403		// KT-D&P ���հ��� stored-procedure ���� ����
#define		ERROR_MIDDLEWARE_DB_EXCEPTION				0x20001404		// KT-D&P ���հ��� stored-procedure ���� ����
#define		ERROR_MIDDLEWARE_RESPONSE_TIMEOUT_FAIL		0x20001405		// [2017.08.30. smpark] OMS ������ ���� ����� Timeout �߻��� ���� ���� �ڵ� �߰�.

#define		ERROR_CDID_NOT_EXIST						0x20001407		// CDID�� cdid ���̺� �������� �ʾ� ������ �߱޺Ұ�.	// [2017.08.30. smpark] OMS ���� Timeout �� �߰���, 0x20001405 --> 0x20001407 �� ����.
#define		ERROR_CDID_CHECK_FAIL						0x20001406		// CDID�� ��ȣȭ�� ��Ʈ���� db�� cdid ��ȣȭ ��Ʈ���� �����Ͽ� ������ �߱޺Ұ�.

#define		ERROR_IMCS_PACKAGE_NOT_EXIST				0x20001501		// LG-Dacom ���������� ��û�� ��Ű�� ����.
#define		ERROR_IMCS_SUBSCRIBE_NOT_EXIST				0x20001502		// LG-Dacom ���������� ��û�� �������� ����.

#define		ERROR_IMCS_USER_NOT_EXIST					0x20001503		// LG-Dacom PPV ��û�� ����� ���� ����
#define		ERROR_IMCS_CONTENT_NOT_EXIST				0x20001504		// LG-Dacom PPV ��û�� ������ ���� ����
#define		ERROR_IMCS_PPV_SUBSCRIBE_NOT_EXIST			0x20001505		// LG-Dacom PPV ��û�� ������ ���� ����


#define		ERROR_AMOC_HTTP_SERVER_CONNECT_FAIL			0x20002401		// AMOC ���� ���� ����
#define		ERROR_AMOC_REQUESTER_FAIL					0x20002402		// AMOC ���� ����
#define		ERROR_AMOC_REQUESTER_TIMEOUT_FAIL			0x20002403		// AMOC ���� ����. Timeout
#define		ERROR_AMOC_TICKET_VALIDATE_FAIL				0x20002404		// AMOC Ƽ����ȿ�� ���� ����.

// [2017.06.25. smpark] KT-CAS ���� ���� �ڵ� �߰�.
#define		ERROR_KTCAS_CONNTECTION_FAIL				0x20003401		// KT-CAS ���� ����.
#define		ERROR_KTCAS_REQUEST_FAIL					0X20003402		// KT-CAS ���� ����


//-------- DRM Server License ���� ����
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
// freeon ���� protocol �����ڵ� �߰�
#define		ERROR_INVALID_SERVICE_ID					0x30000001
#define		ERROR_INVALID_AUTH_INFO						0x30000002
#define		ERROR_INVALID_CONTENT_INFO					0x30000003
#define		ERROR_INVALID_CRC							0x30000004	// CRC Check fail.

//	CMD_WEB_LI_PARAM	Protoco Error
#define		ERROR_INVALID_SECRET_KEY					0x30000004

// [2015.03.11. smpark] BO ���Կ� ���� OMS �Ķ���� ���� �ڵ� �߰�.
#define		ERROR_INVALID_OMSPARAM						0x30000005


#define		LICENSE_FILE_SIZE	320 // [smpark, 140729] IPTV2.0.0 ���� ���̼��� ������ ����� 128���� 320���� �����.
#define		ERROR_COMPOSITION_LICENSE_DISABLE			0x90000001  // Composition License ��� �Ұ� ����.

#define		ERROR_LICENSEISSUER_CONNECT_FAIL			0x80000001	// Proxy Server ���� License issuer ���� ���� ����.
#define		ERROR_PROXY_NOT_EXIST						0x80000002	// Proxy Server ������ ���� ���� ����
#define		ERROR_PROXY_DISABLED						0x80000003	// Proxy Server �� ��Ȱ�� �Ǿ� �ִ� ����

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
//-------- ���� �޽��� �߰�
#define		ERROR_LICENSETERMINAL_DISABLE				0x20000013
#define		ERROR_LICENSE_ISSUE_COUNT_DISABLE			0x20000014
#define		ERROR_LICENSE_ISSUE_TERM_DISABLE			0x20000015
#define		ERROR_LICENSE_ISSUE_KNOWN					0x20000016
#define		ERROR_LICENSE_ISSUE_SDATE_TERM_DISABLE		0x20000017
//-------- ���� �޽��� �߰� kyooni
#define		ERROR_DRMSERVER_KEY_PAIR_NOT_EXIST			0x20000019
#define		ERROR_TERMINAL_KEY_PAIR_NOT_EXIST			0x20000020
#define		ERROR_TERMINAL_AUTH							0x20000021
#define		ERROR_PK_AUTH								0x20000022
//-------- ���� �޽��� �߰� jong myoung
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
	unsigned long ulSignLength;						// Sign ����

	short shServicePort;							// ���� ��Ʈ��ȣ

	char szDBProvider[MAX_PATH];					// ADO DB Connection ���� 
	char szDBProviderMW[MAX_PATH];					// ADO DB Connection ���� 
	// freeon
	// OMS DB Connection ����.
	char szDBProviderOMS[MAX_PATH];

	// [2015.03.09. smpark] BO ���Կ� ���� ���� ���� ����
	char	szDBProviderOTM[MAX_PATH];			// �ű� OTM DB Connection string.
	char	szDBProviderMIPG[MAX_PATH];			// �ű� OTT(MIPG) DB Connection string.

	char szDBProviderLOG[MAX_PATH];				// ADO Log DB Connection ���� 

	//------------------------------------------------------------
	//		�Ͽ��� �߰� : STB ��� ���̼����� ���� ���� 
	//------------------------------------------------------------
	long	lServerLicense_STBCount;
	long	lServerLicense_UseSTBCount;
	long	lServerLicense_DBSTBCount;
	DWORD	dwLicenseState;
	//------------------------------------------------------------
	//		������ �߰� : STB GARBAGE ��� ���̼����� ���� ���� 
	//------------------------------------------------------------
	long	lServerLicense_GarbageMaxCount;
	long	lServerLicense_LastUpdateDate;
	long	lServerLicense_GarbageTerminlaCount;

	long	l_auth_server_check_flag;				// KT:������������ ���� ����, Dacom:HTTP-Get ���� ���� ���� 
	char	sz_auth_server_webservice_name[256];	// KT:������������ WebService Name, Dacom:HTTP-Get ���� ���� Web URL

	char szL4_IP1[32];								// IPS Server�� Heart-bit�� üũ�ϴ� L4 ����� IP
	char szL4_IP2[32];								// �״���� ����
	char szL4_IP3[32];								// 
	char szL4_IP4[32];								// 

	// DNP
	long	l_ppv_check_flag;							// PPV�� ���� Middeware ���� ���� 
	long	l_subscribe_check_flag;						// �������� ���� Middleware ���� ����
	char	sz_middleware_db_ip[32];					// Middleware DB IP
	char	sz_middleware_db_catalog[64];				// Middleware DB Catalog
	char	sz_middleware_db_user[32];					// Middleware DB User
	char	sz_middleware_db_pw[32];					// Middleware DB Password
	char	sz_middleware_stored_procedure_name[64];	// Middleware Stored Procedure Name
	long	l_configuration_ver;						// DRM Server ȯ�� ���� ����  (ctsinfo.IPS_Policy_counter �÷�)

	// freeon
	// OMS AS
	long	l_oms_ppv_check_flag;
	long	l_oms_subscribe_check_flag;
	char	sz_oms_db_ip[32];
	char	sz_oms_db_catalog[64];
	char	sz_oms_db_user[32];
	char	sz_oms_db_pw[32];
	char	sz_oms_db_stored_procedure_name[64];

	// [2015.03.09. smpark] - BO ���Կ� ���� OTM ���� ���� �߰�.
	// OTM
	short	sh_otm_integ_fixed_flag;
	short	sh_otm_integ_ppv_flag;
	char	sz_otm_db_ip[32];
	short	sh_otm_db_Port;
	char	sz_otm_db_catalog[64];
	char	sz_otm_user_id[32];
	char	sz_otm_user_pwd[32];
	char	sz_otm_sp_name[64];

	// [2015.03.09. smpark] - BO ���Կ� ���� OTT ���� ���� �߰�.
	// OTT	
	short	sh_ott_integ_fixed_flag;
	short	sh_ott_integ_ppv_flag;
	char	sz_ott_db_ip[32];
	short	sh_ott_db_Port;
	char	sz_ott_db_catalog[64];
	char	sz_ott_user_id[32];
	char	sz_ott_user_pwd[32];
	char	sz_ott_sp_name[64];

	// [2015.03.09. smpark] - BO ���Կ� ���� �����(�ܸ�) ���� ���� ���� �߰�.
	long	l_auth_integ_check_flag;			// ������������ ���� ����
	char	sz_auth_integ_webservice_url[256];	// ������������ WebService URL

	// [2022.05.19. mgsong] - OpenCAS-MEIN������ ���� �����(�ܸ�) ���� ���� ���� �߰�.
	long	l_auth_integ_mein_check_flag;			// MEIN�������� ���� ����
	char	sz_auth_integ_mein_url[256];	// MEIN�������� WebService URL

	/// OTP ���ſ����� ���� url �ּ� �߰� �ʿ�?
	// [2017.02.24. smpark] KT �����ӹ����� OTT ���� AMOC ���� URL
	char	sz_amoc_integ_url[256];	///< ������������ WebService URL @details ������������
	char	sz_amoc_integ_ocas_url[256];	///< ������������ WebService URL @details Ƽ����ȿ������
	int	sh_amoc_timeout;			// ������������ ���� Timeout(ms)

	// [2017.04.04] KT-CAS ������ ���� ���� ����
	char sz_apppath[128];			// KT-CAS �� ������ ���� CA, Client �������� �ִ� ��ġ.

	char sz_ktcas_hostname[256];
	int  sh_ktcas_port;

	char sz_ktcas_back_hostname[256];	// [2017.06.25. smpark] KT-CAS 2�� ���� IP
	int  sh_ktcas_back_port;			// [2017.06.25. smpark] KT-CAS 2�� ���� Port

	int sh_ktcas_con_timeout;			// [2017.07.21. smpark] VKS Connection Timeout
	int sh_ktcas_recv_timeout;			// [2017.07.21. smpark] VKS Receive Timeout


	// Host Name - for Process Up/Down Log
	char host_name[32];				// Host Name
	char server_name[51];			// Server ID (LI �� ���� ���� ID �� �����Ƿ� HostName�� ����.
	char process_event[51];			// SERVICE START,  SERVICE STOP
	char process_message[257];		// 

	// KT �� ������ Channel ���񽺰� �����Ƿ� ����.
	// Emergency
	// bool	bFileDBMode;	
	// char	szBaseDateTime[32];		// [smpark, 140731] Unicast EMMG���� �����ϴ� ������ Service Key �߱��� ���� �ð�. (DB ��� �ð�)

	// Device License �߰�.
	//CPtrList		*ptrDeviceList;

	unsigned int		gDeviceTypeCount;
	DEVICE_LICENSE_INFO	gDeviceLicenseArray[MAX_DEVICE_LIST];

	// [smpark, 140730] Composition License �� ���� ���� �߰�.
	char szComLicFilePath[MAX_PATH];
	bool bFlagPackageKeyIssuer;
	bool bFlagVODLicenseIssuer;
	bool bFlagChannelKeyIssuer;
	bool bFlagAppleKeyIsser;

	/// [2022.06.16. mgsong] - OpenCAS-MEIN���� HTTP Request�� ����� ����
	char mein_appkey[32 + 1];	///< HTTP Request ��� applicationKey��	

} DRM_SERVER_INFO, *PDRM_SERVER_INFO;

// KT ������ CAS ��� ���� ���� �߰�. addby dhseo 2022.12.07
typedef struct _EMERG_MODE_INFO
{
	bool bUseEmerg;												// Emergency Mode ��� ����
	char szDBProvider[MAX_PATH];								// Emergency DB ����
	char szDBIP[64];											// Emergency DB IP
	char szDBPort[16];											// Emergency DB Port
	bool bIsConnect;											// Emergency DB ���� ����
	bool bDBReConnect[CREATED_COMPLETION_THREAD_COUNT];			// Emergency DB ������ ���� �Ǿ����� �� ���� Ȯ�� ����
	bool bLogDBReConnect[CREATED_COMPLETION_THREAD_COUNT];		// Emergency Log DB ������ ���� �Ǿ����� �� ���� Ȯ�� ����
	bool bUseFileLog;											// Log ���� ���� ����
	char szFileLogPath[MAX_PATH];								// Log ���� ���
	char szLogDBProvider[MAX_PATH];								// Emergency Log DB ����
} EMERG_MODE_INFO;
// === 2022.12.07

//------------------------------------------------------------------------------------------------------------------------------------------------
// freeon : 
// ���� �������ݿ� ����ü �� ���� ����
//------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct _PACKET_HEADER_INTEG					// header�� ���۽� ��ȣȭ ���� ����.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID �� ���  0x1234 : ������.
	unsigned short	sh_ProtocolVer;				// �������� ����  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 �� ��� ��ϵ��� ���� �ʱ����)
	unsigned short	us_ProtocolCommand;			// ��ɾ� �� �����ڵ�.
	unsigned short	us_ProtocolDataSize;		// ���� ������ ũ��.
	short			s_Nonce;
	unsigned short	us_crc;						// CRC Check
} PACKET_HEADER_INTEG;

typedef struct _PACKET_INIT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	unsigned char	uc_X[16];						// Key ��ȯ �˰��� : X ���� �� y �۽�
}PACKET_INIT_INTEG_REQ;

typedef struct _PACKET_INIT_INTEG_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned char	uc_Y[16];						// Key ��ȯ �˰��� : X ���� �� y �۽�
}PACKET_INIT_INTEG_RES;

typedef struct _PACKET_SERV_CERT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
}PACKET_SERV_CERT_INTEG_REQ;

typedef struct _PACKET_SERV_CERT_INTEG_RES			// ���۽� ul_DecryptCheck ���ĸ� ��ȣȭ ��.
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	lCertVersion;						// ������ ����
	char			szPadding[4];					// byte ordering Padding
	char			szCertOwner[64];				// ������ ������ "CoreTrust License Issuer" ����
	char			szCertIssuer[64];				// ������ �߱��� (Server ID)
	char			szServPubkey[PUB_KEY_LENGTH];	// ���� Public Key - DB�� ���� ����/����Ǹ� �ȵ�.
	unsigned long	ulSignLength;					// sign ����
	char			szPadding2[12];					// AES ��ȣȭ�� ���� �е�
	char			chServCertSign[128];			// 
} PACKET_SERV_CERT_INTEG_RES;

typedef struct _PACKET_AGENT_CERT_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device ���� ����
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key cspterminal.cspterminal_publickey.
	char			sz_DeviceType[32];				// Device Type, MKT���� �߱�
	//char			sz_DeviceSerial[32];	
} PACKET_AGENT_CERT_INTEG_REQ;

typedef struct _PACKET_AGENT_CERT_INTEG_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	long			lCertVersion;					// ������ ����
	unsigned long	ulAgentID;						// STB PK
	char			szCertOwner[64];				// ������ ������ "CoreTrust License Issuer" ����
	char			szCertIssuer[64];				// ������ �߱��� (Server ID)
	char			szAgentPubKey[PUB_KEY_LENGTH];	// Agent Public Key - DB�� ���� ����.
	unsigned long	ulSignLength;					// sign ����
	char			szPadding[12];					// AES ��ȣȭ�� ���� �е�
	char			chServCertSign[128];			// 
} PACKET_AGENT_CERT_INTEG_RES;

typedef struct _PACKET_CONFIG_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	char			sz_DeviceType[32];				// Device Type, MKT���� �߱�
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
	// DB SP ���� �ʿ��� �� 
	unsigned int	usPreViewTime;					// �̸����� �ð�
	char			chEMMMFlag;						// MulticastEMM 0:�̻�� 1:���
	char			chUEMMGFlagOnError;				// MulticastEMM������ UEMM 0:�̻�� 1:���
	char			chPadding;
} PACKET_CONFIG_INTEG_RES;

typedef struct _PACKET_SECRET_KEY_INTEG_REQ
{
	unsigned long 	ulDecryptCheck;
	long			lNonce;
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device ���� ����
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
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	unsigned long	ul_ContentIDLength;				// �ڿ� �ٴ� sz_ContentID�� length
	unsigned char	*sz_ContentID;					// content.content_name
	unsigned long	ul_IssuerID;
} VOD_LIC_INTEG_REQ_INFO, *pVOD_LIC_INTEG_REQ_INFO;

typedef struct _VOD_LIC_INTEG_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	char			ch_Kc[16];						// ��ȣȭŰ.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// ���� define �ּ� ����.
	unsigned char	us_SaveDay;						// VoD ���̼��� ���� �ϼ� 0:����Ұ�.
	char			sz_Padding[9];					// ��ȣȭ�� ���� 16 ����� ���߱�
} VOD_LIC_INTEG_RES_INFO, *pVOD_LIC_INTEG_RES_INFO;

// typedef struct _VOD_LIC_INTEG_RES_INFO_IN_DB
// {
// 	VOD_LIC_INTEG_RES_INFO	lic_res;
// 	unsigned long	ul_LicenseID;			// license.license_pk
// 	char			ch_KcHexa[32];			// filecontent.filecontent_kc
// 	char			ch_ContentFormat[8];	// content.ctype_exe .XXXXXXXX �� ǥ���� ���ڿ�.
// 	unsigned short  us_zero;				// �߿�!! ch_ContentFormat �� string_to_hex ó���� �� null-terminaltor ����  !!
// 	unsigned short	us_PackagePullFlag;		// IMCS_packagelist.product_type �� �ش��ϴ� ���� �÷���
// 	unsigned short	us_DevicePullFlag;		// license_device.pull_flag
// 	char			sz_DevicePullURL[256];	// license_device.pull_url
// } VOD_LIC_INTEG_RES_INFO_IN_DB, *pVOD_LIC_INTEG_RES_INFO_IN_DB; 

// Database���� �������� ���̼��� ���� ����ü ���� (bin->hexa ������)
typedef struct _VOD_LIC_RES_INFO_IN_DB
{
	VOD_LIC_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];					// filecontent.filecontent_kc
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX �� ǥ���� ���ڿ�.	// [2016.12.15. smpark] Null ���� ó���� ���� �迭 1 ����.
} VOD_LIC_RES_INFO_IN_DB, *pVOD_LIC_RES_INFO_IN_DB;

typedef struct _VOD_LIC_INTEG_RES_INFO_IN_DB
{
	VOD_LIC_INTEG_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];				// filecontent.filecontent_kc	// [2016.12.15. smpark] Null ���� ó���� ���� �迭 1 ����.
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX �� ǥ���� ���ڿ�.	// [2016.12.15. smpark] Null ���� ó���� ���� �迭 1 ����.
} VOD_LIC_RES_INTEG_INFO_IN_DB, *pVOD_LIC_RES_INTEG_INFO_IN_DB;

// [2016.12.13. smpark] VOD �̸����� Response �߰�
typedef struct _VOD_PREVIEW_LIC_INTEG_RES_INFO
{
	unsigned long	ul_DecryptCheck;				// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			l_Nonce;						// ��ȣȭ�� ���� �Ҷ����� �ٸ�����. ���� ��
	// char			ch_Kc[16];						// ��ȣȭŰ.
	char			ch_Kc[17];						// ��ȣȭŰ.
	unsigned long	ul_ScrambleLevel;				// 0:not scrambled, 1:video only, 2:audio only, 3:all
	unsigned long	ul_ContentFormat;				// 
	unsigned char	uc_EncryptType;					// ���� define �ּ� ����.
	unsigned long	ul_PreviewTimeSec;				// VoD �̸����� �ð�.(��)
	char			sz_Padding[9];					// ��ȣȭ�� ���� 16 ����� ���߱�
} VOD_PREVIEW_LIC_INTEG_RES_INFO, *pVOD_PREVIEW_LIC_INTEG_RES_INFO;

typedef struct _VOD_PREVIEW_LIC_INTEG_RES_INFO_IN_DB
{
	VOD_PREVIEW_LIC_INTEG_RES_INFO	lic_res;
	unsigned long	ul_LicenseID;					// license.license_pk
	char			ch_KcHexa[32 + 1];					// filecontent.filecontent_kc
	char			ch_ContentFormat[8 + 1];			// content.ctype_exe .XXXXXXXX �� ǥ���� ���ڿ�.
} VOD_PREVIEW_LIC_RES_INTEG_INFO_IN_DB, *pVOD_PREVIEW_LIC_RES_INTEG_INFO_IN_DB;
///////////////////////////////////////////////////////

// freeon 
// Web License Issuer ���� ��ȭ�� ���� �߰��� ��������
typedef struct _PACKET_WEB_LI_PARAM_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ServiceIDSize;				// SerivceID Length
	unsigned char	*sz_ServiceID;					// ���������� ServiceID�� �ޱ�����
	unsigned long	ul_AuthInfoSize;
	unsigned char	*sz_AuthInfo;					// User, Device ���� ����	
} PACKET_WEB_LI_PARAM_REQ;

typedef struct _PACKET_WEB_LI_PARAM_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_KeyServerURLSize;
	unsigned char	*sz_KeyServerURL;
	unsigned long	ul_EncParameterSize;			// EncParameter Size
	unsigned char	*sz_EncParameter;				// SecretKey �� ��ȣȭ�� �Ķ���� ����
} PACKET_WEB_LI_PARAM_RES;

// [2016.12.13. smpark] VOD �̸����� ��� �߰��� ���� WEB_LI_PARAM �� �̸�����(��) �߰�
typedef struct _PACKET_WEB_LI_PREVIEW_PARAM_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_KeyServerURLSize;
	unsigned char	*sz_KeyServerURL;
	unsigned long	ul_EncParameterSize;			// EncParameter Size
	unsigned char	*sz_EncParameter;				// SecretKey �� ��ȣȭ�� �Ķ���� ����
	unsigned int	ul_preview_time;				// [2016.12.13. smpark] �̸����� �ð�(��) �߰�
} PACKET_WEB_LI_PREVIEW_PARAM_RES;

// [2015.03.11. smpark] BO ���Կ� ���� OMSParam ������ ������ ����ü 
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

// [2015.03.11. smpark] BO ���Կ� ���� define �߰�.
#define	SERVICE_TYPE_OTT				"OTT"
#define	SERVICE_TYPE_OTM				"OTM"
#define	SERVICE_TYPE_OM					"OM"
#define	SERVICE_TYPE_KIBOT				"KIBOT"

#define	SERVICE_ID_FOR_KTOTN2			"KT-OTN-02"
#define SERVICE_ID_FOR_KTOTN2_LEN		9
#define	SERVICE_ID_FOR_KTOTN2_VALUE		0x00030102			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOTN3			"KT-OTN-03"			// KT OTT �����ӹ����� ��.
#define SERVICE_ID_FOR_KTOTN3_LEN		9
#define	SERVICE_ID_FOR_KTOTN3_VALUE		0x00030103			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOTN4			"KT-OTN-04"			// KT OTP Player ��.
#define SERVICE_ID_FOR_KTOTN4_LEN		9
#define	SERVICE_ID_FOR_KTOTN4_VALUE		0x00030104	

#define	SERVICE_ID_FOR_KTOTN5			"KT-OTN-05"			// KT �ʵ�� ��
#define SERVICE_ID_FOR_KTOTN5_LEN		9
#define	SERVICE_ID_FOR_KTOTN5_VALUE		0x00030105			// reserved(2) site(2) service(2) version(2)

#define	SERVICE_ID_FOR_KTOCAS			"KT-OCAS-01"		// [20220406 mgsong] OCAS (�ű� AMOC ����)
#define SERVICE_ID_FOR_KTOCAS_LEN		10
#define	SERVICE_ID_FOR_KTOCAS_VALUE		0x00030106			// reserved(2) site(2) service(2) version(2)


// AuthInfo �� ContentID type �� ��ȿ�� �˻� �ϱ� ���� ����
#define AUTHINFO_IPTV					1
#define AUTHINFO_NSCR					2
#define CONTENT_ID_IPTV_CH				1
#define CONTENT_ID_IPTV_VOD				2
#define CONTENT_ID_NSCR_CH				0
#define CONTENT_ID_NSCR_VOD				3

#define CONTENT_ID_VOD					5 // 2014.12.03. By smpark. ex) PIN,Payment,Content ID,Album ID, Content Format, ASSET ID

// [smpark, 140730] Composition License ���� �߰�.
#define	PACKAGE_KEY_ISSUER	0x00000001
#define VOD_LICENSE_ISSUER	0x00000002
#define CHANNEL_KEY_ISSUER	0x00000003
#define APPLE_KEY_ISSUER	0x00000004

/* [2017.02.14. smpark] Proxy ����ó���� ���� �޽��� */
typedef struct _PROXY_INIT_REQ
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ProxyNameSize;				// ProxyName  Length
	unsigned char	*sz_ProxyName;					// ���������� ProxyName�� �ޱ�����
	unsigned char	uc_X[16];						// Key ��ȯ �˰��� : X �� �����Ͽ� ����. ���� ���ŵǴ� Y �� �����Ͽ� Session Key ����.
}PROXY_INIT_REQ;

typedef struct _PROXY_INIT_RES
{
	unsigned long 	ulDecryptCheck;					// ��ȣȭ �������� �Ǵ��� ���� ������. 0xCC12.
	long			lNonce;							// ��ȣȭ�� ���� ���� �Ҷ����� �ٸ�����. ���� ��
	unsigned long	ul_ProxyID;						// proxyserverlist.proxyserver_pk 
	unsigned char	uc_Y[16];						// Key ��ȯ �˰��� : X ���� �� y �۽�
}PROXY_INIT_RES;

// [2017.04.06. smpark] KT-CAS ������ ���� �޽��� ����
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

