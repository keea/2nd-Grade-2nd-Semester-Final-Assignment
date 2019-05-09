/*
*
* @file		GameTimeControl.h
* @date     Fri May 11 10:35:37 2018
* @author
*/

#pragma once
#include "ImageManager.h"
#include "GAME_INFO.h"

class GameTimeControl
{
private:
	DWORD timer;
	DWORD interval;
	int isNotEnd;

public:
	void Init();
	void TimerUpdate(DWORD tick, bool * currentScene);
	void DisplayTimer(Graphics * pGraphics, dsSprite ** sprites);
};

