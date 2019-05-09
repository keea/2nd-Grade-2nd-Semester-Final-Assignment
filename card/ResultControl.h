/*
*
* @file		ResultControl.h
* @date     Tue May 15 15:20:27 2018
* @author
*/

#pragma once
#include "ImageManager.h"
#include "Button.h"
#include "GAME_INFO.h"

class ResultControl
{
private:
	Button button;
	ImageManager * p_imageManager;
	dsSprite * winImg;
	dsSprite * loseImg;

	int resultType;
	//충돌검사
	bool IsContains(int x, int y);
public:
	//초기화
	void Init(ImageManager * _p_imageManager);
	//화면 보여주기
	void Display(Graphics * pGraphics);
	//마우스 동작
	void UpperMouseEvent(const int x, const int y);
	//클릭 동작
	void ClickEvent(const int x, const int y, int * currentScene);
	//상태값 추가.
	void SetResultType(int _resultType);
};

