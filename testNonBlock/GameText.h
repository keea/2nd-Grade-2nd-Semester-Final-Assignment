#include <windows.h>
#pragma once
struct GameText
{
	char		text[20]; //추가되는 텍스트
	DWORD		userID; //이거 추가한 사람 아이디.
	POINT		pos; //위치 값.
	COLORREF	color; //텍스트 색상.
	DWORD		tick = 0;
};