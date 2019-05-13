#pragma once
#include "GameObject.h"
#include "CMyMath.h"

class BulletObject : public GameObject
{
public:
	BulletObject(std::string name, int srcX, int srcY, int srcW, int srcH, TYPE type)
		:GameObject(name, srcX, srcY, srcW, srcH, type) {
		m_speed = 100.0f;
		isFire = false;
	};

	void Move(float deltaTime);
	void SetIsMyBullet(bool isMy);
	void SetVecDir(Vector2 dir);
	std::string GetName();

	RECT GetRect();

	~BulletObject();

private:
	float		m_speed;
	bool		isMyBullet;
	Vector2		m_vecDirection; //ÃÑ¾Ë ¹æÇâ
	bool		isFire;
};

