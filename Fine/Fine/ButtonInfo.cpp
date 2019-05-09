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
	string question[] = { "¾Æ", "¾î" ,"¿À", "¿ì", "³ª", "³É", "¹°", "¼Ò", "±Í", "½º", "µ¹", "½É", "³Ê", "¸»", "È¥", "I", "N" , "µ·", "¿Í","»í","Å¾","µ·","ÀE","³°","Âü","¾ß",";","¾Ö","Äá", "µÎ", "µª", "¸ô", "½½", "Èú", "°­", "°ø", "ÀÇ", "”Ô", "¹Ù", "¿ì", "¶¥", "°ø", "l", "Àü", "³×", "¼±", "S", "R", "´«", "¼Ö", "»ê", "¼Ö", "Èñ", "°ú", "ºø", "È¨", "È¨", "½±","»ó","F","ÇÓ","ÀÇ","¹Ì","¿­","Ã£","¾Î","¹ä","«i","·Ô","Çì","¼ª","››","ÈÛ","µr","¼•" };
	string answer[] = { "¾î", "¾Æ", "¿ì", "¿À", "³Ê", "¸Û", "ºÒ", "¼ö", "±â", "¾²", "µ¶", "¼À", "³ª", "¹ß", "µ·", "J", "M", "Åæ", "¿Ü","»î","Å½","µ¸","¾–","…þ","Âý","Çá",":","¿¡","°ø", "ºÎ", "ÅÜ", "Æú", "ºñ", "ÀÏ", "°Á", "‚‹", "À§", "’„", "ÆÄ", "ÁÖ", "ÅÁ", "Äá", "I", "Ã¶", "³»", "¼Ç", "C", "K", "´¯", "¼Õ", "°­", "¼ñ", "È¸", "ÀÚ", "ºû", "ÄL", "Èì", "šv","Àå","E","ÀŸ","¿Ü","ºñ","ÀÏ","ªO","¾Í","¹ý","ªe","¸©","ÇØ","¼Ð","Â½","ÅV","µs","¼–" };

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


