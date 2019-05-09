#pragma once
#include "dsSpriteManager.h"
#include <string>

using namespace std;
class Node
{
private:
	wstring words[3]; //���ڵ�
	int posY;
	DWORD speed;
	DWORD interval;
	int judgment; //����
	bool isClick;
	DWORD timer;
	
	int sortNums[3]; //������ ��� �ִ��� �Ǵ�.
public:
	void Init();
	int Update(Graphics * g_BackBuffer, DWORD tick, bool * isNewNode);
	int getJud(); //���� ��� �� ���
	bool getIsClick();
	void SetQuest(wstring w, int pos[]);
	void setClick(bool _isClick);
	int getSortBox(int i);
};

