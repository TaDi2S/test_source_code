#include "CARMAgent.h"
#include "CARMAgent_define.h"
#include "CARMUtil.h"
#include "ctcom_def.h"
#include "ctglo_def.h"

#define ALLOWED_PACKAGE_NAME ""
int CTAGENT_CHECK = 1;
char CTAGENT_PACKAGE_NAME[4096] = { 0, };

#ifndef CTAGENT_NO_API_MUTEX
CTAGENT_MUTEX CTAGENT_api_mutex;
//	static int CTAGENT_api_mutex_flag = 0;
int CTAGENT_api_mutex_flag = 0;
#define CTAGENT_API_MUTEX_INITIALIZE(mutex) CTAGENT_MUTEX_INITIALIZE(mutex, CTAGENT_api_mutex_flag)
#define CTAGENT_API_MUTEX_LOCK(mutex) CTAGENT_MUTEX_LOCK(mutex)
#define CTAGENT_API_MUTEX_UNLOCK(mutex) CTAGENT_MUTEX_UNLOCK(mutex)
#else
#define CTAGENT_API_MUTEX_INITIALIZE(mutex)
#define CTAGENT_API_MUTEX_LOCK(mutex)
#define CTAGENT_API_MUTEX_UNLOCK(mutex)
#endif

CTAGENT_GLOBAL_INFO CTAGENT_GlobalInfo;
static int CTAGENT_GlobalInfoInitFlag = CTAGENT_FALSE;

void CTAGENT_initCARMAgentGlobalInfo(void)
{
	char DeviceType[33] = { 0 };
	char BuildEnvironmentId[51] = { 0 };

	if (CTAGENT_GlobalInfoInitFlag == CTAGENT_TRUE)
		return;

	CTAGENT_GlobalInfoInitFlag = CTAGENT_TRUE;

	// set DeviceType
	CTAGENT_OLD_CTGLO_HexToBin(CTCOM_AGENT_TYPE, DeviceType, strlen(CTCOM_AGENT_TYPE));
	CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(DeviceType, DeviceType, 16, WHOLE_MESSAGE_ENC_KEY);
	CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(DeviceType + 16, DeviceType + 16, 16, WHOLE_MESSAGE_ENC_KEY);
	strcpy(CTAGENT_DeviceType, DeviceType); /////////////////////// DeviceType

	// set PackagedPreviewInfo
	memset(&CTAGENT_GlobalInfo.PackagedPreviewInfo, 0, sizeof(CTAGENT_GlobalInfo.PackagedPreviewInfo));
	CTAGENT_GlobalInfo.PackagedPreviewInfo.ChannelPreviewUseFlag = CTAGENT_FALSE;
	CTAGENT_GlobalInfo.PackagedPreviewInfo.PreviewCallbackFlag = CTAGENT_FALSE;
	CTAGENT_GlobalInfo.PackagedPreviewInfo.FUNCP_PreviewCallback = NULL;
	CTAGENT_GlobalInfo.PackagedPreviewInfo.PlayerChannelPreviewTimeSec = -1;
	CTAGENT_GlobalInfo.PackagedPreviewInfo.PlayerChannelPreviewTimeMSec = -1;

	// set LicenseUseInfo
	memset(&CTAGENT_GlobalInfo.LicenseUseInfo, 0, sizeof(CTAGENT_GlobalInfo.LicenseUseInfo));
	CTAGENT_GlobalInfo.LicenseUseInfo.ChannelLicenseUseFlag = CTAGENT_CHANNEL_LICENSE_USE_FLAG_DEFAULT;
	CTAGENT_GlobalInfo.LicenseUseInfo.LicenseManagerUseFlag = CTAGENT_LICENSE_MANAGER_USE_FLAG_DEFAULT;

	// set ProtocolUseInfo
	memset(&CTAGENT_GlobalInfo.ProtocolUseInfo, 0, sizeof(CTAGENT_GlobalInfo.ProtocolUseInfo));
	CTAGENT_GlobalInfo.ProtocolUseInfo.ConfigReqUseFlag = CTAGENT_CHANNEL_PROTOCOL_CONFIG_REQ_USE_FLAG_DEFAULT;

	// set CertificateUseInfo
	memset(&CTAGENT_GlobalInfo.CertificateUseInfo, 0, sizeof(CTAGENT_GlobalInfo.CertificateUseInfo));
	CTAGENT_GlobalInfo.CertificateUseInfo.CertificateRemoveFlag = CTAGENT_CERTIFICATE_REMOVE_FLAG_DEFAULT; // CT_SetCARM시 인증서 삭제하는 옵션

	// set VodPreviewInfo
	memset(&CTAGENT_GlobalInfo.VodPreviewInfo, 0, sizeof(CTAGENT_GlobalInfo.VodPreviewInfo));
	CTAGENT_GlobalInfo.VodPreviewInfo.PreviewUseFlag = CTAGENT_TRUE;
	CTAGENT_GlobalInfo.VodPreviewInfo.FUNCP_PreviewCallback = NULL;
	CTAGENT_GlobalInfo.VodPreviewInfo.PreviewCallbackFlag = CTAGENT_FALSE;
	CTAGENT_GlobalInfo.VodPreviewInfo.PlayerPreviewTimeSec = -1;
	CTAGENT_GlobalInfo.VodPreviewInfo.PlayerPreviewTimeMSec = -1;
	CTAGENT_GlobalInfo.VodPreviewInfo.AudioPtsUseFlag = CTAGENT_FALSE;
	CTAGENT_GlobalInfo.VodPreviewInfo.AudioPtsLastCheckReturn = CTRET_SUCCESS;

	// Set TimeCompareUse
	memset(&CTAGENT_GlobalInfo.TimeCompareUseInfo, 0, sizeof(CTAGENT_GlobalInfo.TimeCompareUseInfo));
	CTAGENT_GlobalInfo.TimeCompareUseInfo.TimeCompareEnableFlag = CTAGENT_TIME_CHECK_ENABLE_FLAG_DEFAULT; // Live에서 H/E와 PC시간 체크하는 기능 사용여부 옵션
	CTAGENT_GlobalInfo.TimeCompareUseInfo.TimeRange = 0;

	// Set DeviceControlInfo
	memset(&CTAGENT_GlobalInfo.DeviceControlInfo, 0, sizeof(CTAGENT_GlobalInfo.DeviceControlInfo));
	CTAGENT_GlobalInfo.DeviceControlInfo.CheckTimeout = CTAGENT_HE_CHECK_TIMEOUT_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.EMMCacheFlag = CTAGENT_HE_MEMM_CACHE_USE_FLAG_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.OfflineOnFlag = CTAGENT_HE_OFFLINE_MODE_USE_FLAG_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.SocketRetryCount = CTAGENT_HE_SOCKET_RETRY_COUNT_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.SocketTimeoutMs = CTAGENT_HE_SOCKET_TIME_OUT_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.DecryptLogCount = CTAGENT_HE_DECRYPT_LOG_COUNT_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.VerifyAgentFlag = CTAGENT_HE_VERIFY_AGENT_FLAG_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.VerifyPlayerFlag = CTAGENT_HE_VERIFY_PLAYER_FLAG_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.ContentFormatCount = CTAGENT_HE_CONTENT_FORMAT_COUNT_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.LogDirManagingSize = CTAGENT_LOG_DIRECTORY_MANAGING_SIZE_DEFAULT;
	CTAGENT_GlobalInfo.DeviceControlInfo.LastLogDirManagingTime = 0;
	CTAGENT_GlobalInfo.DeviceControlInfo.PollTimeOut = 5000;

	// Set AuthenticationInfo
	memset(&CTAGENT_GlobalInfo.AuthenticationInfo, 0, sizeof(CTAGENT_GlobalInfo.AuthenticationInfo));

	// Set CallbackLogInfo
	memset(&CTAGENT_GlobalInfo.CallbackLogInfo, 0, sizeof(CTAGENT_GlobalInfo.CallbackLogInfo));

	// Set AgentInfo
	memset(&CTAGENT_GlobalInfo.AgentInfo, 0, sizeof(CTAGENT_GlobalInfo.AgentInfo));
	CTAGENT_OLD_CTGLO_HexToBin(CTCOM_AGENT_BUILD_ENVIRONMENT_ID, BuildEnvironmentId, strlen(CTCOM_AGENT_BUILD_ENVIRONMENT_ID));
	CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(BuildEnvironmentId, BuildEnvironmentId, 16, CT_AUTHENTICATE_KEY);
	CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(BuildEnvironmentId + 16, BuildEnvironmentId + 16, 16, CT_AUTHENTICATE_KEY);
	CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(BuildEnvironmentId + 32, BuildEnvironmentId + 32, 16, CT_AUTHENTICATE_KEY);
	BuildEnvironmentId[48] ^= (unsigned char)'C';
	BuildEnvironmentId[49] ^= (unsigned char)'T';
	strncpy(CTAGENT_GlobalInfo.AgentInfo.BuildEnvironment, BuildEnvironmentId, sizeof(CTAGENT_GlobalInfo.AgentInfo.BuildEnvironment));
}


int CT_SetCARM(
	BYTE *SrvID,                   // Service Provider 구분자
	unsigned int SrvIDSize,        // Service Provider 구분자의 크기
	BYTE *AuthInfo,                // 사용자 인증을 위한 BYTE String. Service Site에 Oriented 된 Value
	unsigned int AuthInfoSize,     // 사용자 인증정보의 크기
	char *LiUrl,                   // License Issuer URL
	char *UEMMUrl,                 // Unicast EMM URL
	char *MEMMUrl,                 // Multicast EMM URL
	char *CertPath,                // CARM Agent에서 사용하는 인증서 저장 경로
	char *LogPath,                 // CARM Agent의 파일 Log 저장 경로
	unsigned int *ErrCode)         // Error Code return 용

{

	int ReturnValue = CTRET_SUCCESS;
	int ReturnError = 0;
	int ServiceSiteIndex = -1;
	int CARMIndex = -1;
#ifdef _USE_TIME_LOG
	clock_t before_clock_t = 0;
	clock_t after_clock_t = 0;
#endif



	CTAGENT_API_MUTEX_INITIALIZE(&CTAGENT_api_mutex);

	CTAGENT_API_MUTEX_LOCK(&CTAGENT_api_mutex);

	if (CTAGENT_CHECK == 0) {
		ReturnValue = CTRET_FAIL;
		CT_setErrorValueInPointer(ErrCode, CTERR_GLO_UNKNOWN_PACKAGE_NAME);
		goto FUNCTION_END;
	}

#ifdef _USE_TIME_LOG
	before_clock_t = clock();
	CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "[TIMECHECK]\t11000001\tstart\n");
#endif

	CTAGENT_initCARMAgentGlobalInfo();

#ifdef CTAGENT_OS_WINDOWS
#ifdef LOG_WRITE_MUTEX
	InitializeCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

#ifdef CTAGENT_TEST_OFFLINE
	CARMIndex = CTAGENT_DUMMY_DESCRIPTOR;
	ServiceSiteIndex = 0;

	{
		char szLog[4096] = { 0 };

		strcpy(szLog, "CT_SetCARM");
		sprintf(szLog + strlen(szLog), "  Version=[%s]", CT_GetCARMVer());
		sprintf(szLog + strlen(szLog), "  SrvID=[%s]", SrvID != NULL ? SrvID : "NULL");
		sprintf(szLog + strlen(szLog), "  SrvIDSize=[%u]", SrvIDSize);
		sprintf(szLog + strlen(szLog), "  AuthInfo=[%s]", AuthInfo != NULL ? AuthInfo : "NULL");
		sprintf(szLog + strlen(szLog), "  AuthInfoSize=[%u]", AuthInfoSize);
		sprintf(szLog + strlen(szLog), "  LiUrl=[%s]", LiUrl != NULL ? LiUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  UEMMUrl=[%s]", UEMMUrl != NULL ? UEMMUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  MEMMUrl=[%s]", MEMMUrl != NULL ? MEMMUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  CertPath=[%s]", CertPath);
		sprintf(szLog + strlen(szLog), "  LogPath=[%s]", LogPath != NULL ? LogPath : "NULL");
		sprintf(szLog + strlen(szLog), "  ErrCode=[%08X]", ErrCode != NULL ? *ErrCode : 0xFFFFFFFF);
		sprintf(szLog + strlen(szLog), "  ReturnValue=[%d]\n", ReturnValue);

		/// \brief Callback log 사용
		/// UseFlag가 true인 경우 callback을 사용하기때문에 LogPath파라미터는 무의미하다
		/// \author kohen_i
		/// \date 2019-08-29
		if (CTAGENT_GlobalInfo.CallbackLogInfo.UseFlag != CTAGENT_TRUE)
		{
			strncpy(CTAGENT_CarmServiceSiteList[ServiceSiteIndex].STBUserInfo.szSetMessagesFilePath, LogPath, 256);
			strncpy(CTAGENT_CarmServiceSiteList[ServiceSiteIndex].STBUserInfo.szSetConfigFilePath, LogPath, 256);
		}
		else
		{
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].STBUserInfo.szSetMessagesFilePath[0] = NULL;
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].STBUserInfo.szSetConfigFilePath[0] = NULL;
		}

		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite = &CTAGENT_CarmServiceSiteList[ServiceSiteIndex];

		if (CTAGENT_isHdmiBlockDevice(ServiceSiteIndex) == CTRET_YES)
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].HdmiBlockFlag = CTAGENT_TRUE;
		else
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].HdmiBlockFlag = CTAGENT_FALSE;

		CTAGENT_OLD_CTGLO_InterfaceLog(CARMIndex, szLog);
	}

	CT_setErrorValueInPointer(ErrCode, CTERR_SUCCESS);
	CTAGENT_API_MUTEX_UNLOCK(&CTAGENT_api_mutex);
	return ReturnValue;
#endif

	//	CTAGENT_CarmInfo[CARMIndex].nBootingProcFuncStatus = CALLED;

	//	CTAGENT_CarmInfo[CARMIndex].nBootingProcFuncStatus |= PARAMETER_SET;


	ReturnValue = CTAGENT_setServiceSite_adv(SrvID, SrvIDSize,
		AuthInfo, AuthInfoSize, LiUrl, UEMMUrl, MEMMUrl, CertPath, LogPath, ErrCode, &ServiceSiteIndex);



	if (ReturnValue != CTRET_SUCCESS)
	{
		goto FUNCTION_END;
	}

	CTAGENT_CarmServiceSiteList[ServiceSiteIndex].SetCarmFunctionStatus |= USER_INFO_SET;

	CTAGENT_CarmServiceSiteList[ServiceSiteIndex].License_CRC = 0xFFFF;
	CTAGENT_CarmServiceSiteList[ServiceSiteIndex].Preview_License_CRC = 0xFFFF;

	memset(CTAGENT_CarmServiceSiteList[ServiceSiteIndex].PackageUEMM, 0, sizeof(PACKAGE_UEMM_INFO) * CTAGENT_MAX_EMM_INFO_COUNT);

	CTAGENT_checkCertificateExtension(ServiceSiteIndex,
		CTAGENT_CarmServiceSiteList[ServiceSiteIndex].AgentCertificatePath);

	if (CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LicenseUseInfo.ChannelLicenseUseFlag == CTAGENT_TRUE)
	{
		ReturnError = CTAGENT_loadLicense_v10(ServiceSiteIndex,
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LicenseFilePath,
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].STBID,
			&CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelLicenseVersion,
			(CTLIC_CHANNEL_INFO *)CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo,
			0, 0);
		if (ReturnError == CTERR_LIC_API_PREFIX_INVALID)
		{
			CTAGENT_removeFile(ServiceSiteIndex, CTAGENT_CERTIFICATE_TYPE_ALL, NULL);
			ReturnError = CTERR_SUCCESS;
		}

		if (ReturnError != CTERR_SUCCESS)
		{
			//nRet = CTGLO_GetErrMsg(CARMIndex, szErrMsg, nRet);
			CT_setErrorValueInPointer(ErrCode, ReturnError);
			ReturnValue = CTRET_FAIL;
			goto FUNCTION_END;
		}

#ifdef CTAGENT_SHOW_LICENSEINFO
		CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "================================License Information================================\n");
		// for test 2022.06.03
		for (int i = 0; i < CTGLO_MAXLIVECHANNEL; i++)
		{
			int start = CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lStartTime;
			int end = CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lEndTime;
			int CurlExpire = CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lCurKcExpire;
			int NextExpire = CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lNextKcExpire;
			int Nextreq = CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lNextReqTime;

			time_t timerStart, timerEnd, timerCurlExpire, timerNextExpire, timerNextreq;
			//struct tm *t_start, *t_end;
			char startDate[32] = { 0, };
			char endDate[32] = { 0, };
			char CurlExpireDate[32] = { 0, };
			char NextExpireDate[32] = { 0, };
			char NextreqDate[32] = { 0, };

			timerStart = start;
			SecToYearMonthDay(timerStart, startDate);

			timerEnd = end;
			SecToYearMonthDay(timerEnd, endDate);

			timerCurlExpire = CurlExpire;
			SecToYearMonthDay(timerCurlExpire, CurlExpireDate);

			timerNextExpire = NextExpire;
			SecToYearMonthDay(timerNextExpire, NextExpireDate);

			timerNextreq = Nextreq;
			SecToYearMonthDay(timerNextreq, NextreqDate);



			//sprintf(startDate, "%04d-%02d-%02d %02d:%02d:%02d", t_start->tm_year + 1900, t_start->tm_mon + 1, t_start->tm_mday, t_start->tm_hour, t_start->tm_min, t_start->tm_sec);
			//sprintf(endDate, "%04d-%02d-%02d %02d:%02d:%02d", t_end->tm_year + 1900, t_end->tm_mon + 1, t_end->tm_mday, t_end->tm_hour, t_end->tm_min, t_end->tm_sec);

			//if (CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].nFlag != 0)
			if (CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ulLicenseID != 0)
			{
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "ulLicenseID = %u\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ulLicenseID);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "ulCAPackageID = %u\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ulCAPackageID);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "lStartTime = %s \n", startDate);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "lEndTime = %s \n", endDate);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "ContentID = %u\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ContentID);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "lCurKcExpire = %s\n", CurlExpireDate);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "lNextKcExpire = %s\n", NextExpireDate);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "lNextReqTime = %s\n", NextreqDate);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "ucChannelIP = %u\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ucChannelIP);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "ulContentFormat = %u\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].ulContentFormat);

				//CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x11111111, "lStartTime = %d\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lStartTime);
				//CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x11111111, "lEndTime = %d\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LiveChannelInfo[i].lEndTime);
				CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "=============================================================================\n");
			}
		}
#endif

	}

	CTAGENT_CarmServiceSiteList[ServiceSiteIndex].Rights_User = 1;

	CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeOutCount = 0;
	while (CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeOutCount < CTAGENT_CarmServiceSiteList[ServiceSiteIndex].MaxTimeOutCount)
	{
		ReturnValue = CTAGENT_initManager_adv(ServiceSiteIndex, CARMIndex, ErrCode);
		if (ReturnValue != CTRET_SUCCESS)
		{
			if ((*ErrCode == CTERR_GLO_SOCKRECV) || (*ErrCode == CTERR_GLO_SOCKCONNECT))
			{
				CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeOutCount++;
				if (CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeOutCount < CTAGENT_CarmServiceSiteList[ServiceSiteIndex].MaxTimeOutCount)
					CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "RetryCount [%d]\n", CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeOutCount);
				continue;
			}

			goto FUNCTION_END;
		}
		startMulticastEMM(ServiceSiteIndex, ErrCode);
		break;
	}

FUNCTION_END:

#ifdef _USE_TIME_LOG
	after_clock_t = clock();
	CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, "[TIMECHECK]\t11000001\tend\t[%.3f]\n", ((double)(after_clock_t - before_clock_t) / CLOCKS_PER_SEC));
#endif

	CTAGENT_API_MUTEX_UNLOCK(&CTAGENT_api_mutex);

	{
		char szLog[4096] = { 0 };

		strcpy(szLog, "CT_SetCARM");
		sprintf(szLog + strlen(szLog), "  Version=[%s]", CT_GetCARMVer());
		sprintf(szLog + strlen(szLog), "  SrvID=[%s]", SrvID != NULL ? SrvID : "NULL");
		sprintf(szLog + strlen(szLog), "  SrvIDSize=[%u]", SrvIDSize);
		sprintf(szLog + strlen(szLog), "  AuthInfo=[%s]", AuthInfo != NULL ? AuthInfo : "NULL");
		sprintf(szLog + strlen(szLog), "  AuthInfoSize=[%u]", AuthInfoSize);
		sprintf(szLog + strlen(szLog), "  LiUrl=[%s]", LiUrl != NULL ? LiUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  UEMMUrl=[%s]", UEMMUrl != NULL ? UEMMUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  MEMMUrl=[%s]", MEMMUrl != NULL ? MEMMUrl : "NULL");
		sprintf(szLog + strlen(szLog), "  CertPath=[%s]", CertPath);
		sprintf(szLog + strlen(szLog), "  LogPath=[%s]", LogPath != NULL ? LogPath : "NULL");
		sprintf(szLog + strlen(szLog), "  ErrCode=[%08X]", ErrCode != NULL ? *ErrCode : 0xFFFFFFFF);
		sprintf(szLog + strlen(szLog), "  ReturnValue=[%d]\n", ReturnValue);

		CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, szLog);
		CTAGENT_OLD_CTGLO_HistoryLog_s(ServiceSiteIndex, szLog);

		sprintf(szLog, "ChannelLicenseUseFlag=[%d] ",
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LicenseUseInfo.ChannelLicenseUseFlag);

		sprintf(szLog + strlen(szLog), "LicenseManagerUseFlag=[%d] ",
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].LicenseUseInfo.LicenseManagerUseFlag);

		sprintf(szLog + strlen(szLog), "ConfigUseFlag=[%d] ",
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].ProtocolUseInfo.ConfigReqUseFlag);

		sprintf(szLog + strlen(szLog), "TimeCompareUseFlag=[%d]\n",
			CTAGENT_CarmServiceSiteList[ServiceSiteIndex].TimeCompareUseInfo);

		CTAGENT_OLD_CTGLO_DebugLog_s(ServiceSiteIndex, 0x00000011, szLog);
	}

	return ReturnValue;
}