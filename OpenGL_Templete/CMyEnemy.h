#pragma once
#include "MyObject.h"
#include "CMyMath.h"

class CMyEnemy : public CMyObject
{
protected:
	DWORD	m_interval;
	dsTexture		*m_pHpSprite;
	RECT m_rect;
	int m_hp;
	int m_damage;
	Vector2 m_vecPostion;
	
public:
	CMyEnemy();
	~CMyEnemy();

	
	bool Create(char * filename);
	void OnDraw();
	void OnUpdate(DWORD tick);
	void HaveDamage(int damage); //데미지 입는 경우
	RECT GetEnemyRect() { return m_rect; }
};

