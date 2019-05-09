#include "stdafx.h"
#include "ButtonInfo.h"
#include <iostream>
#include <string>

using namespace std;


ButtonInfo::ButtonInfo()
{
	buttons = new Button[20];
}

Button* ButtonInfo::GetBtn(int i)
{

	return &buttons[i];
}

void ButtonInfo::Init() {
	string question[] = { "��", "��" ,"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "ȥ", "I", "N" , "��", "��","��","ž","��","�E","��","��","��",";","��","��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "l", "��", "��", "��", "S", "R", "��", "��", "��", "��", "��", "��", "��", "Ȩ", "Ȩ", "��","��","F","��","��","��","��","ã","��","��","�i","��","��","��","��","��","�r","��" };
	string answer[] = { "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "J", "M", "��", "��","��","Ž","��","��","��","��","��",":","��","��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "I", "ö", "��", "��", "C", "K", "��", "��", "��", "��", "ȸ", "��", "��", "�L", "��", "�v","��","E","��","��","��","��","�O","��","��","�e","��","��","��","½","�V","�s","��" };

	int probleRand = rand() % (sizeof(question)/sizeof(string));
	int randNum = rand() % 20;

	for (int i = 0; i < 20; i++) {
		if (i == randNum)
			buttons[i].Init(answer[probleRand], i, true);
		else
			buttons[i].Init(question[probleRand], i, false);
	}
}

ButtonInfo::~ButtonInfo()
{
	delete[] buttons;
}


