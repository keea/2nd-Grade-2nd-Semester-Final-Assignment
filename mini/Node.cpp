#include <iostream>
#include "Node.h"
#include "BEAN.h"

void Node::Init()
{
	posY = 0;
	interval = 0;
	speed = 100;
	timer = 60;
	isClick = false; //Ŭ������ ������ üũ
	judgment = JUDGMENT::NONE;
}

int Node::Update(Graphics * g_BackBuffer, DWORD tick, bool * isNewNode)
{
	//1��(1000)���� 100�ȼ�
	//100���� 10�ȼ�
	interval += tick;
	if (interval >= speed) {
		interval = interval-speed;
		posY += 10;
		if (posY > 100) {
			posY = 0;
			timer -= 1;
			*isNewNode = true;
			judgment = JUDGMENT::NONE;
		}
	}
	Font F(FONT_DEFAULT, posY-2, FontStyleRegular, UnitPixel);

	//judgment�� �������� �ʰ� �ؾ߰ڴ�.
	Color color;
	if (posY >= 90) { //����Ʈ�� ���
		color = H_PINK;
		if (isClick) {
			judgment = JUDGMENT::PERFECT;
		}

	}
	else if (posY >= 60) { //�׷���Ʈ ���
		color = H_RESULT_YELLOW;
		if (isClick) {
			judgment = JUDGMENT::GREAT;
		}

	}
	else { //miss
		color = H_DARK_YELLOW;
		if (isClick) {
			judgment = JUDGMENT::MISS;
		}
	}
	//�� ĭ�� ���ڵ� ���
	for (int i = 0; i < 3; i++) {
		RectF R(i * 138, 500, 138, 100);
		
		StringFormat SF;
		SF.SetAlignment(StringAlignmentCenter);
		//SF.SetLineAlignment(StringAlignmentCenter);
		SolidBrush B(color);
		g_BackBuffer->DrawString(words[i].c_str(), -1, &F, R, &SF, &B);
	}
	isClick = false;

	//�ð� �������� ���â ����
	if (timer <= 0)
		return SCENE::GAME_RESULT;

	//timer ���
	cout << timer << std::endl;
	for (int i = 0; i < timer; i++) {
		int random = (rand() % 8)+1;
		int height = 25;
		if ((i + 1) % random) {
			height = (rand() % 29) + 1;
		}
		Rect r = { ((i * 7)),500-height,7,height };
		Gdiplus::SolidBrush brush(H_TIMER_YELLOW);
		g_BackBuffer->FillRectangle(&brush, r);
	}
	return SCENE::SCENE_NONE;
}

int Node::getJud()
{
	return judgment;
}

bool Node::getIsClick()
{
	return isClick;
}

void Node::SetQuest(wstring w, int pos[])
{
	//�������� ���ֱ�
	words[0] = w[pos[0]];
	words[1] = w[pos[1]];
	words[2] = w[pos[2]];

	//�ش� ���ڰ� ��� �ڽ��� ����ִ��� Ȯ��
	sortNums[pos[0]] = 0;
	sortNums[pos[1]] = 1;
	sortNums[pos[2]] = 2;
}

void Node::setClick(bool _isClick)
{
	isClick = _isClick;
}

int Node::getSortBox(int i)
{
	return sortNums[i];
}
