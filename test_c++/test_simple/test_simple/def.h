#pragma once

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
	unsigned char	*sz_SplunkUrl;			// Splunk 연동 URL 값
	unsigned char	*sz_Reserved;			// reserved
} PACKET_INFO_RES_TEST2;

typedef struct _PACKET_HEADER					// header는 전송시 암호화 하지 않음.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID 를 명시  0x1234 : 고정값.
	unsigned short	sh_ProtocolVer;				// 프로토콜 버젼  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 일 경우 등록되지 않은 초기상태)
	unsigned short	us_ProtocolCommand;			// 명령어 및 리턴코드.
	unsigned short	us_ProtocolDataSize;		// 전송 데이터 크기.
} PACKET_HEADER;
