// test_simple.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdarg.h>
#include <tchar.h>
#include "def.h"

void WriteLog(wchar_t *fmt, ...)
{
	wchar_t	 LogMessage[1024 + 1];

	memset(LogMessage, 0, 1024 + 1);
	va_list vargs;
	va_start(vargs, fmt);
	_vsntprintf(LogMessage, sizeof(LogMessage), fmt, vargs);
	va_end(vargs);

	LogMessage[1024] = '\0';
	printf("%s\n", LogMessage);
}

int main()
{

	if (true) {
		wchar_t tmp[100];
		for (int i = 0; i < 100; i++) {
			tmp[i] = '1';
		}
		WriteLog((wchar_t*)("tmp %s"), tmp);

		return 0;
	}

	PACKET_INFO_RES_TEST1 test1;
	PACKET_INFO_RES_TEST2 test2;
	int size1 = sizeof(test1);
	int size2 = sizeof(test2);

	printf("size 1, 2 : %d, %d\n", size1, size2);

	test1.sz_SplunkUrl = (unsigned char*)malloc(6);
	test2.sz_SplunkUrl = (unsigned char*)malloc(6);
	test1.sz_Reserved = (unsigned char*)malloc(6);
	test2.sz_Reserved = (unsigned char*)malloc(6);


	strncpy((char *)test1.sz_SplunkUrl, "abcde", 6);
	strncpy((char *)test2.sz_SplunkUrl, "abcde", 6);
	strncpy((char *)test1.sz_Reserved, "abcde", 6);
	strncpy((char *)test2.sz_Reserved, "abcde", 6);

	size1 = sizeof(test1);
	printf("size 1, 2 : %d, %d\n", size1, size2);

	printf("%s\n", test1.sz_SplunkUrl);

	test1.ul_SplunkUrlLen = strlen((const char*)test1.sz_SplunkUrl) + 1;
	test1.ul_ReservedLen = strlen((const char*)test1.sz_Reserved) + 1;
	test1.ul_BaseGenre = 10;
	
	

	unsigned char * aa = reinterpret_cast<unsigned char*>(&test1);

	for (int i = 0; i < 45; i++) {
		printf("%02x ", aa[i]);
	}
	printf("\n");

	/*
	char* a = NULL;
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING", '\0', "", NULL, nullptr);
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING",'\0',"", a ? a : "" ,nullptr);
	*/


}
