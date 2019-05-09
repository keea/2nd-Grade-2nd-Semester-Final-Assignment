/*
*
* @file		
* @date     Fri May 11 15:47:50 2018
* @author
*/
#pragma once
#include "ImageManager.h"
#include "GAME_INFO.h"
class ScoreControl
{
private:
	int score;
	int combo;
public:
	void Init();
	void AddScore();
	void AddCombo();
	void ClearCombo();
	void DisplayScore(Graphics * pGraphics, dsSprite ** sprites, dsSprite * combo);
};

