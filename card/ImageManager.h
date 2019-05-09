/*
*
* @file		ImageManager.h
* @date     Wed May  9 18:50:48 2018
* @author
*/

#pragma once
#include "dsSpriteManager.h"

class ImageManager
{
private:
	dsSpriteManager * pSprtieManager;
public:
	ImageManager(dsSpriteManager * _pSpriteManager);
	dsSpriteManager * GetSpriteManager() const;
	virtual void InitImage() = 0;
	virtual dsSprite * getImage(int idx) = 0;
	virtual dsSprite * GetCreateImage(const char * name) = 0;
	virtual dsSprite ** GetImages() = 0;
};
