#include "Utility.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "dsSpriteManager.h"

using namespace std;
wstring * FileIo::InputFile(int * _size)
{
	wifstream in("quest.txt");
	wstring * wstrings = nullptr;
	in.imbue(locale("kor")); //한글 출력 문제 해결.
	if (in.is_open()) {
		wstring str;
		getline(in, str);
		int size = stoi(str);
		*_size = size;
		wstrings = new wstring[size];
		cout << size << endl;
		for (int i = 0; i < size; i++) {
			wstring word;
			getline(in, word);
			wstrings[i] = word;
		}
	}
	return wstrings;
}

MixManager::MixManager()
{
	FILE *fp = fopen("number.txt", "rt");
	if (fp) {
		for (int i = 0; i < 6; i++) {
			int n1, n2, n3;
			fscanf(fp, "%d %d %d", &pos[i][0], &pos[i][1], &pos[i][2]);
		}
	}
	randNum = rand() % 6;
}

void MixManager::setRandNum()
{
	randNum = rand() % 6;
}

int * MixManager::getPos()
{
	return pos[randNum];
}
