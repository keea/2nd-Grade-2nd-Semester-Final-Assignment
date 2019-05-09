#pragma once
#include "dsSpriteManager.h"
#include <Windows.h>
#include <string>
using namespace std;
class Keyboard
{
private:
	Rect colRect[3];
	bool isAnswer; //�������� �Ǵ�.
	bool isClick; //Ŭ���� �����Ѱ�?
	bool isShowJud; //�������� �����־��°�?
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

