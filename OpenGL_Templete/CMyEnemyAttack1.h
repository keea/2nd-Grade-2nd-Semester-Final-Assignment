#pragma once
#include "CMyEnemyAttackPatten.h"
#include "dsTexture.h"
#include "CMyMath.h"

class CMyEnemyAttack1 :
	public CMyEnemyAttackPatten
{
protected:
	DWORD			m_interval;
	dsTexture		*m_pSprite_guideLine;
	dsTexture		*m_pSprite_attack;
	DWORD			m_TimeBtwSpawn;
	int				m_TimeBtwStart;
	
	bool			m_guideLineOnOff;
	int				m_angle;

	Vector2			m_vecPostion;

public:
	CMyEnemyAttack1();
	~CMyEnemyAttack1();

	bool Create();

	CURRENT_TYPE OnUpdate(DWORD tick);
	void OnDraw();
	bool IsCollision(RECT rect);
	void OnStart();
};

