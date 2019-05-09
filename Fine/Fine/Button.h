#pragma once
#include <string>

using namespace std;

class Button {
private:
	string caption_name;
	int id;
	bool isAnswer;

public:
	void Init(string caption_name, int id, bool isAnswer);
	string getName();
	int GetId();
	bool GetIsAnswer();
};

