#pragma once
#include "MyObject.h"
#include "CMyMath.h"
#include <vector>

class CMyBulletObject;

class CMyBullet : public CMyObject
{
protected:
	DWORD m_interval;
	typedef std::vector<CMyBulletObject *> BULLET;
	
	BULLET m_bullets;

public:
	CMyBullet();
	~CMyBullet();

	void CreateBullet(Vector2 position, Vector2 dir, float angle);
	bool Create(char *filename);
	void OnDraw(int x, int y);
	void OnUpdate(DWORD tick);
	int GetDamage(RECT rect);
};

