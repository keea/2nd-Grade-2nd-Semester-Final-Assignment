/*
*
* @file		CardImageManager.h
* @date     Wed May  9 19:06:46 2018
* @author
*/

#pragma once
#include "ImageManager.h"
#include "GAME_INFO.h"
class CardImageManager : public ImageManager
{
private:
	dsSprite * images[CARD_IMAGE_NUM];
public:
	CardImageManager(dsSpriteManager * _pSpriteManager) :ImageManager(_pSpriteManager) {};
	virtual void InitImage();
	virtual dsSprite * getImage(int idx);
	dsSprite * GetCreateImage(const char * name);
	dsSprite ** GetImages();
};

