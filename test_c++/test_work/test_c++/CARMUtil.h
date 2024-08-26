#pragma once
#ifndef CT_CARM_UTIL_H
#define CT_CARM_UTIL_H

///////////////////////////////////////////////////////////////////////////////
// header
#include "CARMAgent_define.h"
//#include "CARMAgent.h"
// header
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// define
#define CTAGENT_LOG_CRITICAL  0x01
#define CTAGENT_LOG_ERROR     0x02
#define CTAGENT_LOG_WARNING   0x04
#define CTAGENT_LOG_NOTICE    0x08
#define CTAGENT_LOG_INFO      0x10
#define CTAGENT_LOG_DEBUG1    0x20
#define CTAGENT_LOG_DEBUG2    0x40
#define CTAGENT_LOG_DEBUG3    0x80

#ifdef CTAGENT_DEBUG_LOG
#ifndef CTAGENT_LOG_LEVEL
#define CTAGENT_LOG_LEVEL (CTAGENT_LOG_CRITICAL | CTAGENT_LOG_ERROR | CTAGENT_LOG_WARNING | CTAGENT_LOG_NOTICE \
					| CTAGENT_LOG_INFO | CTAGENT_LOG_DEBUG1 | CTAGENT_LOG_DEBUG2 | CTAGENT_LOG_DEBUG3)
		//#define CTAGENT_LOG_LEVEL (CTAGENT_LOG_CRITICAL | CTAGENT_LOG_ERROR | CTAGENT_LOG_INFO)
#endif
#ifdef CTAGENT_DEBUG_FILE_LOG
#ifdef LOG_FILE_SIZE_MANAGEMENT
#ifndef CTAGENT_LOG_FILE_SIZE
#define CTAGENT_LOG_FILE_SIZE (1024*1024*5) // 5MBytes
#endif
#endif
#endif
#endif

#define CTAGENT_MAX_LOG_LENGTH				4096

// Mutex
#ifdef CTAGENT_OS_WINDOWS
#define CTAGENT_MUTEX_INITIALIZE(mutex, flag) \
		do { \
			if(!flag) \
			{ \
				InitializeCriticalSection(mutex); \
				flag=1; \
			} \
		} while(0)
#define CTAGENT_MUTEX_LOCK(mutex) EnterCriticalSection(mutex)
#define CTAGENT_MUTEX_UNLOCK(mutex) LeaveCriticalSection(mutex)
#else
	//extern pthread_mutex_t CTAGENT_InitialMutex;
#define CTAGENT_MUTEX_INITIALIZE(mutex, flag) \
		do { \
			if(!flag) \
			{ \
				/*memcpy(mutex, &CTAGENT_InitialMutex, sizeof(pthread_mutex_t));*/ \
				pthread_mutex_init(mutex, NULL); \
				flag=1; \
			} \
		} while(0)
#define CTAGENT_MUTEX_LOCK(mutex) pthread_mutex_lock(mutex)
#define CTAGENT_MUTEX_UNLOCK(mutex) pthread_mutex_unlock(mutex)
#endif

// Signal
#ifdef CTAGENT_OS_WINDOWS
#define CTAGENT_SIGNAL_INITIALIZE(signal) {if(*signal == NULL) {*signal = CreateEvent(NULL, FALSE, FALSE, NULL);}}
#define CTAGENT_SIGNAL_WAIT(signal, usec) CTAGENT_signal_wait(signal, usec)
#define CTAGENT_SIGNAL_POST(signal) {SetEvent(*signal);}
#else
#define CTAGENT_SIGNAL_INITIALIZE(signal) sem_init(signal, 0, 0)
#define CTAGENT_SIGNAL_WAIT(signal, usec) CTAGENT_signal_wait(signal, usec)
#define CTAGENT_SIGNAL_POST(signal) sem_post(signal)
#endif

#define CT_setIntValueInPointer(pointer, value) \
	{ \
	if(pointer != NULL) \
	*(pointer) = (value); \
	} while(0)

#define CT_setErrorValueInPointer(pointer, value) CT_setIntValueInPointer(pointer, value)

#define CT_getIntValueInPointer(pointer) pointer==NULL?0:*pointer
#define CT_getErrorValueInPointer(pointer) CT_getIntValueInPointer(pointer)

#ifdef CTAGENT_BIG_ENDIAN
#define CT_swapLong(variable, value) {variable = htonl(value);}
#define CT_swapShort(variable, value) {variable = htons(value);}
#else
#define CT_swapLong(variable, value)
#define CT_swapShort(variable, value)
#endif

#ifdef CTAGENT_ANDROID_LOGCAT
#include <jni.h>
#include <android/log.h>
#define LOG_V(...) __android_log_print(ANDROID_LOG_VERBOSE, "CARMAgent150", __VA_ARGS__)
#endif

// KT Connected Car log level
/// \brief Callback log ���
/// \author kohen_i
/// \date 2019-08-30
#define CT_KTCONNECTEDCAR_LOGLEVEL_TRACE 0
#define CT_KTCONNECTEDCAR_LOGLEVEL_DEBUG 1
#define CT_KTCONNECTEDCAR_LOGLEVEL_INFO  2
#define CT_KTCONNECTEDCAR_LOGLEVEL_WARN  3
#define CT_KTCONNECTEDCAR_LOGLEVEL_ERROR 4
#define CT_KTCONNECTEDCAR_LOGLEVEL_FATAL 5

#ifndef uint8_t
//#define uint8_t		unsigned char
typedef unsigned char uint8_t;
#endif


// define
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// struct

// struct
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// variable
// variable
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// function
unsigned short CT_CRC16(unsigned char *p, int len);
int CTAGENT_signal_wait(CTAGENT_SIGNAL *signal, int usec);

#ifdef CTAGENT_OS_WINDOWS
void usleep(int usec);
#endif


int CTAGENT_gettimeofday(struct timeval* tp, int* tz);
# define CTAGENT_timeradd(a, b, result)                               \
	do {                                                      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;             \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;          \
    if ((result)->tv_usec >= 1000000)                         \
	{                                                         \
    ++(result)->tv_sec;                                       \
    (result)->tv_usec -= 1000000;                             \
	}                                                         \
	} while (0)
# define CTAGENT_timersub(a, b, result)                               \
	do {                                                      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;          \
    if ((result)->tv_usec < 0) {                              \
	--(result)->tv_sec;                                       \
	(result)->tv_usec += 1000000;                             \
    }                                                         \
	} while (0)


#define AV_BASE64_DECODE_SIZE(x) ((x) * 3LL / 4)
#define AV_BASE64_SIZE(x)  (((x)+2) / 3 * 4 + 1)

int CTAGENT_DeleteFile(int ServiceSiteIdIndex, int CertType, char *szDeleteFilePath);

char *CTAGENT_encode_base64(char *out, int out_size, unsigned char *in, int in_size);
int CTAGENT_decode_base64(uint8_t * out, const char *in, int out_length);
int CTAGENT_getIPPortInURL(int ServiceSiteIndex, char *URL, unsigned int *IP, char *StringIP, unsigned short *Port);
int CT_strincmp(char *p1, char *p2, int len);
char *CTAGENT_findToken(char *Buffer, int BufferSize, char *Token, int TokenSize);
int CTAGENT_isHdmiBlockDevice(int ServiceSiteIndex);
int CTAGENT_checkUseHdmi(int ServiceSiteIndex);

char *CT_strlwr(char *str);

int CTAGENT_manageLogDirectory(char *LogPath, int ManageSize);
int CTAGENT_CopyFile(const char *dst_filepath, const char *src_filepath);

// function
///////////////////////////////////////////////////////////////////////////////

#endif
