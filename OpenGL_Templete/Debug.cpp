#include "Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void keea::Debug(char * fmt, ...)
{
	va_list ap; //�������� ���
	int i = 0;

	char Buf[1024] = { 0, };

	va_start(ap, fmt);
	vsprintf(Buf, fmt, ap);  //Buf ������ ���Ŀ� ���� ������� ���ڿ��� ����ȴ�.
	va_end(ap);

	OutputDebugString(Buf);
}
