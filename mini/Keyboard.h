#pragma once
#include "dsSpriteManager.h"
#include <Windows.h>
#include <string>
using namespace std;
class Keyboard
{
private:
	Rect colRect[3];
	bool isAnswer; //정답인지 판단.
	bool isClick; //클릭이 가능한가?
	bool isShowJud; //판정값을 보여주었는가?
	wstring quest;
	int step;
public:
	void Init();
	void Draw(Graphics * backBuffer);
	bool OnClick(POINT p, int answer);
	void SetQuest(wstring w);
	bool getIsAnswer();
	void setAbleClick(bool b);
	bool getAbleClick() const;
	int getStep();
	void ResetStep();
};

