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
	int key; //ī�� Ű�� ������ ���� ī���̴�.
	bool isOpen; //���� �ִ��� ���� �ִ��� Ȯ��.
	bool isEnd; //¦�� ã�� ī�� ����.
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

