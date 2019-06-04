#include <windows.h>
#pragma once

struct UserInfo
{
	COLORREF	color; //유저 색상.
	DWORD		userID; //유저 아이디.
	char		userName[20]; //유저 이름
	int			score = 0;	//유저 점수
	bool		isLogin = false; //로그인 했는가?
};