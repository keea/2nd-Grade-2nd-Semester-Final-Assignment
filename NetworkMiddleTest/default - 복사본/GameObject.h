#pragma once

#include "dsTexture.h"
#include <string>

enum TYPE
{
	NONE,
	AIR, //ºñÇà±â
	BULLET, //ÃÑ¾Ë
};

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, int srcX, int srcY,
		int srcW, int srcH, TYPE type);
	virtual ~GameObject();

	void GetPosition(float *x, float *y);
	void SetPosition(float x, float y);
	virtual void Draw(dsTexture * pTexture, unsigned int tick);
private:
	
protected:
	float m_xPos;
	float m_yPos;
	TYPE m_type = NONE;
	std::string name;
	RECT m_rcImage;
};

