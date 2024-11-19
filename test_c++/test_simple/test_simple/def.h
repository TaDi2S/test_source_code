#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <MSWSock.h>

#include <iostream>
#include <stdio.h>
#include <minwindef.h>
#include <atlstr.h>

#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024


#define		BUFFER_SIZE							16384
#define		CMD_INFO_REQ						0x4301		// [2024.10.16. dykim] Splunk ���� ��û �������� �߰�
#define		CMD_INFO_RES						0x4302		// [2024.10.16. dykim] Splunk ���� ���� �������� �߰�
#define		PROTOCOL_ID							0x1234

#define		SUCCESS_LICENSEISSUER							0x10000000	// ���� 
#define		ERROR_DRMSERVER_LICENSE						0x20000023
#define		ERROR_CSPTERMINAL_NOT_EXIST					0x2000000F
#define		ERROR_DRMSERVER_ID_NOT_EXIST				0x20000018

#define		CMD_IMG_UPDATE_RES					0x4003
#define		CMD_CERT_UPDATE_RES					0x4004
#define		CMD_SECRET_KEY_UPDATE_RES		0x4005
#define		CMD_DEVICE_ID_DISABLE_RES			0x4006

#define		ERROR_DEVICE_STATE_DISABLED				0x2000000E		// STB_type �� ���Ͽ� license_device ���̺� ������ ���ų�, status disable ����
#define		ERROR_DEVICE_ID_DISABLED					0x20000022		// STB_type �� ���Ͽ� license_device ���̺� ������ ���ų�, status disable ����
#define		PROTOCOL_VERSION					4
// 2007-10-05 : add 
#define		PROTOCOL_VERSION_5					5

#define BOOL	int					//	1-bit data type
#define BYTE	unsigned char		//	unsigned 1-byte data type
#define WORD	unsigned short int	//	unsigned 2-bytes data type

// db ���� null�� ��� ���鹮�ڿ��� return.
#define NULL_2_EMPTY(xx) ( xx.vt == VT_NULL ? "" : _bstr_t(xx) )
// db ���� null�� ��� 0 ���� return.
#define NULL_2_ZERO(xx)  ( xx.vt == VT_NULL ? 0 : xx.intVal )
typedef unsigned long       DWORD;

#define		ERROR_DATABASE_EXCEPTION 						0x20000002
#define		ERROR_CSPMEMBER_NOT_EXIST						0x20000010		// INIT_REQ �� ���� mem_id �� �������� ����.
#define		ERROR_CSPTERMINAL_ID_ALREADY_EXIST			0x20000033
#define		ERROR_CSPTERMINAL_AUTHKEY_EXPIRED			0x20000034


typedef struct _PACKET_INFO_RES
{
	unsigned int	ul_SplunkFlag;			//Splunk ���� Flag (1: ���, 0:�̻��)
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl�� ����
	unsigned int	ul_ReservedLen;		// sz_Reserved�� ����
	unsigned int	ul_BaseGenreLen;		// sz_BaseGenre�� ����
	unsigned int	ul_EMMCacheFlag;		//EMM cache ��� flag (1:���, 0: �̻��)
	unsigned int	ul_OperationMode;	// ����: ( 1: ���, 0: ����)
	char				*sz_BaseGenre;			// �ʼ���ûä�� �帣 �ڵ� �迭
	char				*sz_SplunkUrl;			// Splunk ���� URL ��
	char				*sz_Reserved;			// reserved -> Ȯ�强�� ���� �ӽ÷� ��Ƶ� ��
} PACKET_INFO_RES;
// [2024.10.16 dykim] Splunk ���� ������ ���� �������ݿ� ����ü �߰�
// �׽�Ʈ��
typedef struct _PACKET_INFO_RES_TEST1
{
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl�� ����
	unsigned char	*sz_SplunkUrl;			// Splunk ���� URL ��
	unsigned int	ul_BaseGenre;			// �⺻ ä�� �帣 Code ��
	unsigned int	ul_ReservedLen;			// sz_Reserved�� ����
	unsigned char	*sz_Reserved;			// reserved
} PACKET_INFO_RES_TEST1;

// ������ �ڷ� �̷��
typedef struct _PACKET_INFO_RES_TEST2
{
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl�� ����	
	unsigned int	ul_BaseGenre;			// �⺻ ä�� �帣 Code ��
	unsigned int	ul_ReservedLen;			// sz_Reserved�� ����
	unsigned char* 	sz_SplunkUrl;			// Splunk ���� URL ��
} PACKET_INFO_RES_TEST2;

typedef struct _PACKET_HEADER					// header�� ���۽� ��ȣȭ ���� ����.
{
	short				sh_ProtocolID;				// CoreTrust Protocol ID �� ���  0x1234 : ������.
	unsigned short	sh_ProtocolVer;				// �������� ����  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 �� ��� ��ϵ��� ���� �ʱ����)
	unsigned short	us_ProtocolCommand;			// ��ɾ� �� �����ڵ�.
	unsigned short	us_ProtocolDataSize;		// ���� ������ ũ��.
} PACKET_HEADER;


//-----------------------------------------------------------------------------
// struct SOCKET_OBJ
//-----------------------------------------------------------------------------
typedef struct _SOCKET_OBJ
{
	WSAOVERLAPPED        ol;

	SOCKET               s;									// Socket handle

	LPFN_ACCEPTEX				lpfnAcceptEx;					// pointer to AcceptEx function
	LPFN_GETACCEPTEXSOCKADDRS	lpfnGetAcceptExSockaddrs;		// pointer to GetAcceptExSockaddrs

	int                  buflen;			// Length of the buffer
	char                 buf[BUFFER_SIZE];	// Buffer for recv/send/AcceptEx

	int                  operation;			// Type of operation issued
	int					 socketstate;		// 
	DWORD				 ticktime;			// time of socket operation.

	char				sz_client_ip[33];		// ���� ������
	char				sz_client_port[7];		// ���� Port
	unsigned short		us_STBLicenseCnt;		// stb�� lic-count. LIC_ARRAY_REQ ���̿��� ����.
	char				sz_DHEncKey[17];		// DH Enc key.

	DWORD				dwBeginTime;			// ���̼��� �߱� ���� �ð�.
	DWORD				dwFinishTime;			// ���̼��� �߱� ���� �ð�.
// reaload ���°� ã�� �� ����	BOOL				b_EncKeySet;			// DH

	// [2017.07.07. smpark] �����ÿ� �ܸ� Publickey ���� ����� ���� �߰�.
	char				PublicKeyInHexa[512 + 1];
} SOCKET_OBJ;