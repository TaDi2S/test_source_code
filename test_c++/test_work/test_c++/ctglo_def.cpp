#include <stdarg.h>
#include "ctglo_def.h"
#include "ctprotocol.h"
#include "CARMUtil.h"
#include "ctdec_aes.h"
#include "ctcom_def.h"
#include "CARMManagerConstant.h"

#ifndef CTCOM_OS_WINCE
#include <time.h>
#else
#include <windows.h>
#endif

#ifdef CTCOM_OS_WIN32
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#endif

#ifdef IOS_DEVICE
void CT_AppleLog(char *LogString);
#endif

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
#include <pthread.h>
pthread_mutex_t CTAGENT_OLD_logwrite_mutex = PTHREAD_MUTEX_INITIALIZER;
#else
CRITICAL_SECTION CTAGENT_OLD_logwrite_critical_section;
#endif
#endif



static int	CTAGENT_OLD_masks[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };	/* bit Mask  */

/****************************************************************
	Desc
		대소문자에 구별없이 문자열 비교
	Parameters
		a : 비교할 문자열
		b : 비교할 문자열
		n : 비교할 사이즈
	Return
		성공시 0, 실패 1
*****************************************************************/
int CTAGENT_OLD_CTGLO_strncmp_nc(char *a, char *b, int n)
{
	while (*a && *b)
	{
		if (*a != *b)
		{
			if (*a >= 'a' && *a <= 'z')
			{
				if (*a != (*b + ('a' - 'A')))
				{
					return 1;
				}
			}
			else if (*a <= 'Z' && *a >= 'A')
			{
				if ((*a + ('a' - 'A')) != *b)
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		a++; b++;

		if (--n <= 0)
		{
			return 0;
		}
	}

	if (!*a && !*b)
	{
		return 0;
	}

	return 1;
}

/****************************************************************
	Desc
		대소문자에 구별없이 문자열 비교
	Parameters
		a : 비교할 문자열
		b : 비교할 문자열
	Return
		성공시 0, 실패 1
*****************************************************************/
int CTAGENT_OLD_CTGLO_strcmp_nc(char *a, char *b)
{
	while (*a && *b)
	{
		if (*a != *b)
		{
			if (*a >= 'a' && *a <= 'z')
			{
				if (*a != (*b + ('a' - 'A')))
				{
					return 1;
				}
			}
			else if (*a <= 'Z' && *a >= 'A')
			{
				if ((*a + ('a' - 'A')) != *b)
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		a++; b++;
	}

	if (!*a && !*b)
	{
		return 0;
	}

	return 1;
}

/****************************************************************
	Desc
		Hex문자열을 Binary데이타으로 변환
	Parameters
		strHex : 변환할 Hex 문자열
		strBin : Binary데이타가 저장될 버퍼
		size : 변환할 Hex문자열의 크기
	Return
		성공시 0, 실패 1
*****************************************************************/
int CTAGENT_OLD_CTGLO_HexToBin(char *strHex, char *strBin, int size)
{
	while (*strHex && size--)
	{
		if (*strHex >= '0' && *strHex <= '9')	*strBin = *strHex - '0';
		else if (*strHex >= 'a' && *strHex <= 'f')  	*strBin = *strHex - 'a' + 10;
		else if (*strHex >= 'A' && *strHex <= 'F')  	*strBin = *strHex - 'A' + 10;
		else	return 1;
		*strBin <<= 4;

		strHex++;
		if (!*strHex) return 1;

		if (*strHex >= '0' && *strHex <= '9')	*strBin += *strHex - '0';
		else if (*strHex >= 'a' && *strHex <= 'f')  	*strBin += *strHex - 'a' + 10;
		else if (*strHex >= 'A' && *strHex <= 'F')  	*strBin += *strHex - 'A' + 10;
		else	return 1;

		strHex++;
		strBin++;
	}
	return 0;
}

/****************************************************************
	Desc
		Binary데이타을 Hex문자열로 변환
	Parameters
		strBin : 변환할 BinaryData
		strHex : Hex문자열이 저장될 버퍼
		size : 변환할 BinaryData의 크기
	Return
		성공시 0, 실패 1
*****************************************************************/
int CTAGENT_OLD_CTGLO_BinToHex(char *strBin, char *strHex, int size)
{
	char a;
	while (size--)
	{
		a = *strBin >> 4;
		a &= 0x0F;
		if (a <= 9)	*strHex = a + '0';
		else  *strHex = a - 10 + 'A';
		strHex++;

		a = *strBin & 0x0F;
		if (a <= 9)	*strHex = a + '0';
		else  *strHex = a - 10 + 'A';
		strHex++;

		strBin++;
	}
	*strHex = 0;
	return 0;
}

/****************************************************************
	Desc
		버퍼에서 일정 위치의 한 byte를 찾아 일정 위치에 한 bit를 반환하는 함수
	Parameters
		*chBuffer : char 버퍼
		lBufPos : 버퍼에서 찾을 한 byte의 위치
		Pos : 한 byte에서 찾을 한 bit의 위치
	Return
		한 bit의 값
*****************************************************************/
DWORD CTAGENT_OLD_CTGLO_Get1Bit(unsigned char * chBuffer, DWORD lBufPos, int Pos)
{
	//unsigned char buf;
	//buf = chBuffer[lBufPos];

	//return (DWORD) (buf & masks[8 - Pos]) >> (8 - Pos); 
	return (DWORD)(chBuffer[lBufPos] & CTAGENT_OLD_masks[8 - Pos]) >> (8 - Pos);
}

/****************************************************************
	Desc
		버퍼에서 일정 위치의 한 byte를 찾아 반환하는 함수
	Parameters
		*chBuffer : char 버퍼
		lBufPos : 버퍼에서 찾을 한 byte의 위치
	Return
		한 byte의 값
*****************************************************************/
DWORD CTAGENT_OLD_CTGLO_Get1Byte(unsigned char * chBuffer, DWORD lBufPos)
{
	return (DWORD)chBuffer[lBufPos];
}

/****************************************************************
	Desc
		버퍼에서 일정 위치의 3 byte를 찾아 반환하는 함수
	Parameters
		*chBuffer : char 버퍼
		lBufPos : 버퍼에서 찾을 3 byte의 위치
	Return
		3 byte의 값
*****************************************************************/
DWORD CTAGENT_OLD_CTGLO_Get3Bytes(unsigned char * chBuffer, DWORD lBufPos)
{
	unsigned char	szGetBuf[4];
	DWORD	*pLGetBuf;
	memcpy(szGetBuf, &chBuffer[lBufPos], 3);
	szGetBuf[3] = '\0';
	pLGetBuf = (DWORD *)szGetBuf;

	return (*pLGetBuf);
}

/****************************************************************
	Desc
		버퍼에서 일정 위치의 n개의 byte를 찾아 반환하는 함수 (n은 4byte까지만 가능)
	Parameters
		*chBuffer : char 버퍼
		lBufPos : 버퍼에서 찾을  n개의 byte 위치
	Return
		 n개의 byte의 값
*****************************************************************/
DWORD CTAGENT_OLD_CTGLO_GetBytes(unsigned char * chBuffer, DWORD lBufPos, unsigned char * szGetBuf2, int n)
{
	/* 4byte 이상 넘어 가면 안됨 */
	DWORD	*pLGetBuf;
	int i;
	for (i = 0; i < n; i++)
	{
		memcpy(&szGetBuf2[(n - 1) - i], &chBuffer[lBufPos + i], 1);
	}
	szGetBuf2[n] = '\0';
	pLGetBuf = (DWORD *)szGetBuf2;
	return (*pLGetBuf);
}

#ifdef _USE_LOG_ENCRYPT
#define LOG_KEY_SEED		"25632DCA6E83C05E4451B8EB8C04C77B"
void CTAGENT_Encrypt_Log(char *plainLog, char **encLog)
{
	char			*tmpLog;
	int				plainLength = 0;
	int				paddingLength = 0;
	int				logLength = 0;

	plainLength = strlen(plainLog);
	paddingLength = (16 - (plainLength % 16));
	logLength = (plainLength + paddingLength);

	tmpLog = (char *)malloc(logLength + 1);
	memset(tmpLog, 0, logLength + 1);
	CTAGENT_OLD_CTAES_Encrypt_Onetime(plainLog, tmpLog, logLength, LOG_KEY_SEED);

	(*encLog) = (char *)malloc(logLength * 2);
	memset((*encLog), 0, logLength * 2);
	CTAGENT_encode_base64((*encLog), logLength * 2, tmpLog, logLength);
}
#endif

#ifdef CTCOM_OS_WIN32
extern void CTAGENT_OLD_TRACE(char * pszFmt, ...);
#endif

#ifdef DEBUG_LOG
void CTAGENT_OLD_CTGLO_DebugLog(int CARMIndex, unsigned int level, const char *pLogString, ...)
{
#ifdef DEBUG_FILE_LOG
	FILE			*pfp;
	char			szDebugLogFilePath[256] = { 0 };
	char			szTempLog[256] = { 0 };
	char			szDebugLog[4096] = { 0 };
#ifndef CTCOM_OS_WINCE
	struct	tm		tCurrentTime = { 0 };
	time_t			tCurTime = { 0 };
#endif
#endif
#ifdef LOG_MANAGEMENT_SIZE
	int			lLogSize = 0;
#endif
	va_list			strVL;					// 가변 인수 List 객체 선언
	char			szbuf[4096] = { 0 };
#ifdef _USE_LOG_ENCRYPT
	char			plainLog[4096] = { 0 };
	char			*encLog = NULL;
#endif

#ifdef LOG_OFF
	return;
#endif

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite == NULL)
		return;

	// Callback Log 
	/// \brief Callback log 사용 
	/// \author kohen_i 
	/// \date 2019-08-29 
	if (CTAGENT_GlobalInfo.CallbackLogInfo.UseFlag == CTAGENT_TRUE)
	{
		if (level > LOG_LEVEL)
			return;

		tCurTime = time(&tCurTime);
		tCurrentTime = *(localtime(&tCurTime));
		if (tCurrentTime.tm_year < 1900)
		{
			tCurrentTime.tm_year += 1900;
			tCurrentTime.tm_mon += 1;
		}
		sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
			tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);

		va_start(strVL, pLogString);

		vsprintf(szbuf, pLogString, strVL);

		/// \brief log level은 info 고정하여 사용. log level을 나눠서 모든 로그를 전달할시 키정보도 로그에 남을수 있음
		/// \author kohen_i
		/// \date 2019-08-29
		CTAGENT_GlobalInfo.CallbackLogInfo.LogCallBackParam.LogCall(CTAGENT_GlobalInfo.CallbackLogInfo.LogCallBackParam.LogContext, CT_KTCONNECTEDCAR_LOGLEVEL_INFO, szbuf);

		va_end(strVL);
		return;
	}

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath == NULL || !strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	if (level > LOG_LEVEL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

#ifdef DEBUG_CONSOLE_LOG
	va_start(strVL, pLogString);

	vsprintf(szbuf, pLogString, strVL);
	//vfprintf(szbuf, pLogString, strVL);

#ifdef IOS_DEVICE
	CT_AppleLog(szbuf);
#elif defined CTCOM_OS_WIN32
	CTAGENT_OLD_TRACE("%s", szbuf);
#elif defined CTAGENT_ANDROID_LOGCAT
	LOG_V("%s", szbuf);
#else
	fprintf(stderr, "%s", szbuf);
#endif

	va_end(strVL);
#endif

#ifdef DEBUG_FILE_LOG
#ifndef CTCOM_OS_WINCE
	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}
#endif //CTCOM_OS_WINCE

#ifdef CTCOM_OS_WIN32
#ifndef CTCOM_OS_WINCE
	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath[strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath) - 1] == 'ww')
		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath[strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath) - 1] = '\0';

	sprintf(szDebugLogFilePath, "%s\\messages_%04d_%02d_%02d.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#endif //CTCOM_OS_WINCE
#endif //CTCOM_OS_WIN32

#ifndef CTCOM_OS_WIN32
#ifndef CTCOM_OS_WINCE
	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath[strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath) - 1] == '/')
		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath[strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath) - 1] = '\0';

	sprintf(szDebugLogFilePath, "%s/messages_%04d_%02d_%02d.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#endif //CTCOM_OS_WINCE
#endif //CTCOM_OS_WIN32

	pfp = fopen(szDebugLogFilePath, "a+");

	if (pfp == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

#ifdef LOG_MANAGEMENT_SIZE
	if (tCurTime - CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->DeviceControlInfo.LastLogDirManagingTime > CTAGENT_LOG_DIRECTORY_MANAGING_TIME_SEC)
	{
		CTAGENT_manageLogDirectory(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->LogPath,
			CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->DeviceControlInfo.LogDirManagingSize);
		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->DeviceControlInfo.LastLogDirManagingTime = tCurTime;
	}

	fseek(pfp, 0, SEEK_END);
	lLogSize = ftell(pfp);

	if (lLogSize > LOG_LIMIT_SIZE)
	{
		FILE *newfp = NULL;
		char szNewDebugLogFilePath[256] = { 0 };

		fclose(pfp);

		strcpy(szNewDebugLogFilePath, szDebugLogFilePath);
		strcpy(szNewDebugLogFilePath + (strlen(szNewDebugLogFilePath) - 4), "_1.txt");

		newfp = fopen(szNewDebugLogFilePath, "rb");

		if (newfp)
		{
			fclose(newfp);

			remove(szNewDebugLogFilePath);
		}

		rename(szDebugLogFilePath, szNewDebugLogFilePath);

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}
#endif

	va_start(strVL, pLogString);		// 가변 인수 시작

	vsprintf(szDebugLog, pLogString, strVL);
#ifndef _USE_LOG_ENCRYPT
	fprintf(pfp, "[%s] %s", szTempLog, szDebugLog);
#else
	sprintf(plainLog, "[%s] %s", szTempLog, szDebugLog);
	CTAGENT_Encrypt_Log(&plainLog, &encLog);

	fprintf(pfp, "%s\r\n", encLog);

	free(encLog);
#endif

	va_end(strVL);						// 가변 인수 끝 

	if (pfp != NULL)
		fclose(pfp);

#endif //DEBUG_FILE_LOG

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	return;
}


void CTAGENT_OLD_CTGLO_DebugLog_s(int ServiceSiteIdIndex, unsigned int level, const char *pLogString, ...)
{
#ifdef DEBUG_FILE_LOG
	FILE			*pfp;
	char			szDebugLogFilePath[256] = { 0 };
	char			szTempLog[256] = { 0 };
	char			szDebugLog[4096] = { 0 };
#ifndef CTCOM_OS_WINCE
	struct	tm		tCurrentTime = { 0 };
	time_t			tCurTime = { 0 };
#endif
#endif
#ifdef LOG_MANAGEMENT_SIZE
	int			lLogSize = 0;
#endif
	va_list			strVL;					// 가변 인수 List 객체 선언
//	char			szbuf[4096] = {0};
	char			szbuf[40960] = { 0 };
#ifdef _USE_LOG_ENCRYPT
	char			plainLog[4096] = { 0 };
	char			*encLog;
#endif

#ifdef LOG_OFF
	return;
#endif

#ifdef IOS_DEVICE
#ifdef DEBUG_CONSOLE_LOG
	{
		struct timeval time_val;
		if (level > LOG_LEVEL)
			return;

		tCurTime = time(&tCurTime);
		tCurrentTime = *(localtime(&tCurTime));
		if (tCurrentTime.tm_year < 1900)
		{
			tCurrentTime.tm_year += 1900;
			tCurrentTime.tm_mon += 1;
		}
		gettimeofday(&time_val, 0);
		sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
			tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec, time_val.tv_usec / 1000);


		va_start(strVL, pLogString);

		vsprintf(szbuf, pLogString, strVL);
		//vfprintf(szbuf, pLogString, strVL);

		//	fprintf(stderr, "%s %s", szTempLog, szbuf);
		CT_AppleLog(szbuf);

		va_end(strVL);
	}

	/*if(level > LOG_LEVEL)
		return;

	va_start(strVL, pLogString);

	vsprintf(szbuf, pLogString, strVL);
	//vfprintf(szbuf, pLogString, strVL);

	fprintf(stderr, "%s", szbuf);

	va_end(strVL);*/
#endif
#endif // #ifdef IOS_DEVICE

	//#ifndef IOS_DEVICE

	if ((ServiceSiteIdIndex < 0) || (ServiceSiteIdIndex >= CTAGENT_MAX_SERVICE_SITE))
		return;

	// Callback Log
	/// \brief Callback log 사용
	/// \author kohen_i
	/// \date 2019-08-29
	if (CTAGENT_GlobalInfo.CallbackLogInfo.UseFlag == CTAGENT_TRUE)
	{
		if (level > LOG_LEVEL)
			return;

		tCurTime = time(&tCurTime);
		tCurrentTime = *(localtime(&tCurTime));
		if (tCurrentTime.tm_year < 1900)
		{
			tCurrentTime.tm_year += 1900;
			tCurrentTime.tm_mon += 1;
		}
		sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
			tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);

		va_start(strVL, pLogString);

		vsprintf(szbuf, pLogString, strVL);

		/// \brief log level은 info 고정하여 사용. log level을 나눠서 모든 로그를 전달할시 키정보도 로그에 남을수 있음
		/// \author kohen_i
		/// \date 2019-08-29
		CTAGENT_GlobalInfo.CallbackLogInfo.LogCallBackParam.LogCall(CTAGENT_GlobalInfo.CallbackLogInfo.LogCallBackParam.LogContext, CT_KTCONNECTEDCAR_LOGLEVEL_INFO, szbuf);

		va_end(strVL);
		return;
	}

	if (CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath == NULL || !strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	if (level > LOG_LEVEL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

#ifdef DEBUG_CONSOLE_LOG
	va_start(strVL, pLogString);

	vsprintf(szbuf, pLogString, strVL);
	//vfprintf(szbuf, pLogString, strVL);

#if defined CTCOM_OS_WIN32
	CTAGENT_OLD_TRACE("%s", szbuf);
#elif defined CTAGENT_ANDROID_LOGCAT
	LOG_V("%s", szbuf);
#else
	fprintf(stderr, "%s", szbuf);
#endif

	va_end(strVL);
#endif

#ifdef DEBUG_FILE_LOG
#ifndef CTCOM_OS_WINCE
	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}
#endif //CTCOM_OS_WINCE

#ifdef CTCOM_OS_WIN32
#ifndef CTCOM_OS_WINCE
	if (CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath[strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath) - 1] == '\\')
		CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath[strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath) - 1] = '\0';

	sprintf(szDebugLogFilePath, "%s\\messages_%04d_%02d_%02d.txt", CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath, tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#endif //CTCOM_OS_WINCE
#endif //CTCOM_OS_WIN32

#ifndef CTCOM_OS_WIN32
#ifndef CTCOM_OS_WINCE
	if (CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath[strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath) - 1] == '/')
		CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath[strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath) - 1] = '\0';

	sprintf(szDebugLogFilePath, "%s/messages_%04d_%02d_%02d.txt", CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath, tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#endif //CTCOM_OS_WINCE
#endif //CTCOM_OS_WIN32

	pfp = fopen(szDebugLogFilePath, "a+");

	if (pfp == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

#ifdef LOG_MANAGEMENT_SIZE
	if (tCurTime - CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].DeviceControlInfo.LastLogDirManagingTime > CTAGENT_LOG_DIRECTORY_MANAGING_TIME_SEC)
	{
		CTAGENT_manageLogDirectory(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].LogPath,
			CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].DeviceControlInfo.LogDirManagingSize);
		CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].DeviceControlInfo.LastLogDirManagingTime = tCurTime;
	}

	fseek(pfp, 0, SEEK_END);
	lLogSize = ftell(pfp);

	if (lLogSize > LOG_LIMIT_SIZE)
	{
		FILE *newfp = NULL;
		char szNewDebugLogFilePath[256] = { 0 };

		fclose(pfp);

		strcpy(szNewDebugLogFilePath, szDebugLogFilePath);
		strcpy(szNewDebugLogFilePath + (strlen(szNewDebugLogFilePath) - 4), "_1.txt");

		newfp = fopen(szNewDebugLogFilePath, "rb");

		if (newfp)
		{
			fclose(newfp);

			remove(szNewDebugLogFilePath);
		}

		rename(szDebugLogFilePath, szNewDebugLogFilePath);

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}
#endif

	va_start(strVL, pLogString);		// 가변 인수 시작

	vsprintf(szDebugLog, pLogString, strVL);
#ifndef _USE_LOG_ENCRYPT
	fprintf(pfp, "[%s] %s", szTempLog, szDebugLog);
#else
	sprintf(plainLog, "[%s] %s", szTempLog, szDebugLog);
	CTAGENT_Encrypt_Log(&plainLog, &encLog);

	fprintf(pfp, "%s\r\n", encLog);

	free(encLog);
#endif

	va_end(strVL);						// 가변 인수 끝 

	if (pfp != NULL)
		fclose(pfp);

#endif //DEBUG_FILE_LOG

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	//#endif // #ifndef IOS_DEVICE
	return;
}
#endif

#ifdef CTAGENT_TEST_OFFLINE
void CTAGENT_OLD_CTGLO_InterfaceLog(int CARMIndex, const char *pLogString, ...)
{
	FILE		*fp;
	va_list		strVL;					// 가변 인수 List 객체 선언
	char		szInterfaceLogFilePath[256];
	struct tm tCurrentTime = { 0 };
	time_t tCurTime = { 0 };

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath == NULL || !strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}

#ifndef CTCOM_OS_WIN32
	sprintf(szInterfaceLogFilePath, "%s/Interface.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath);
#else
	sprintf(szInterfaceLogFilePath, "%s\\Interface.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath);
#endif

	fp = fopen(szInterfaceLogFilePath, "a+");

	if (fp == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

	va_start(strVL, pLogString);		// 가변 인수 시작

	fprintf(fp, "==================%04d/%02d/%02d %02d:%02d:%02d======================\r\n", tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
	vfprintf(fp, pLogString, strVL);
	fprintf(fp, "===========================================================\r\n\n");

	va_end(strVL);						// 가변 인수 끝 

	if (fp != NULL)
		fclose(fp);

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	return;
}
#endif

void CTAGENT_OLD_CTGLO_HistoryLog(int CARMIndex, const char *pLogString, ...)
{
	FILE		*fp;
	va_list		strVL;					// 가변 인수 List 객체 선언
	char		szHistoryLogFilePath[256] = { 0 };
	char		szTempLog[256] = { 0 };
	char		szHistroyLog[2048] = { 0 };
#ifndef CTCOM_OS_WINCE
	struct tm tCurrentTime = { 0 };
	time_t tCurTime = 0;
#else
	SYSTEMTIME tCurrentTime = { 0 };
#endif
#ifdef _USE_LOG_ENCRYPT
	char			plainLog[4096] = { 0 };
	char			*encLog = NULL;
#endif

#ifdef LOG_OFF
	return;
#endif

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite == NULL)
		return;

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath == NULL || !strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

#ifndef CTCOM_OS_WINCE
	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}

#ifndef CTCOM_OS_WIN32	
	sprintf(szHistoryLogFilePath, "%s/CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);
#else
	sprintf(szHistoryLogFilePath, "%s\\CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);
#endif //CTCOM_OS_WIN32

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#else
	GetLocalTime(&tCurrentTime);

	sprintf(szHistoryLogFilePath, "%s\\CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.wYear, tCurrentTime.wMonth, tCurrentTime.wDay);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.wYear, tCurrentTime.wMonth, tCurrentTime.wDay, tCurrentTime.wHour, tCurrentTime.wMinute, tCurrentTime.wSecond);
#endif //CTCOM_OS_WINCE

	fp = fopen(szHistoryLogFilePath, "a+");

	if (fp == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

	va_start(strVL, pLogString);		// 가변 인수 시작

	vsprintf(szHistroyLog, pLogString, strVL);

#ifndef _USE_LOG_ENCRYPT
	fprintf(fp, "[%s] %s\n", szTempLog, szHistroyLog);
	//fprintf(fp, "%s\n", szHistroyLog);
#else
	sprintf(plainLog, "[%s] %s", szTempLog, szHistroyLog);
	CTAGENT_Encrypt_Log(&plainLog, &encLog);

	fprintf(fp, "%s\r\n", encLog);

	free(encLog);
#endif

	va_end(strVL);						// 가변 인수 끝 

	if (fp != NULL)
		fclose(fp);

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	return;
}

void CTAGENT_OLD_CTGLO_HistoryLog_s(int ServiceSiteIdIndex, const char *pLogString, ...)
{
	FILE		*fp;
	va_list		strVL;					// 가변 인수 List 객체 선언
	char		szHistoryLogFilePath[256] = { 0 };
	char		szTempLog[256] = { 0 };
	char		szHistroyLog[2048] = { 0 };
#ifndef CTCOM_OS_WINCE
	struct tm tCurrentTime = { 0 };
	time_t tCurTime = 0;
#else
	SYSTEMTIME tCurrentTime = { 0 };
#endif

#ifdef _USE_LOG_ENCRYPT
	char			plainLog[4096] = { 0 };
	char			*encLog = NULL;
#endif

#ifdef LOG_OFF
	return;
#endif

	if ((ServiceSiteIdIndex < 0) || (ServiceSiteIdIndex >= CTAGENT_MAX_SERVICE_SITE))
		return;

	if (CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].FileLog == 0)
		return;

	if (CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].STBUserInfo.szSetMessagesFilePath == NULL || !strlen(CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].STBUserInfo.szSetMessagesFilePath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

#ifndef CTCOM_OS_WINCE
	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}

#ifndef CTCOM_OS_WIN32	
	sprintf(szHistoryLogFilePath, "%s/CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);
#else
	sprintf(szHistoryLogFilePath, "%s\\CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);
#endif //CTCOM_OS_WIN32

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday, tCurrentTime.tm_hour, tCurrentTime.tm_min, tCurrentTime.tm_sec);
#else
	GetLocalTime(&tCurrentTime);

	sprintf(szHistoryLogFilePath, "%s\\CC4IPTV_%04d_%02d_%02d.txt", CTAGENT_CarmServiceSiteList[ServiceSiteIdIndex].STBUserInfo.szSetMessagesFilePath,
		tCurrentTime.wYear, tCurrentTime.wMonth, tCurrentTime.wDay);

	sprintf(szTempLog, "%04d-%02d-%02d %02d:%02d:%02d",
		tCurrentTime.wYear, tCurrentTime.wMonth, tCurrentTime.wDay, tCurrentTime.wHour, tCurrentTime.wMinute, tCurrentTime.wSecond);
#endif //CTCOM_OS_WINCE

	fp = fopen(szHistoryLogFilePath, "a+");

	if (fp == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}

	va_start(strVL, pLogString);		// 가변 인수 시작

	vsprintf(szHistroyLog, pLogString, strVL);
#ifndef _USE_LOG_ENCRYPT	
	fprintf(fp, "[%s] %s\n", szTempLog, szHistroyLog);
	//fprintf(fp, "%s\n", szHistroyLog);
#else
	sprintf(plainLog, "[%s] %s", szTempLog, szHistroyLog);
	CTAGENT_Encrypt_Log(&plainLog, &encLog);

	fprintf(fp, "%s\r\n", encLog);

	free(encLog);
#endif

	va_end(strVL);						// 가변 인수 끝 

	if (fp != NULL)
		fclose(fp);

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	return;
}

#ifdef DUMP_TS_BUFFER
void CTAGENT_OLD_CTGLO_DumpData(unsigned char *pData, unsigned int nDataLen, int err)
{
	FILE		*fp;
	char		szDumpFilePath[512] = { 0 };
	struct tm tCurrentTime = { 0 };
	time_t tCurTime = 0;

	tCurTime = time(&tCurTime);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurrentTime = *(localtime(&tCurTime));

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}

#ifndef CTCOM_OS_WIN32
	sprintf(szDumpFilePath, "%s/Dump_%d.%d.%d.%d %d %04d-%02d-%02d %02d%02d%02d %x.mpg",
		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath,
		(CTGLO_ulChannelID & 0xFF),
		(CTGLO_ulChannelID & 0xFF00) >> 8,
		(CTGLO_ulChannelID & 0xFF0000) >> 16,
		(CTGLO_ulChannelID & 0xFF000000) >> 24,
		CTGLO_usChannelPort,
		tCurrentTime.tm_year,
		tCurrentTime.tm_mon,
		tCurrentTime.tm_mday,
		tCurrentTime.tm_hour,
		tCurrentTime.tm_min,
		tCurrentTime.tm_sec,
		err);
#else
	sprintf(szDumpFilePath, "%s\\Dump_%d.%d.%d.%d %d %04d-%02d-%02d %02d%02d%02d %x.mpg",
		CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath,
		(CTGLO_ulChannelID & 0xFF),
		(CTGLO_ulChannelID & 0xFF00) >> 8,
		(CTGLO_ulChannelID & 0xFF0000) >> 16,
		(CTGLO_ulChannelID & 0xFF000000) >> 24,
		CTGLO_usChannelPort,
		tCurrentTime.tm_year,
		tCurrentTime.tm_mon,
		tCurrentTime.tm_mday,
		tCurrentTime.tm_hour,
		tCurrentTime.tm_min,
		tCurrentTime.tm_sec,
		err);
#endif

	fp = fopen(szDumpFilePath, "ab+");

	if (fp == NULL)
		return;

	fwrite(pData, 1, nDataLen, fp);

	if (fp != NULL)
		fclose(fp);

	return;
}
#endif

/* Write By NODONG */
/* Log File Manager */
void CTAGENT_OLD_CTGLO_HistoryLogManager(int CARMIndex)
{
	struct tm	tCurrentTime = { 0 };
	struct tm	*pCurrentTime = NULL;
	time_t		tCurTime = 0;
	time_t		tMinusCurTime = LOG_MANAGEMENT_DAY;

#ifdef CTCOM_OS_WIN32
	char		szRemoveFilePath[256] = { 0 };
	char		szTempFilePath[256] = { 0 };
	char		szFile[256] = { 0 };
	char		*pRemoveFilePath = NULL;
	struct		_finddata_t c_file;
	int		hFile = 0;
#else
	char		szTempFilePath[256] = { 0 };
	char		szFile[256] = { 0 };
	char		*szRemoveFilePath = NULL;
	struct dirent *item;
	DIR *dp = NULL;
#endif //CTCOM_OS_WIN32

	if (CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath == NULL || !strlen(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath))
		return;

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_lock(&CTAGENT_OLD_logwrite_mutex);
#else
	EnterCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	tCurTime = time(NULL);

	//if(tCurTime < 0 || tCurTime > 0x7FFFFFFF)
	//	return;

	tCurTime -= tMinusCurTime;

	pCurrentTime = localtime(&tCurTime);
	if (pCurrentTime == NULL)
	{
#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
		pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
		LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif
		return;
	}
	tCurrentTime = *pCurrentTime;

	if (tCurrentTime.tm_year < 1900)
	{
		tCurrentTime.tm_year += 1900;
		tCurrentTime.tm_mon += 1;
	}

#ifdef CTCOM_OS_WIN32
	snprintf(szTempFilePath, sizeof(szTempFilePath), "%04d_%02d_%02d.txt", tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	snprintf(szRemoveFilePath, sizeof(szRemoveFilePath), "%s\\*.txt", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath);

	hFile = _findfirst(szRemoveFilePath, &c_file);
	if (hFile != -1L)
	{
		if (strstr(c_file.name, "messages_") || strstr(c_file.name, "CC4IPTV_"))
		{
			pRemoveFilePath = strchr(c_file.name, '_');

			if (pRemoveFilePath != NULL)
			{
				pRemoveFilePath++;
				if (strcmp(pRemoveFilePath, szTempFilePath) < 0)
				{
					snprintf(szFile, sizeof(szFile), "%s\\%s", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, c_file.name);
					remove(szFile);
				}
			}
		}
		/*
		if(c_file.time_create < tCurTime)
		{
			sprintf(szRemoveFilePath, "%s\\%s", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, c_file.name);
			remove(szRemoveFilePath);
		}
		*/

		while (_findnext(hFile, &c_file) == 0)
		{
			if (strstr(c_file.name, "messages_") || strstr(c_file.name, "CC4IPTV_"))
			{
				pRemoveFilePath = strchr(c_file.name, '_');

				if (pRemoveFilePath != NULL)
				{
					pRemoveFilePath++;
					if (strcmp(pRemoveFilePath, szTempFilePath) < 0)
					{
						snprintf(szFile, sizeof(szFile), "%s\\%s", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, c_file.name);
						remove(szFile);
					}
				}
			}
			/*
			if(c_file.time_create < tCurTime)
			{
				sprintf(szRemoveFilePath, "%s\\%s", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, c_file.name);
				remove(szRemoveFilePath);
			}
			*/
		}

		_findclose(hFile);
	}
#else

	sprintf(szTempFilePath, "%04d_%02d_%02d.txt", tCurrentTime.tm_year, tCurrentTime.tm_mon, tCurrentTime.tm_mday);

	dp = opendir(CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath);
	if (dp != NULL)
	{
		while (1)
		{
			item = readdir(dp);

			if (item == NULL)
				break;

			if (strstr(item->d_name, "messages_") || strstr(item->d_name, "CC4IPTV_"))
			{
				if (strstr(item->d_name, ".txt"))
				{
					szRemoveFilePath = strchr(item->d_name, '_') + 1;

					if (szRemoveFilePath != NULL)
					{
						if (strcmp(szRemoveFilePath, szTempFilePath) < 0)
						{
							sprintf(szFile, "%s/%s", CTAGENT_CarmInfo[CARMIndex].CarmServiceSite->STBUserInfo.szSetMessagesFilePath, item->d_name);
							remove(szFile);
						}
					}
				}
			}
		}

		closedir(dp);
	}
#endif //CTCOM_OS_WIN32

#ifdef LOG_WRITE_MUTEX
#ifndef CTCOM_OS_WIN32
	pthread_mutex_unlock(&CTAGENT_OLD_logwrite_mutex);
#else
	LeaveCriticalSection(&CTAGENT_OLD_logwrite_critical_section);
#endif
#endif

	return;
}
