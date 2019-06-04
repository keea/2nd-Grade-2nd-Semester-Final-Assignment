#include "UserInfoCtrl.h"

void UserInfoCtrl::Add(PACKETHEADER userPacket)
{
	//추가 한다.
	UserInfo info;
	info.userID = userPacket.userID;
	strcpy(info.userName, userPacket.userStrID);
	//색상을 랜덤하게 설정.
	info.color = RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255);
	info.score = 0;
	info.isLogin = true;

	userInfos[info.userID] = info;
}

void UserInfoCtrl::AddUserInfo(PACKETHEADER userPacket)
{
	if (userInfos.size() <= 0) { //처음이니까 저장한다.
		Add(userPacket);
		return;
	}

	if (userInfos.find(userPacket.userID) == userInfos.end()) { //못 찾을 경우 추가한다.
		Add(userPacket);
		return;
	}
}

void UserInfoCtrl::ShowUsers(RECT winSize, HDC hdc)
{
	int width = winSize.right;
	int posX = width - 100;
	int posY = 10;
	for (USER_MAP::iterator it = userInfos.begin(); it != userInfos.end(); it++) {
		char bufScore[256];
		sprintf(bufScore, "%d", it->second.score);

		SetTextColor(hdc, it->second.color); //글자색 변경.
		TextOut(hdc, posX, posY, it->second.userName, strlen(it->second.userName));
		posY += 20;
		TextOut(hdc, posX, posY, bufScore, strlen(bufScore));
		posY += 50;
	}
}

void UserInfoCtrl::DelUserInfo(DWORD userID)
{
	if (userInfos.size() <= 0) { //맵의 사이즈가 0이라는 것은 뭔가 문제가 있다.
		return;
	}

	if (userInfos.find(userID) != userInfos.end()) { //존재할 경우 삭제한다.
		userInfos.erase(userID); //삭제
		return;
	}
}

void UserInfoCtrl::SetMyInfo(DWORD userID)
{
	myUserInfo = userInfos[userID];
}


