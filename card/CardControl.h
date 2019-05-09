/*
*
* @file		CardControl.h
* @date     Wed May  9 16:36:13 2018
* @author
*/

#pragma once
#include "Card.h"
#include "ImageManager.h"
#include "GAME_INFO.h"
#include "ScoreControl.h"

typedef Card * pCard;

class CardControl
{
private:
	pCard cards;
	ImageManager * p_imageManager;
	bool isTimeOver;
	int beforeIdx; //���� ���� ī�� �ε��� ��ȣ. ���� ��� 999, ����ī�尡 ���� ��� -1
public:
	//�ʱ�ȭ
	void Init(ImageManager * _p_imageManager);
	//ī��� ����
	void CreateCard();
	//ī����� �����ش�.
	void DisplayCard(Graphics * pGraphics);
	//Ŭ���� ���� ó��
	void ClickEvent(const int x, const int y, ScoreControl * pScoreControl);
	//ī��� ������Ʈ ����
	void CardsUpdate(DWORD tick , ScoreControl * pScoreControl);

	//���� �ѱ��
	void ChangeScene(bool * isGameEnd);

	~CardControl();
};

