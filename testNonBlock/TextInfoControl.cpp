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

	if (texts.size() <= 0) { //ó���̴ϱ� �����Ѵ�.
		
		Add(gameText);
		return ADD_TEXT;
	}

	if (texts.find(text) == texts.end()) { //���� ��� �߰�
		Add(gameText);
		return ADD_TEXT;
	}
	else { //���� ��� �ؽ��ʿ��� ����
		return SCORE_UP;
	}
		
}
