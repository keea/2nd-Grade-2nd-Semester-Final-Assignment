#pragma once
#include "MyObject.h"
#include "CMyMath.h"

class CMyBulletObject
{
private:
	Vector2 m_vecDirection;
	Vector2 m_vecPosition;
	RECT bulletSizeRect;
	float m_angle;
public:
	CMyBulletObject(Vector2 vecPos, Vector2 vecDir, float angle);

	void OnUpdate(DWORD tick);
	void Draw(dsTexture * pTexture);

	bool isOutScreen();
	bool isAttack(RECT rect);

	~CMyBulletObject();
};

