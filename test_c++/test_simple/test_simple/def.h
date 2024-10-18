#pragma once

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
	unsigned char	*sz_SplunkUrl;			// Splunk ���� URL ��
	unsigned char	*sz_Reserved;			// reserved
} PACKET_INFO_RES_TEST2;

typedef struct _PACKET_HEADER					// header�� ���۽� ��ȣȭ ���� ����.
{
	short			sh_ProtocolID;				// CoreTrust Protocol ID �� ���  0x1234 : ������.
	unsigned short	sh_ProtocolVer;				// �������� ����  - PROTOCOL_VERSION (3)
	unsigned long	ul_AgentID;					// cspterminal.cspterminal ID (0 �� ��� ��ϵ��� ���� �ʱ����)
	unsigned short	us_ProtocolCommand;			// ��ɾ� �� �����ڵ�.
	unsigned short	us_ProtocolDataSize;		// ���� ������ ũ��.
} PACKET_HEADER;
