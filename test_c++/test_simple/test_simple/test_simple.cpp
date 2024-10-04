// test_simple.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

int main()
{
	/*
	char* a = NULL;
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING", '\0', "", NULL, nullptr);
	printf("NOTTING=%s,\\0=%s,\"\"=%s,NULL=%s,nullptr=%s\n", (char *)"NOTTHING",'\0',"", a ? a : "" ,nullptr);
	*/

	int  a = 10;
	int *p = &a;
	int &ref_a = a;

	return 0;
}
