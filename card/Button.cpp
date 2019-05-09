/*
*
* @file		Button.cpp
* @date     Fri May 11 14:04:43 2018
* @author
*/

#include "Button.h"

void Button::SetData(dsSprite * onImage, dsSprite * offImage) {
	sprites[ON] = onImage;
	sprites[OFF] = offImage;
	state = OFF;
	SetX(512);
	SetY(384);
}

void Button::SetState(int _state) {
	state = _state;
}

void Button::DrawImg(Graphics * pGraphics) {
	sprites[state]->Draw(pGraphics, 512-(BTN_WIDTH/4), 384, BTN_WIDTH, BTN_HEIGHT);
}