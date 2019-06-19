#include "UserInfoCtrl.h"
#include <algorithm>
#include <vector>

void UserInfoCtrl::Add(LOGIN userPacket)
{
	//추가 한다.
	UserInfo info;
	info.userID = userPacket.userID;
	strcpy(info.userName, userPacket.userStrID);
	//색상을 랜덤하게 설정.
	info.color = RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255);
	info.isLogin = true;
	info.score = userPacket.score;

	userInfos[info.userID] = info;
	SortData();
}

void UserInfoCtrl::AddUserInfo(LOGIN userPacket)
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
	int posX = width - 25;
	int posY = 10;
	for (USER_SORT_VECTOR::iterator it = printData.begin(); it != printData.end(); it++) {
		char bufScore[256];
		sprintf(bufScore, "%d", it->second.score);

		SetTextAlign(hdc, TA_RIGHT);//오른쪽 정렬
		SetTextColor(hdc, it->second.color); //글자색 변경.
		string name = it->second.userName;
		//자기자신 구분하기.
		if (strcmp(myUserInfo.userName, it->second.userName) == 0) //같으면 0
		{
			name = "★ " + name;	
		}
		TextOut(hdc, posX, posY, name.c_str(), strlen(name.c_str()));
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
	}
	SortData();
}

void UserInfoCtrl::SetMyInfo(DWORD userID)
{
	myUserInfo = userInfos[userID];
}

bool UserInfoCtrl::GetColor(DWORD userID, COLORREF * color)
{
	if (userInfos.size() <= 0) { //맵의 사이즈가 0이라는 것은 뭔가 문제가 있다.
		return false;
	}

	if (userInfos.find(userID) != userInfos.end()) {
		*color = userInfos[userID].color;
		return true;
	}

	return false;
}

bool UserInfoCtrl::AddScore(DWORD userID)
{
	if (userInfos.size() <= 0)
		return false;

	if (userInfos.find(userID) == userInfos.end()) { //못 찾을 경우 실패
		return false;
	}

	userInfos[userID].score += 100;
	myUserInfo = userInfos[myUserInfo.userID];
	SortData();
}

bool UserInfoCtrl::DownScore(DWORD userID, int dieNum)
{
	if (userInfos.size() <= 0)
		return false;

	if (userInfos.find(userID) == userInfos.end()) { //못 찾을 경우 실패
		return false;
	}

	userInfos[userID].score -= (dieNum * 50);
	if (userInfos[userID].score < 0)
		userInfos[userID].score = 0;

	myUserInfo = userInfos[myUserInfo.userID];
	SortData();
}

void UserInfoCtrl::SortData()
{
	//벡터로 변환해서 정렬.
	printData.clear(); //일단 클리어한다.
	for (auto iter = userInfos.begin(); iter != userInfos.end(); ++iter) {
		printData.emplace_back(std::make_pair(iter->first, iter->second));
	}

	sort(printData.begin(), printData.end(), SCORE_COMP());
}

UserInfoCtrl::~UserInfoCtrl()
{
	printData.clear();
	userInfos.clear();
}


