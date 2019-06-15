#pragma once
#include <hash_map>
#include <string>
#include "packet.h"
#include "GameText.h"

using namespace std;
#define TEXT_MAP std::hash_map<std::string, GameText> 

enum TYPE {
	SCORE_UP,
	ADD_TEXT,
};

class TextInfoControl
{
private:
	TEXT_MAP texts;
	float speed;
	DWORD tick;
public:
	TextInfoControl();
	void ShowText(DWORD tick, HDC hdc);
	TYPE CheckText(string text);
	void Add(GameText gameText);
};

