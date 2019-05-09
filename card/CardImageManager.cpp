/*
*
* @file		CardImageManager.cpp
* @date     Wed May  9 19:11:35 2018
* @author
*/

#include "CardImageManager.h"

void CardImageManager::InitImage() {
	char name[128];
	for (int i = 0; i < CARD_IMAGE_NUM; i++) {
		wsprintf(name, "f%03d.png", i+1);
		images[i] = GetSpriteManager()->Create(name, name);
	}
}

dsSprite * CardImageManager::getImage(int idx) {
	return images[idx];
}

dsSprite * CardImageManager::GetCreateImage(const char * name) {
	return GetSpriteManager()->Create(name, name);
}

dsSprite ** CardImageManager::GetImages() {
	return images;
}