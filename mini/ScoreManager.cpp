#include "ScoreManager.h"
#include <string>
#include "BEAN.h"

using namespace std;
void ScoreManager::Init()
{
	combo = 0;
	for (int i = 0; i < SCORE_TYPE::T_CNT; i++) {
		scoreType[i] = 0;
	}
}

void ScoreManager::AddScore(int score_type)
{
	scoreType[score_type] += 1;
	int combo_score = (scoreType[SCORE_TYPE::T_COMBO]/5);
	switch (score_type) {
	case SCORE_TYPE::T_COOL:
		combo += 1;
		scoreType[SCORE_TYPE::T_SCORE] += (25+ combo_score);
		break;
	case SCORE_TYPE::T_GREATE:
		combo += 1;
		scoreType[SCORE_TYPE::T_SCORE] += (50+ combo_score);
		break;
	case SCORE_TYPE::T_PERFECT:
		scoreType[SCORE_TYPE::T_SCORE] += (100+ combo_score);
		break;
	}

	if (combo >= scoreType[SCORE_TYPE::T_COMBO])
		scoreType[SCORE_TYPE::T_COMBO] = combo;
}

void ScoreManager::resetCombo()
{
	combo = 0;
}

void ScoreManager::Draw(Graphics * backBuffer)
{
	Font F(FONT_CUTE, 25, FontStyleRegular, UnitPixel);
	RectF R(0, 700, 414, 100);
	SolidBrush B(H_PINK);
	StringFormat SF;
	SF.SetAlignment(StringAlignmentFar); //오른쪽 정렬
	SF.SetLineAlignment(StringAlignmentCenter);
	
	wstring s = L"Score : ";
	s = s + to_wstring(scoreType[T_SCORE]);
	backBuffer->DrawString(s.c_str(), -1, &F, R, &SF, &B);
}

int ScoreManager::GetScore(int type)
{
	return scoreType[type];
}
