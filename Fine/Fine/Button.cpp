#include "stdafx.h"
#include "Button.h"
#include <iostream>

void Button::Init(string caption_name, int id, bool isAnswer)
{
	this->caption_name = caption_name;
	this->id = id;
	this->isAnswer = isAnswer;
}

string Button::getName()
{

	return caption_name;
}

int Button::GetId()
{
	return id;
}

bool Button::GetIsAnswer()
{
	return isAnswer;
}
