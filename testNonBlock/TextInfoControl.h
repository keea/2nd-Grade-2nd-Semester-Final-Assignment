#pragma once
#include <hash_map>
#include "GameText.h"

using namespace std;
#define TEXT_MAP std::hash_map< char[20], GameText> 

enum TYPE {
	SCORE_UP,
	ADD_TEXT,
};

class TextInfoControl
{
private:
	TEXT_MAP texts;
	void Add(GameText gameText);
public:
	TYPE ControlText(char Text[20], DWORD userId);
};

