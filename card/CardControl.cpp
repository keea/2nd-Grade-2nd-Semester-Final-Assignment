/*
*
* @file		CardControl.cpp
* @date     Wed May  9 16:38:22 2018
* @author
*/

#include "CardControl.h"

void CardControl::Init(ImageManager * _p_imageManager) {
	cards = new Card[CARD_IMAGE_NUM * 2];
	p_imageManager = _p_imageManager;
	beforeIdx = 999;
	isTimeOver = false;
}

void CardControl::CreateCard() {
	int maxNum = CARD_IMAGE_NUM*2;
	int temps[CARD_IMAGE_NUM * 2];
	int count = 0;
	for (int i = 0; i < CARD_IMAGE_NUM;) {
		
		temps[count++] = i;
		temps[count++] = i;
		i ++;
	}
	for (int y = 0; y < CARD_ROW; y++) {
		for (int x = 0; x < CARD_COL; x++) {
			int idx = (y * CARD_COL) + x;
			int randNum = rand() % maxNum;
			if (maxNum >= 0) 
			{
				cards[idx].SetData(p_imageManager->getImage(temps[randNum]), 
					p_imageManager->GetCreateImage("back.png") ,
					(x + 1) *200, ((y) * 200)+50, temps[randNum]);
				maxNum -= 1;
				temps[randNum] = temps[maxNum];
			}
		}
	}
}

void CardControl::DisplayCard(Graphics * pGraphics) {
	//cards[0].DrawImg(pGraphics);
	for (int i = 0; i < CARD_IMAGE_NUM*2; i++) {
		cards[i].DrawImg(pGraphics);
	}
}

void CardControl::ClickEvent(int x, int y, ScoreControl * pScoreControl) {
	for (int i = 0; i < CARD_IMAGE_NUM * 2; i++) {
		Rect colRect(cards[i].GetX(), cards[i].GetY(), CARD_WIDTH, CARD_HEIGHT);
		if (colRect.Contains(x, y) == TRUE) {
			//�̹� ¦�� ã�� ī��ų�, ���� ī�尡 �ƴ� ���
			if(!cards[i].GetIsEnd() && !cards[i].GetIsOpen() && !(beforeIdx==-1)){
				cards[i].SetOpen(true);

				if (beforeIdx == 999) {//�������� ����.
					beforeIdx = i;
				}
				
				else { //������ ���
					//cards[i].SetOpen(true);
					if (cards[beforeIdx].GetKey() == cards[i].GetKey()) { //������ �� ī���� ���� ���� ���
						cards[i].SetEnd();
						cards[beforeIdx].SetEnd();
						pScoreControl->AddScore();
						pScoreControl->AddCombo();
					}
					else {
						pScoreControl->ClearCombo();
					}
					beforeIdx = -1;
				}
			}
		}
	}
}

void CardControl::CardsUpdate(DWORD tick, ScoreControl * pScoreControl) {
	int count = 0;
	for (int i = 0; i < CARD_IMAGE_NUM * 2; i++) {
		if (cards[i].GetIsOpen() && !cards[i].GetIsEnd()) { //���� ī�尡 �ƴѵ� ���µ� ���� ���
			cards[i].Update(tick, &isTimeOver);
			count += 1;
		}	
	}

	if (count == 0)
		beforeIdx = 999;

	if (isTimeOver) {
		pScoreControl->ClearCombo();
		isTimeOver = false;
	}
}

void CardControl::ChangeScene(bool * isGameEnd) {
	for (int i = 0; i < CARD_IMAGE_NUM * 2; i++) {
		if (!cards[i].GetIsEnd())
			return;
	}
	*isGameEnd = true;
}


CardControl::~CardControl()
{
	delete[] cards;
}
