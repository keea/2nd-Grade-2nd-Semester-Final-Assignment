/*
*
* @file		Card.cpp
* @date     Wed May  9 14:16:27 2018
* @author
*/

#include "Card.h"

void Card::SetData(dsSprite * _cardSprite, dsSprite * _backSprite,
	const int x, const int y, const int _key) {
	cardSprite = _cardSprite;
	backSprite = _backSprite;
#ifdef _DEBUG_MODE //전처리 문. 만약에 _COLLSION 이 define 되어있으면 컴파일 해.
	cardSprite = _backSprite;
	backSprite = _cardSprite;
#endif
	isOpen = false;
	isEnd = false;
	SetX(x);
	SetY(y);
	key = _key;
	ani_time = CARD_ANI_TIME;
	interval = 0;
}

void Card::DrawImg(Graphics * pGraphics) {
	if (isOpen)
		cardSprite->Draw(pGraphics, GetX(), GetY(), CARD_WIDTH, CARD_HEIGHT);
	else
		backSprite->Draw(pGraphics, GetX(), GetY(), CARD_WIDTH, CARD_HEIGHT);
}

void Card::SetOpen(bool _isOpen) {
	isOpen = _isOpen;
}

bool Card::GetIsOpen() {
	return isOpen;
}

bool Card::GetIsEnd() {
	return isEnd;
}

void Card::SetEnd() {
	isOpen = true;
	isEnd = true;
}

int Card::GetKey() {
	return key;
}

void Card::Update(DWORD tick, bool * isTimeOver) {
	interval += tick;
	if (interval >= ani_time) {
		SetOpen(false);
		*isTimeOver = true;
		interval = 0;
	}
}
