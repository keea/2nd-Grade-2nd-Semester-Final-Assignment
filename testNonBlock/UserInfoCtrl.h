#pragma once
#include <map>
#include "packet.h"
#include "UserInfo.h"

using namespace std;
#define USER_MAP std::map< DWORD, UserInfo> 

class UserInfoCtrl
{
private:
	USER_MAP userInfos;
	void Add(PACKETHEADER userPacket);
public:
	void AddUserInfo(PACKETHEADER userPacket);
	void ShowUsers(RECT Winsize, HDC hdc);
	void DelUserInfo(DWORD userInfo);
};

