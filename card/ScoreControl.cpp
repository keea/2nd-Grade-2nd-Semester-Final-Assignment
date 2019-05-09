/*
*
* @file		ScoreClass.cpp
* @date     Fri May 11 15:53:05 2018
* @author
*/
#include "ScoreControl.h"

void ScoreControl::Init() {
	score = 0;
	combo = 0;
}

void ScoreControl::AddScore() {
	score += 100 + (50 * combo);
}

void ScoreControl::AddCombo() {
	combo += 1;
}

void ScoreControl::ClearCombo() {
	combo = 0;
}

void ScoreControl::DisplayScore(Graphics * pGraphics, dsSprite ** sprites , dsSprite * comboImg) {
	char imageIndex[4];
	int idx = 0;
	int value = 1000;
	int tempScore = score;
	for (; value >= 10; value /= 10) {
		imageIndex[idx++] = tempScore / value;
		tempScore = tempScore % value;
	}

	imageIndex[3] = tempScore;

	int x = 950;

	//점수 표기
	for (int i = 0; i < 4; i++) {
		sprites[imageIndex[i]]->Draw(pGraphics, x, 0, NUM_WIDTH, NUM_HEIGTH);
		x += 50;
	}

	sprites[combo]->Draw(pGraphics, 25, 620, NUM_WIDTH, NUM_HEIGTH);
	comboImg->Draw(pGraphics, 100, 600, BTN_WIDTH, BTN_HEIGHT);
}