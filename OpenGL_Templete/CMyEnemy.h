#pragma once
#include "MyObject.h"
#include "CMyMath.h"
#include "CMyEnemyAttackPatten.h"


class CMyEnemy : public CMyObject
{
protected:
	DWORD					m_interval;
	dsTexture				*m_pHpSprite;
	RECT					m_rect;
	int						m_hp;
	int						m_damage;
	Vector2					m_vecPostion;
	CMyEnemyAttackPatten	*m_pAtkPatten[1];
	int						m_attackCount;
	int						m_BtwTimeGap;
	int						m_BtwTimeAttack;
	int						m_attackNum;
	CURRENT_TYPE			m_beforeAttackStatus;
	bool					m_isAttackUpdate;
	
	
public:
	CMyEnemy();
	~CMyEnemy();

	
	bool Create(char * filename);
	void OnDraw();
	void OnUpdate(DWORD tick);
	void HaveDamage(int damage); //데미지 입는 경우
	RECT GetEnemyRect() { return m_rect; }
	bool IsCollision(RECT rect);
	void Init();
};

