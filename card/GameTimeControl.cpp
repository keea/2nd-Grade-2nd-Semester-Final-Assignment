/*
*
* @file		GameTimeControl.cpp
* @date     Fri May 11 10:40:54 2018
* @author
*/

#include "GameTimeControl.h"


void GameTimeControl::Init() {
	timer = TIMER;
	interval = 0;
	isNotEnd = true;
}

void GameTimeControl::TimerUpdate(DWORD tick , bool * currentScene) {
	if (timer <= 0) {
		isNotEnd = false;
		*currentScene = true;
		return;
	}
		

	interval += tick;

	if (interval >= 1000) {
		timer -= 1;
		interval -= 1000;
	}
}

void GameTimeControl::DisplayTimer(Graphics * pGraphics, dsSprite ** sprites) {
	if (isNotEnd) {
		char imgIndex[2];
		imgIndex[0] = timer / 10;
		imgIndex[1] = timer % 10;

		int x = 512;
		for (int i = 0; i < 2; i++) {
			sprites[imgIndex[i]]->Draw(pGraphics, x, 0, NUM_WIDTH, NUM_HEIGTH);
			x += 50;
		}
	}
}

