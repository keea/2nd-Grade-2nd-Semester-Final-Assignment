#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void keea::Debug(char * fmt, ...)
{
	va_list ap; //가변인자 목록
	int i = 0;

	char Buf[1024] = { 0, };

	va_start(ap, fmt);
	vsprintf(Buf, fmt, ap);  //Buf 변수에 형식에 따라 만들어진 문자열이 저장된다.
	va_end(ap);

	OutputDebugString(Buf);
}
