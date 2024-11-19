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
#define		CMD_INFO_REQ						0x4301		// [2024.10.16. dykim] Splunk 정보 요청 프로토콜 추가
#define		CMD_INFO_RES						0x4302		// [2024.10.16. dykim] Splunk 정보 응답 프로토콜 추가
#define		PROTOCOL_ID							0x1234

#define		SUCCESS_LICENSEISSUER							0x10000000	// 성공 
#define		ERROR_DRMSERVER_LICENSE						0x20000023
#define		ERROR_CSPTERMINAL_NOT_EXIST					0x2000000F
#define		ERROR_DRMSERVER_ID_NOT_EXIST				0x20000018

#define		CMD_IMG_UPDATE_RES					0x4003
#define		CMD_CERT_UPDATE_RES					0x4004
#define		CMD_SECRET_KEY_UPDATE_RES		0x4005
#define		CMD_DEVICE_ID_DISABLE_RES			0x4006

#define		ERROR_DEVICE_STATE_DISABLED				0x2000000E		// STB_type 에 대하여 license_device 테이블에 데이터 없거나, status disable 상태
#define		ERROR_DEVICE_ID_DISABLED					0x20000022		// STB_type 에 대하여 license_device 테이블에 데이터 없거나, status disable 상태
#define		PROTOCOL_VERSION					4
// 2007-10-05 : add 
#define		PROTOCOL_VERSION_5					5

#define BOOL	int					//	1-bit data type
#define BYTE	unsigned char		//	unsigned 1-byte data type
#define WORD	unsigned short int	//	unsigned 2-bytes data type

// db 값이 null인 경우 공백문자열로 return.
#define NULL_2_EMPTY(xx) ( xx.vt == VT_NULL ? "" : _bstr_t(xx) )
// db 값이 null인 경우 0 으로 return.
#define NULL_2_ZERO(xx)  ( xx.vt == VT_NULL ? 0 : xx.intVal )
typedef unsigned long       DWORD;

#define		ERROR_DATABASE_EXCEPTION 						0x20000002
#define		ERROR_CSPMEMBER_NOT_EXIST						0x20000010		// INIT_REQ 에 대한 mem_id 가 존재하지 않음.
#define		ERROR_CSPTERMINAL_ID_ALREADY_EXIST			0x20000033
#define		ERROR_CSPTERMINAL_AUTHKEY_EXPIRED			0x20000034


typedef struct _PACKET_INFO_RES
{
	unsigned int	ul_SplunkFlag;			//Splunk 연동 Flag (1: 사용, 0:미사용)
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl의 길이
	unsigned int	ul_ReservedLen;		// sz_Reserved의 길이
	unsigned int	ul_BaseGenreLen;		// sz_BaseGenre의 길이
	unsigned int	ul_EMMCacheFlag;		//EMM cache 사용 flag (1:사용, 0: 미사용)
	unsigned int	ul_OperationMode;	// 운영모드: ( 1: 상용, 0: 개발)
	char				*sz_BaseGenre;			// 필수시청채널 장르 코드 배열
	char				*sz_SplunkUrl;			// Splunk 연동 URL 값
	char				*sz_Reserved;			// reserved -> 확장성을 위해 임시로 잡아둔 것
} PACKET_INFO_RES;
// [2024.10.16 dykim] Splunk 정보 전송을 위한 프로토콜용 구조체 추가
// 테스트용
typedef struct _PACKET_INFO_RES_TEST1
{
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl의 길이
	unsigned char	*sz_SplunkUrl;			// Splunk 연동 URL 값
	unsigned int	ul_BaseGenre;			// 기본 채널 장르 Code 값
	unsigned int	ul_ReservedLen;			// sz_Reserved의 길이
	unsigned char	*sz_Reserved;			// reserved
} PACKET_INFO_RES_TEST1;

// 포인터 뒤로 미루기
typedef struct _PACKET_INFO_RES_TEST2
{
	unsigned int 	ul_SplunkUrlLen;		// sz_SplunkUrl의 길이	
	unsigned int	ul_BaseGenre;			// 기본 채널 장르 Code 값
	unsigned int	ul_ReservedLen;			// sz_Reserved의 길이
	unsigned char* 	sz_SplunkUrl;			// Splunk 연동 URL 값
} PACKET_INFO_RES_TEST2;

typedef struct _PACKET_HEADER					// header는 전송시 암호화 하지 않음.
{
	short				sh_ProtocolID;				// CoreTrust Protocol ID 를 명시  0x1234 : 고정값.
	unsigned short	sh_ProtocolVer;				// 프로토콜 버젼  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 일 경우 등록되지 않은 초기상태)
	unsigned short	us_ProtocolCommand;			// 명령어 및 리턴코드.
	unsigned short	us_ProtocolDataSize;		// 전송 데이터 크기.
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

	char				sz_client_ip[33];		// 접속 아이피
	char				sz_client_port[7];		// 접속 Port
	unsigned short		us_STBLicenseCnt;		// stb의 lic-count. LIC_ARRAY_REQ 사이에서 사용됨.
	char				sz_DHEncKey[17];		// DH Enc key.

	DWORD				dwBeginTime;			// 라이센스 발급 시작 시간.
	DWORD				dwFinishTime;			// 라이센스 발급 종료 시간.
// reaload 쓰는곳 찾을 수 없음	BOOL				b_EncKeySet;			// DH

	// [2017.07.07. smpark] 비상모드시에 단말 Publickey 정보 사용을 위해 추가.
	char				PublicKeyInHexa[512 + 1];
} SOCKET_OBJ;