/*
*
* @file		ResultControl.cpp
* @date     Tue May 15 15:53:22 2018
* @author
*/

#include "ResultControl.h"

void ResultControl::Init(ImageManager * _p_imageManager) {
	p_imageManager = _p_imageManager;
	button.SetData(
		p_imageManager->GetCreateImage("restart_on.png"),
		p_imageManager->GetCreateImage("restart_off.png"));

	winImg = p_imageManager->GetCreateImage("win.png");
	loseImg = p_imageManager->GetCreateImage("lose.png");
}

void ResultControl::Display(Graphics * pGraphics) {
	if (resultType == WIN)
		winImg->Draw(pGraphics, 512 - (BTN_WIDTH / 4), 200, BTN_WIDTH, BTN_HEIGHT);
	else if(resultType == LOSE)
		loseImg->Draw(pGraphics, 512 - (BTN_WIDTH / 4), 200, BTN_WIDTH, BTN_HEIGHT);
	button.DrawImg(pGraphics);
}

void ResultControl::UpperMouseEvent(const int x, const int y) {
	if (IsContains(x, y)) {
		button.SetState(ON);
	}
	else {
		button.SetState(OFF);
	}
}

//�浹�� ��� ���� ���� �ٲ۴�.
void ResultControl::ClickEvent(const int x, const int y, int * currentScene) {
	if (IsContains(x, y)) {
		*currentScene = MAIN;
	}
}

bool ResultControl::IsContains(int x, int y) {
	Rect colRect(button.GetX(), button.GetY(), BTN_WIDTH, BTN_HEIGHT);
	return colRect.Contains(x, y);
}

//���� Ÿ�� ����
void ResultControl::SetResultType(int _resultType) {
	resultType = _resultType;
}