#pragma once
#include "dsSpriteManager.h"
enum SCORE_TYPE
{
	T_SCORE,
	T_PERFECT,
	T_GREATE,
	T_COOL,
	T_MISS,
	T_COMBO,
	T_CNT
};

class ScoreManager
{
private:
	int scoreType[SCORE_TYPE::T_CNT];
	int combo;
public:
	void Init();
	void AddScore(int score_type);
	void resetCombo();
	void Draw(Graphics * backBuffer);
	int GetScore(int score_type);
};

