/*
*
* @file		Button.h
* @date     Fri May 11 13:59:33 2018
* @author
*/

#pragma once
#include "GameObject.h"
#include "dsSpriteManager.h"
#include "GAME_INFO.h"

enum BTN_STATE {
	ON,
	OFF
};

class Button : public GameObject
{
private:
	dsSprite * sprites[2];
	int state;

public:
	void SetData(dsSprite * onImage, dsSprite * offImage);
	void SetState(int _state);
	void DrawImg(Graphics * pGraphics);
};

