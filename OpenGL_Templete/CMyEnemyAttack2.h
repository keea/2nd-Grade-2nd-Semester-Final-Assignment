#pragma once
#include "CMyEnemyAttackPatten.h"
#include "dsTexture.h"
#include "CMyMath.h"

class CMyEnemyAttack2 :
	public CMyEnemyAttackPatten
{
protected:
	DWORD		m_interval;
	dsTexture	*m_pSprites[4];
	DWORD		m_TimeBtwSpawn;
	int			m_TimeBtwStart;

	Vector2		m_vecPostion;
	int			m_imageIndex;
	int			m_TimeBtwSpan;
public:
	CMyEnemyAttack2();
	~CMyEnemyAttack2();

	bool Create();
	CURRENT_TYPE OnUpdate(DWORD tick);
	void OnDraw();
	bool IsCollision(RECT rect);
	void OnStart();
};

