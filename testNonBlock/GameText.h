#include <windows.h>
#pragma once
struct GameText
{
	char		text[20]; //�߰��Ǵ� �ؽ�Ʈ
	DWORD		userID; //�̰� �߰��� ��� ���̵�.
	POINT		pos; //��ġ ��.
	COLORREF	color; //�ؽ�Ʈ ����.
	DWORD		tick = 0;
};