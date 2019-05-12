#pragma once

#include "dsTexture.h"
#include <string>

enum TYPE
{
	NONE,
	AIR, //∫Ò«‡±‚
};

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, int srcX, int srcY,
		int srcW, int srcH, TYPE type);
	~GameObject();

	void GetPosition(float *x, float *y);
	void SetPosition(float x, float y);
	void Draw(dsTexture * pTexture, unsigned int tick);
private:
	std::string name;
	RECT m_rcImage;
protected:
	float m_xPos;
	float m_yPos;
	TYPE m_type = NONE;
};

