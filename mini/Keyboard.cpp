#include <string>
#include "BEAN.h"
#include "Keyboard.h"

void Keyboard::Init(){
	for (int i = 0; i < 3; i++) {
		colRect[i] = { i * 138, 500,138,100 };
	}
	isAnswer = false;
	isClick = true; // 초기에는 클릭가능하게.
	isShowJud = false;
	step = 0;
}

void Keyboard::Draw(Graphics * g_BackBuffer)
{
	Gdiplus::Pen pen(H_TIMER_YELLOW);
	Gdiplus::Color c;

	if (isClick) {
		c = H_LIGTH_YELLOW;
	}
	else { //클릭한 경우 회색 처리.
		c = H_GRAY;
	}

	Gdiplus::SolidBrush brush(c);

	for (int i = 0; i < 3; i++) {
		g_BackBuffer->FillRectangle(&brush, colRect[i]);
		g_BackBuffer->DrawRectangle(&pen, colRect[i]);
	}

	//말풍선을 한번 그려보자.
	Rect ui(5, 35, 404, 100);
	Gdiplus::SolidBrush ui_brush(H_GRAY);
	g_BackBuffer->FillRectangle(&ui_brush, ui);

	Gdiplus::Point points[] = {Point(350,150), Point(404, 100), Point(300, 100)};
	g_BackBuffer->FillPolygon(&ui_brush, points, 3);


	//가운데 정렬 하는 법
	Font F(FONT_DEFAULT, 80, FontStyleRegular, UnitPixel);
	StringFormat SF;

	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);

	//한글자씩 정렬하고 정답을 맞출 경우 색상 변경해보자.
	for (int i = 0; i < 3; i++) {
		Color c;
		RectF R(colRect[i].X, 50, 138, 100);
		wstring str(1, quest[i]);
		if (i < step) {
			c = H_PINK;
		}
		else {
			c = H_DARK_YELLOW;
		}
		SolidBrush B(c);
		g_BackBuffer->DrawString(str.c_str(), -1, &F, R, &SF, &B);
	}
}

//키보드 안을 클릭했는지 반환해준다.
bool Keyboard::OnClick(POINT p, int answer)
{
	Rect r{ 0,500,414,100 };
	if (isClick) {
		if (colRect[answer].Contains(p.x, p.y)) {
			isAnswer = true;
			step += 1;
		}
		else
			isAnswer = false;

		if(r.Contains(p.x, p.y))
			isClick = false;
	}
	return r.Contains(p.x, p.y);
}

void Keyboard::SetQuest(wstring w)
{
	quest = w;
}

bool Keyboard::getIsAnswer()
{
	return isAnswer;
}

void Keyboard::setAbleClick(bool b)
{
	isClick = b;
}

bool Keyboard::getAbleClick() const
{
	return isClick;
}

int Keyboard::getStep()
{
	return step;
}

void Keyboard::ResetStep()
{
	step = 0;
}
