/*
*
* @file		Card.h
* @date     Wed May  9 14:16:06 2018
* @author
*/

#pragma once
#include "GameObject.h"
#include "dsSpriteManager.h"
#include "GAME_INFO.h"


class Card : public GameObject
{
private:
	dsSprite * cardSprite;
	dsSprite * backSprite;
	int key; //카드 키가 같으면 같은 카드이다.
	bool isOpen; //열려 있는지 닫혀 있는지 확인.
	bool isEnd; //짝을 찾은 카드 인지.
	DWORD ani_time;
	DWORD interval;
public:
	void SetData(dsSprite * _cardSprite, dsSprite * _backSprite, 
		const int x, const int y, const int _key);
	void DrawImg(Graphics * pGraphics);
	bool GetIsOpen();
	bool GetIsEnd();
	void SetEnd();
	void SetOpen(bool _isOpen);
	void Update(DWORD tick, bool * isTimeOver);
	int GetKey();
};

