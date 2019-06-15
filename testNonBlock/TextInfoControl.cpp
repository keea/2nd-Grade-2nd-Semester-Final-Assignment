#include "TextInfoControl.h"

void TextInfoControl::Add(GameText text)
{
	texts[text.text] = text;
}


TextInfoControl::TextInfoControl()
{
	speed = 150.0f;
}

void TextInfoControl::ShowText(DWORD time, HDC hdc)
{
	for (TEXT_MAP::iterator it = texts.begin(); it != texts.end();) {

		SetTextColor(hdc, it->second.color); //글자색 변경.

		it->second.pos.y += speed * time /1000.0f;

		if (it->second.pos.y >= 1000)
		{
			texts.erase(it++); //it값이 erase()로 복사되어 넘어가고, it++가 실행되고, 마지막으로 erase()로 넘어간 복사된 it이 할 일을 마치고 무효화됩니다.
		}
		else 
		{
			TextOut(hdc, it->second.pos.x, it->second.pos.y, it->second.text, strlen(it->second.text));
			it++;
		}
	}
}

TYPE TextInfoControl::CheckText(string text)
{
	if (texts.size() <= 0)
		return ADD_TEXT;

	if (texts.find(text) == texts.end())
		return ADD_TEXT;
	else
		return SCORE_UP;
}
