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
	DWORD userId;
public:
	TextInfoControl();
	void SetMyId(DWORD id);
	void ShowText(DWORD tick, HDC hdc, int * dieNum);
	TYPE CheckText(string text);
	void Add(GameText gameText);
	void DelText(string text);
	~TextInfoControl();
};

