#pragma once
#include "MyObject.h"
#include "CMyMath.h"
class CMyHpObject
{
private:
	Vector2 m_vecPostion;
	int hp;
public:
	CMyHpObject(Vector2 vecPos);
	void Draw(dsTexture * pTexture);
	~CMyHpObject();
};

