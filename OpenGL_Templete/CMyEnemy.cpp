#include "CMyEnemy.h"



CMyEnemy::CMyEnemy()
{
	m_pSprite = NULL;
	m_hp = 1024;
	m_vecPostion.x = 512 >> 1;
	m_vecPostion.y = 18;

	m_rect = { (long)m_vecPostion.x , (long)m_vecPostion.y,
		(long)m_vecPostion.x + 512, (long)m_vecPostion.y + 150 };
}


CMyEnemy::~CMyEnemy()
{
	delete m_pHpSprite;
	m_pHpSprite = NULL;
}

bool CMyEnemy::Create(char * filename)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	m_pSprite = new dsTexture(filename);
	m_pHpSprite = new dsTexture("hp_bar.png");
	m_pHpSprite->LoadFromFilename("hp_bar.png");

	return m_pSprite->LoadFromFilename(filename);
}

void CMyEnemy::OnDraw()
{
	m_pSprite->Draw(m_rect.left, m_rect.top, 0, 0, 512, 150, 0);
	m_pHpSprite->Draw(0, 0, 0, 0, m_hp, 15, 0);
}

void CMyEnemy::OnUpdate(DWORD tick)
{
	//적 공격 모션 추가하기.
	/// 1. 세로 방향에서 3초후 공격(왼쪽 공격후 오른쪽 공격)

}

void CMyEnemy::HaveDamage(int damage)
{
	m_hp -= damage;
}


