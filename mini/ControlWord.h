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
	wstring GetQuestNm(); //������ϴ� ����.
	void NextQuest(); //���� ����Ʈ�� �ѱ��.
	~ControlWord();
};

