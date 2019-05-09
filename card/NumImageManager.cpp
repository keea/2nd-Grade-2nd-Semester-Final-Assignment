/*
*
* @file		NumImageManager.cpp
* @date     Fri May 11 10:19:53 2018
* @author
*/

#include "NumImageManager.h"

void NumImageManager::InitImage() {
	char name[128];
	for (int i = 0; i < NUM_IMAGE_NUM; i++) {
		wsprintf(name, "s_%02d.png", i);
		images[i] = GetSpriteManager()->Create(name, name);
	}
}

dsSprite * NumImageManager::getImage(int idx) {
	return images[idx];
}

dsSprite * NumImageManager::GetCreateImage(const char * name) {
	return GetSpriteManager()->Create(name, name);
}

dsSprite ** NumImageManager::GetImages() {
	return images;
}