#pragma once
#include "GameObject.h"
class AirObject : public GameObject
{
public:
	AirObject(std::string name, int srcX, int srcY, int srcW, int srcH, TYPE type)
		:GameObject(name, srcX, srcY, srcW, srcH, type) {};

	void Move(float x, float y, float deltaTime);

	void ChangeSpeed(float value);

	~AirObject();

private:
	float        m_speed;
};

