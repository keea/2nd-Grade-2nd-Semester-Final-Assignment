#include "TextInfoControl.h"

void TextInfoControl::Add(GameText text)
{
	texts[text.text] = text;
}

void TextInfoControl::DelText(string text)
{
	texts.erase(text); //삭제
}

TextInfoControl::~TextInfoControl()
{
	texts.clear();
}


TextInfoControl::TextInfoControl()
{
	speed = 150.0f;
}

void TextInfoControl::SetMyId(DWORD id)
{
	userId = id;
}

void TextInfoControl::ShowText(DWORD time, HDC hdc, int * dieNum)
{
	for (TEXT_MAP::iterator it = texts.begin(); it != texts.end();) {

		SetTextColor(hdc, it->second.color); //글자색 변경.

		it->second.pos.y += speed * time /1000.0f;

		if (it->second.pos.y >= 800)
		{
			//삭제된 만큼 점수 깍기.
			if (userId != it->second.userID) //자신이 친게 아니라면
				(*dieNum) += 1; //몇개 죽었는지 체크
			texts.erase(it++); //it값이 erase()로 복사되어 넘어가고, it++가 실행되고, 마지막으로 erase()로 넘어간 복사된 it이 할 일을 마치고 무효화됩니다.
		}
		else 
		{
			SetTextAlign(hdc, TA_LEFT);//왼쪽 정렬
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
