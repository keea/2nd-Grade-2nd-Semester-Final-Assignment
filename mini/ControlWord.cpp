#include "ControlWord.h"

wstring ControlWord::GetQuestNm()
{
	return words[quest];
}

void ControlWord::NextQuest()
{
	quest = rand() % size;
}


ControlWord::~ControlWord()
{
	delete [] words;
}
