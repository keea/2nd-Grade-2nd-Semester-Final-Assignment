#pragma once
#include <string>
using namespace std;

class FileIo {
public:
	static wstring * InputFile(int * size);
};

class MixManager {
private:
	int pos[6][3];
	int randNum;
public:
	MixManager();
	void setRandNum();
	int * getPos();
};
