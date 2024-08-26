#pragma once
#ifndef _CT_CARM_AGENT_ERROR_H_
#define _CT_CARM_AGENT_ERROR_H_

#define CTERR_SUCCESS                                           0
#define CTERR_SUCCESS_PREVIEW                                   1
#define CTERR_GLO_SOCKINIT                                      0x01000001
#define CTERR_GLO_SOCKCREATE                                    0x01000002
#define CTERR_GLO_SOCKCONNECT                                   0x01000003
#define CTERR_GLO_SOCKSEND                                      0x01000004
#define CTERR_GLO_SOCKRECV                                      0x01000005
#define CTERR_GLO_SOCKSETOPT                                    0x01000006
#define CTERR_GLO_SOCKIPV6                                      0x01000007
#define CTERR_GLO_FUNCTION_TYPE                                 0x02000001
#define CTERR_GLO_NO_MSG                                        0x02000002
#define CTERR_GLO_DELETE_FILE                                   0x02000003
#define CTERR_GLO_AGENT_EXPIRED                                 0x02000004
#define CTERR_GLO_UNKNOWN_PACKAGE_NAME                          0x02000005
#define CTERR_GLO_PROTOCOL_UNKNOWN                              0x03000001
#define CTERR_GLO_PROTOCOL_INCORRECT                            0x03000002
#define CTERR_GLO_PROTOCOL_DATA_INVALID                         0x03000003
#define CTERR_GLO_PROTOCOL_CRC_INVALID                          0x03000004
#define CTERR_LIC_API_FILE_OPEN                                 0x04000001
#define CTERR_LIC_API_INVALID_ID                                0x04000002
#define CTERR_LIC_API_INVALID_FUNCTYPE                          0x04000003
#define CTERR_LIC_API_INVALID_VER                               0x04000004
#define CTERR_LIC_API_INVALID_SEARCH                            0x04000005
#define CTERR_LIC_API_INVALID_INDEX                             0x04000006
#define CTERR_LIC_API_INVALID_COUNT                             0x04000007
#define CTERR_LIC_API_INVALID_LIC_TYPE                          0x04000008
#define CTERR_LIC_API_INVALID_NULL                              0x04000009
#define CTERR_LIC_API_INVALID_STRUCT                            0x0400000A
#define CTERR_LIC_API_INVALID_CRC                               0x0400000B
#define CTERR_LIC_API_VOD_INVALID_ID                            0x0400000C
#define CTERR_LIC_API_VOD_INVALID_CRC                           0x0400000D
#define CTERR_LIC_API_VOD_INVALID_CONTENT_ID                    0x0400000E
#define CTERR_LIC_API_VOD_NOT_ENOUGH_BUFF                       0x04000010
#define CTERR_LIC_API_VOD_LICENSE_NOT_STARTED                   0x04000011
#define CTERR_LIC_API_VOD_LICENSE_EXPIRED                       0x04000012
#define CTERR_LIC_API_FILE_CHMOD								0x04000013	// 권한 설정 실패
#define CTERR_LIC_API_VOD_LICENSE_INVALID_ID					0x04000014
#define CTERR_LIC_API_VOD_LICENSE_OPEN_FAIL						0x04000015
#define CTERR_LIC_API_VOD_LICENSE_READ_FAIL						0x04000016
#define CTERR_LIC_API_VOD_LICENSE_WRITE_FAIL					0x04000017
#define CTERR_LIC_API_VOD_LICENSE_INVALID_PREFIX				0x04000018
#define CTERR_LIC_API_VOD_LICENSE_INVALID_TIME					0x04000019
#define CTERR_LIC_API_VOD_NOT_FOUND_LICENSE						0x0400001A
#define CTERR_LIC_API_PREFIX_INVALID                            0x04000021
#define CTERR_LIC_API_LICENSE_CANNOT_SUPPORT_VERSION            0x04000022
#define	CTERR_EMM_API_FILE_OPEN                                 0x04001001
#define	CTERR_EMM_API_FILE_PRIFIX                               0x04001002
#define	CTERR_EMM_API_FILE_VERSION                              0x04001003
#define	CTERR_EMM_API_FILE_CHECKSUM                             0x04001004
#define	CTERR_EMM_API_FILE_SIZE                                 0x04001005
#define	CTERR_EMM_API_FILE_CHMOD                                0x04001006
#define	CTERR_EMM_API_FILE_READ                                 0x04001007
#define	CTERR_EMM_API_TIME_EXPIRED                              0x04001008
#define	CTERR_EMM_API_TIME_NOT_STARTED                          0x04001009
#define	CTERR_EMM_API_ALLOC_FAIL                                0x0400100A
#define CTERR_CERTIFICATE_INVALID                               0x05000001
#define CTERR_ESIGNATURE_INVALID                                0x05000002
#define CTERR_ESIGNATURE_CREATE_FAIL                            0x05000003
#define CTERR_CERTIFICATE_EXTENSION_INVALID                     0x05000004
#define CTERR_PK_KEY_INVALID                                    0x05000005
#define CTERR_SECRET_KEY_INVALID                                0x05000006
#define CTERR_CERTIFICATE_OPEN                                  0x05000007
#define CTERR_AGENT_CERTIFICATE_OPEN                            0x05000008
#define CTERR_CERTIFICATE_INVALD_TODO_UPDATE                    0x05000009
#define CTERR_PK_CREATE_FAIL                                    0x0500000A
#define CTERR_CERTIFICATE_FILE_CHMOD	            			0x0500000B	// 권한 설정 실패
#define CTERR_STB_SECRET_KEY_LOAD_FAIL                          0x06000001
#define CTERR_STB_SECRET_KEY_UPDATE                             0x06000002
#define CTERR_STB_AGENT_UPDATE                                  0x06000003
#define CTERR_FILE_OPEN                                         0x06000004
#define CTERR_UNKNOWN_FORMAT                                    0x06000005
#define CTERR_CONTENT_NOT_EXIST                                 0x06000006 // remove
#define CTERR_LICENSE_NOT_EXIST                                 0x06000007
#define CTERR_NO_DRM_CONTENT_FILE                               0x06000008
#define CTERR_ERRMSGBUFF_INVALID                                0x06000009
#define CTERR_STB_INVALID_PARAMETER                             0x06000A00
#define CTERR_STB_NOT_ENOUGH_BUFF                               0x0600000B
#define CTERR_STB_INVALID_FUNCTION_CALL                         0x0600000C
#define CTERR_DEVICE_ID_DISABLED                                0x0600000D
#define CTERR_INVALID_CONTENT_FORMAT                            0x0600000E
#define CTERR_CIF_INCORRECT_TS_PACKET_SIZE                      0x07000001
#define CTERR_CIF_INCORRECT_TS_PACKET                           0x07000002
#define CTERR_CIF_NO_PAYLOAD                                    0x07000003
#define CTERR_CIF_NON_TS_PACKET                                 0x07000004
#define CTERR_CIF_INVALID_CC                                    0x07000005
#define CTERR_INVALID_CW                                        0x07000006
#define CTERR_INVALID_SK                                        0x07000007
#define CTERR_GLO_INCORRECT_ECM_SIZE                            0x07000008
#define CTERR_GLO_INCORRECT_ECM_CHECK_SUM                       0x07000009
#define CTERR_GLO_INCORRECT_ECM_SIGN                            0x0700000A
#define CTERR_LICENSE_NOT_STARTED                               0x0700000B
#define CTERR_GLO_INVALID_LOCATION                              0x08000001
#define CTERR_GLO_DEVICE_ID_NOT_EXIST                           0x08000002
#define CTERR_SCRAMBLED_TS_IN_BUFF                              0x08000003
#define CTERR_INVALID_PACKAGE_ON_CHANNEL                        0x08000004
#define CTERR_CIF_TS_PACKET_SIZE_ZERO                           0x08000005
#define CTERR_LICENSE_EXPIRED                                   0x08000006
#define CTERR_PREVIEW_FILE_OPEN                                 0x08000007
#define CTERR_CAM_INCORRECT_EMM_VERSION                         0x09000001
#define CTERR_CAM_INCORRECT_COMMNAD_EMM_SIZE                    0x09000002
#define CTERR_CAM_INCORRECT_PACKAGE_EMM_SIZE                    0x09000003
#define CTERR_CAM_INVALID_COMMAND_EMM_FLAG                      0x09000004
#define CTERR_CAM_INVALID_COMMAND_EMM_COMMAND                   0x09000005
#define CTERR_CAM_INVALID_COMMAND_EMM_COMMAND_FULL              0x09000006
#define CTERR_CAM_INCORRECT_EMM_CHECK_SUM                       0x09000007
#define CTERR_CAM_CAMESSAGE_PROCESSOR                           0x09000008
#define CTERR_CAM_CAMESSAGE_NOT_CALL_XSEC                       0x09000009
#define CTERR_H264_ES_ENC_CODE_ADDED                            0x09000A00
#define CTERR_PVR_INVALID_CHANNEL_ID                            0x0A000001
#define CTERR_PVR_NO_RIGHT                                      0x0A000002
#define CTERR_PVR_IMPOSSIBLE_CHANNEL                            0x0A000003
#define CTERR_PVR_MAX_CHANNEL                                   0x0A000004
#define CTERR_PVR_INSERT_ECM                                    0x0A000005
#define CTERR_CARM_SERVICE_IS_FULL                              0x0B000001
#define CTERR_CARM_DESCRIPTOR_IS_FULL                           0x0B000002
#define CTERR_NOT_ENOUGH_LIC_BUFFSIZE                           0x0B000003
#define CTERR_INVALID_DESCRIPTOR                                0x0B000004
#define CTERR_SERVICE_SITE_NOT_EXIST                            0x0B000005
#define CTERR_CONTENT_ID_NOT_EXIST                              0x0B000006
#define CTERR_WORKER_SERVICE_NOT_EXIST                          0x0B000007
#define CTERR_SYSTEM_MALLOC_FAIL                                0x0B000008
#define CTERR_CREATE_EMM_THREAD_FAIL                            0x0B000009
#define CTERR_INVALID_URL                                       0x0B00000A
#define CTERR_CANNOT_FOUND_CONTENT_INFO                         0x0B00000B
#define CTERR_ALREADY_EXIST_SERVICE_SITE                        0x0B00000C // remove
#define CTERR_INVALID_USE_FLAG                                  0x0B00000D
#define CTERR_SERVICE_SITE_IS_BUSY                              0x0B00000E
#define CTERR_UNKNOWN_COMMAND                                   0x0B00000F
#define CTERR_DETECTED_OUTPUT_DEVICE                            0x0B000010
// CARMAgentLoader Generated Error
#define CTERR_CAL_INVALID_SIGNING								0x0B000011	// 파일 서명 인증 실패
#define CTERR_CAL_INVALID_PROCESS								0x0B000012	// Process 인증 실패
#define CTERR_CAL_PAIRING_FAILED								0x0B000013	// Pairing 실패
#define CTERR_CAL_INVALID_OS									0x0B000014	// OS 버전 불일치
#define CTERR_CAL_INVALID_ENVIRONMENT							0x0B000015	// 가상머신 등의 허용되지 않는 환경에서 실행
#define CTERR_CAL_CAL_NOT_INITIALIZED							0x0B000016	// CARMAgentLoader가 초기화 되지 않음
#define CTERR_CAL_INVALID_DLL_PATH								0x0B000017	// CARMAgent 파일 경로 오류
#define CTERR_CAL_AGENT_FILE_READ_FAILED						0x0B000018	// CARMAgent 파일 Read 오류
#define CTERR_CAL_AGENT_DEC_FAILED								0x0B000019	// CARMAgent 복호화 오류
#define CTERR_CAL_AGENT_MEM_LOAD_FAILED							0x0B00001A	// CARMAgent Memory Load 오류
#define CTERR_CAL_PAIRING_DATA_WRITE_FAILED						0x0B00001B	// Pairing Data 기록 실패
#define CTERR_CAL_THREAD_START_FAILED							0x0B00001C	// Thread Start 실패
#define CTERR_CAL_TIME_COMP_USE_FAILED							0x0B00001D	// H/E & PC Time check 기능 실행 실패
#define CTERR_CAL_TIME_COMPARE_FAILED							0x0B00001E	// H/E & PC Time 불일치
#define CTERR_CAL_UNKNOWN_ERR									0x0B00001F	// Unknown 오류
#define CTERR_CAL_AUTHENTICATE_INFO_NOT_FOUND					0x0B000020	// 인증 정보 없음
#define CTERR_CAL_INVALID_AUTHENTICATE_TOKEN					0x0B000021	// 잘못된 인증정보
#define CTERR_CAL_INVALID_AUTHENTICATE_TOKEN_EXPIRED			0x0B000022	// 인증정보 만료
#define CTERR_CAL_PLAYER_OPEN_FAIL								0x0B000023	// PLAYER open fail
#define CTERR_CAL_PLAYER_READ_FAIL								0x0B000024	// PLAYER read fail
#define CTERR_CONFIG_FILE_SAVE_FAIL								0x0B000025	// config file save fail
//
#define CTERR_HEVC_PARSE_FAIL                                   0x0C000001
#define CTERR_HEVC_PARSE_MALLOC_FAIL                            0x0C000002
#define CTERR_HEVC_PARSE_NO_SPS                                 0x0C000003
#define CTERR_HEVC_PARSE_NO_PPS                                 0x0C000004
#define CTERR_HEVC_PARSE_BUFFER_SIZE_OVER                       0x0C000005
#define CTERR_H264_ES_NO_ENCRYPT_INFO                           0x0D000001
#define CTERR_IN_THREAD                                         0x80000000 // inner error
#define CTERR_DEC_WARNING                                       0x90000000
#define ERROR_LICENSEISSUER                                     0x20000000
#define ERROR_DATABASE_EXCEPTION                                0x20000002
#define ERROR_CSPMEMBER_STATE_DISABLE                           0x20000004
#define ERROR_CSPTERMINAL_STATE_DISABLE                         0x20000005
#define ERROR_CSPCONTENT_NOT_EXIST                              0x20000006 // remove
#define ERROR_CONTENT_STATE_DISABLE                             0x20000007
#define ERROR_CSPCONTENTLIST_STATE_DISABLE                      0x20000008
#define ERROR_CSPINFO_NOT_EXIST                                 0x2000000C
#define ERROR_CSPINFO_STATE_DISABLE                             0x2000000D
#define ERROR_CSPTERMINAL_NOT_EXIST                             0x2000000F
#define ERROR_CSPMEMBER_NOT_EXIST                               0x20000010
#define ERROR_DRMSERVER_ID_NOT_EXIST                            0x20000018
#define ERROR_DRMSERVER_LICENSE                                 0x20000023
#define ERROR_DIGITAL_SIGN_FAIL                                 0x20000026
#define ERROR_PROTOCOL_VERSION                                  0x20000027
#define ERROR_PROTOCOL_ID                                       0x20000028
#define ERROR_PROTOCOL_COMMAND                                  0x20000029
#define ERROR_PROTOCOL_DATA_SIZE                                0x20000030
#define ERROR_PROTOCOL_DECRYPTION                               0x20000031
#define ERROR_PROTOCOL_FLOW                                     0x20000032
#define ERROR_CSPTERMINAL_ID_ALREADY_EXIST                      0x20000033
#define ERROR_CSPTERMINAL_AUTHKEY_EXPIRED                       0x20000034
#define ERROR_MIDDLEWARE_LINK                                   0x20000035
#define ERROR_CONTENT_NOT_EXIST                                 0x20000036
#define ERROR_LICENSE_NOT_EXIST                                 0x20000037
#define ERROR_LICENSE_STATE_DISABLE                             0x20000038
#define ERROR_LICENSE_NOT_ACTIVATED                             0x20000039
#define ERROR_LICENSE_EXPIRED                                   0x20000040
#define ERROR_LICENSE_INFO_NOT_EXIST                            0x20000041
#define LICENSEISSUER_ENABLE                                    0x10000000
#define LICENSEISSUER_DISABLED                                  0x20000000
#define LICENSEISSUER_DISABLE_TERM                              0x20010000
#define LICENSEISSUER_DISABLE_TERMINAL_CNT                      0x20020000
#define LICENSEISSUER_DISABLE_TIMESYNC                          0x20040000
#define LICENSEISSUER_DISABLE_FILEERROR                         0x20080000
#define ERROR_INVALID_SERVICE_ID                                0x30000001
#define ERROR_INVALID_AUTH_INFO                                 0x30000002
#define ERROR_INVALID_CONTENT_INFO                              0x30000003


#endif

