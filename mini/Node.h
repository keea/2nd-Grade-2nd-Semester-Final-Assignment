#pragma once
#include "dsSpriteManager.h"
#include <string>

using namespace std;
class Node
{
private:
	wstring words[3]; //글자들
	int posY;
	DWORD speed;
	DWORD interval;
	int judgment; //판정
	bool isClick;
	DWORD timer;
	
	int sortNums[3]; //정답이 어디에 있는지 판단.
public:
	void Init();
	int Update(Graphics * g_BackBuffer, DWORD tick, bool * isNewNode);
	int getJud(); //판정 결과 값 얻기
	bool getIsClick();
	void SetQuest(wstring w, int pos[]);
	void setClick(bool _isClick);
	int getSortBox(int i);
};

