#include "TextInfoControl.h"

void TextInfoControl::Add(GameText text)
{
	texts[text.text] = text;
}

void TextInfoControl::DelText(string text)
{
	texts.erase(text); //����
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

		SetTextColor(hdc, it->second.color); //���ڻ� ����.

		it->second.pos.y += speed * time /1000.0f;

		if (it->second.pos.y >= 800)
		{
			//������ ��ŭ ���� ���.
			if (userId != it->second.userID) //�ڽ��� ģ�� �ƴ϶��
				(*dieNum) += 1; //� �׾����� üũ
			texts.erase(it++); //it���� erase()�� ����Ǿ� �Ѿ��, it++�� ����ǰ�, ���������� erase()�� �Ѿ ����� it�� �� ���� ��ġ�� ��ȿȭ�˴ϴ�.
		}
		else 
		{
			SetTextAlign(hdc, TA_LEFT);//���� ����
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
