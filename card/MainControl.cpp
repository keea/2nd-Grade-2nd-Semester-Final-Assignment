/*
*
* @file		MainControl
* @date     Fri May 11 14:16:52 2018
* @author
*/

#include "MainControl.h"

void MainControl::Init(ImageManager * _p_imageManager) {
	p_imageManager = _p_imageManager;
	button.SetData(
		p_imageManager->GetCreateImage("start_on.png"),
		p_imageManager->GetCreateImage("start_off.png"));
}

void MainControl::Display(Graphics * pGraphics) {
	button.DrawImg(pGraphics);
}

void MainControl::UpperMouseEvent(const int x, const int y) {
	if (IsContains(x, y)) {
		button.SetState(ON);
	}
	else {
		button.SetState(OFF);
	}
}

//충돌할 경우 현재 씬을 바꾼다.
void MainControl::ClickEvent(const int x, const int y, int * currentScene) {
	if (IsContains(x, y)) {
		*currentScene = GAME;
	}
}

bool MainControl::IsContains(int x, int y) {
	Rect colRect(button.GetX(), button.GetY(), BTN_WIDTH, BTN_HEIGHT);
	return colRect.Contains(x, y);
}