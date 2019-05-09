#include <iostream>
#include "Node.h"
#include "BEAN.h"

void Node::Init()
{
	posY = 0;
	interval = 0;
	speed = 100;
	timer = 60;
	isClick = false; //클릭한지 안한지 체크
	judgment = JUDGMENT::NONE;
}

int Node::Update(Graphics * g_BackBuffer, DWORD tick, bool * isNewNode)
{
	//1초(1000)동안 100픽셀
	//100동안 10픽셀
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

	//judgment를 변경하지 않게 해야겠다.
	Color color;
	if (posY >= 90) { //퍼팩트일 경우
		color = H_PINK;
		if (isClick) {
			judgment = JUDGMENT::PERFECT;
		}

	}
	else if (posY >= 60) { //그레이트 경우
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
	//각 칸의 글자들 출력
	for (int i = 0; i < 3; i++) {
		RectF R(i * 138, 500, 138, 100);
		
		StringFormat SF;
		SF.SetAlignment(StringAlignmentCenter);
		//SF.SetLineAlignment(StringAlignmentCenter);
		SolidBrush B(color);
		g_BackBuffer->DrawString(words[i].c_str(), -1, &F, R, &SF, &B);
	}
	isClick = false;

	//시간 지났으면 결과창 띄우기
	if (timer <= 0)
		return SCENE::GAME_RESULT;

	//timer 출력
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
	//랜덤으로 값넣기
	words[0] = w[pos[0]];
	words[1] = w[pos[1]];
	words[2] = w[pos[2]];

	//해당 글자가 어느 박스에 들어있는지 확인
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
