#include <windows.h>
#include <vector>
#pragma once

struct UserInfo
{
	COLORREF	color; //유저 색상.
	DWORD		userID; //유저 아이디.
	char		userName[20]; //유저 이름
	int			score;	//유저 점수
	bool		isLogin = false; //로그인 했는가?
};

struct SCORE_COMP {
	bool operator()(const std::pair<DWORD, UserInfo>& user1, const std::pair<DWORD, UserInfo>& user2) {
		return user1.second.score > user2.second.score;
	}
};