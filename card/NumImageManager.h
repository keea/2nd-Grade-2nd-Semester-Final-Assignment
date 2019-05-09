/*
*
* @file		NumImageManager.h
* @date     Fri May 11 10:16:31 2018
* @author
*/
#pragma once
#include "ImageManager.h"
#include "GAME_INFO.h"
class NumImageManager : public ImageManager
{
private:
	dsSprite * images[NUM_IMAGE_NUM];
public:
	NumImageManager(dsSpriteManager * _pSpriteManager) :ImageManager(_pSpriteManager) {};
	virtual void InitImage();
	virtual dsSprite * getImage(int idx);
	dsSprite * GetCreateImage(const char * name);
	dsSprite ** GetImages();
};

