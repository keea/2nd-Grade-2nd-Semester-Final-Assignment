#include "TextInfoControl.h"

void TextInfoControl::Add(GameText gameText)
{
	texts[gameText.text] = gameText;
}

TYPE TextInfoControl::ControlText(string text, DWORD userId)
{
	GameText gameText;
	strcpy(gameText.text, text.c_str());
	gameText.userID = userId;

	if (texts.size() <= 0) { //처음이니까 저장한다.
		
		Add(gameText);
		return ADD_TEXT;
	}

	if (texts.find(text) == texts.end()) { //없을 경우 추가
		Add(gameText);
		return ADD_TEXT;
	}
	else { //있을 경우 해쉬맵에서 제거
		return SCORE_UP;
	}
		
}
