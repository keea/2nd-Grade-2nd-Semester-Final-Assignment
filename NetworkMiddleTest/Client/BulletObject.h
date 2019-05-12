#pragma once
#include "GameObject.h"

class BulletObject : public GameObject
{
public:
	BulletObject(std::string name, int srcX, int srcY, int srcW, int srcH, TYPE type)
		:GameObject(name, srcX, srcY, srcW, srcH, type) {
		m_speed = 100.0f;
	};

	void Move(float deltaTime);
	void SetIsMyBullet(bool isMy);
	std::string GetName();

	RECT GetRect();

	~BulletObject();

private:
	float		m_speed;
	bool		isMyBullet;
};

