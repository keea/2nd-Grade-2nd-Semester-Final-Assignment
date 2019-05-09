/*
*
* @file		MainControl.h
* @date     Fri May 11 14:08:45 2018
* @author
*/

#pragma once
#include "ImageManager.h"
#include "Button.h"
#include "GAME_INFO.h"
class MainControl
{
private:
	Button button;
	ImageManager * p_imageManager;
	//�浹�˻�
	bool IsContains(int x, int y);
public:
	//�ʱ�ȭ
	void Init(ImageManager * _p_imageManager);
	//����ȭ�� �����ֱ�
	void Display(Graphics * pGraphics);
	//���콺 ����
	void UpperMouseEvent(const int x, const int y);
	//Ŭ�� ����
	void ClickEvent(const int x, const int y, int * currentScene);
};

