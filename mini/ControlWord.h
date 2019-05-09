#pragma once
#include <string>
using namespace std;
class ControlWord
{
private:
	wstring* words;
	int size;
	int quest;
public:
	ControlWord(wstring * words, int size) :words(words), size(size), quest(0){};
	wstring GetQuestNm(); //맞춰야하는 문자.
	void NextQuest(); //다음 퀘스트로 넘기기.
	~ControlWord();
};

