#pragma once
#include <map>
#include "packet.h"
#include "UserInfo.h"

using namespace std;
#define USER_MAP std::map< DWORD, UserInfo> 
#define USER_SORT_VECTOR std::vector< std::pair< DWORD, UserInfo > >

class UserInfoCtrl
{
private:
	USER_MAP userInfos;
	void Add(LOGIN userPacket);
	UserInfo myUserInfo; //내 정보 저장.
	USER_SORT_VECTOR printData;
public:
	void AddUserInfo(LOGIN userPacket);
	void ShowUsers(RECT Winsize, HDC hdc);
	void DelUserInfo(DWORD userInfo);
	void SetMyInfo(DWORD userID);
	DWORD GetMyID() { return myUserInfo.userID; };
	bool GetColor(DWORD userID, COLORREF * color);
	UserInfo GetMyInfo(){ return myUserInfo; };
	bool AddScore(DWORD userID); //성공/실패 반환.
	bool DownScore(DWORD userID, int dieNum);
	void SortData();
	~UserInfoCtrl();
};

