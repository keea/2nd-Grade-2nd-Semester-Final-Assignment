/*
*
* @file		ImageManager.cpp
* @date     Wed May  9 18:53:51 2018
* @author
*/

#include "ImageManager.h"

ImageManager::ImageManager(dsSpriteManager * _pSpriteManager)
{
	pSprtieManager = _pSpriteManager;
}

dsSpriteManager *ImageManager::GetSpriteManager() const{
	return pSprtieManager;
}
