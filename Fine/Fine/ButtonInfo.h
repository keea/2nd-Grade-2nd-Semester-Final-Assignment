#pragma once
#include "Button.h"
class ButtonInfo
{
private:
	Button * buttons;
public:
	ButtonInfo();
	Button * GetBtn(int i);
	void Init();
	~ButtonInfo();
};

