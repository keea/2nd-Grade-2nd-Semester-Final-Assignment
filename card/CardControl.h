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
	int beforeIdx; //이전 열림 카드 인덱스 번호. 없을 경우 999, 열린카드가 있을 경우 -1
public:
	//초기화
	void Init(ImageManager * _p_imageManager);
	//카드들 생성
	void CreateCard();
	//카드들을 보여준다.
	void DisplayCard(Graphics * pGraphics);
	//클릭시 동작 처리
	void ClickEvent(const int x, const int y, ScoreControl * pScoreControl);
	//카드들 업데이트 동작
	void CardsUpdate(DWORD tick , ScoreControl * pScoreControl);

	//씬을 넘기기
	void ChangeScene(bool * isGameEnd);

	~CardControl();
};

