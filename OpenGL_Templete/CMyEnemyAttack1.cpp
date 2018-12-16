#include "CMyEnemyAttack1.h"
#include "Debug.h"

CMyEnemyAttack1::CMyEnemyAttack1()
{
	m_pSprite_guideLine = NULL;
	m_pSprite_attack = NULL;
	m_TimeBtwStart = 1000;
	m_currentType = START;
	m_angle = 0;
}


CMyEnemyAttack1::~CMyEnemyAttack1()
{
	delete m_pSprite_guideLine;
	m_pSprite_guideLine = NULL;
}

bool CMyEnemyAttack1::Create()
{
	if (m_pSprite_guideLine != NULL)
		delete m_pSprite_guideLine;
	if (m_pSprite_attack != NULL)
		delete m_pSprite_attack;

	m_pSprite_guideLine = new dsTexture("guideline1.png");
	m_pSprite_attack = new dsTexture("attack1.png");
	bool result = m_pSprite_guideLine->LoadFromFilename("guideline1.png")
		&& m_pSprite_attack->LoadFromFilename("attack1.png");
	
	return result;
}

CURRENT_TYPE CMyEnemyAttack1::OnUpdate(DWORD tick)
{
	if (m_currentType == START) {
		if (m_TimeBtwStart <= 0) {
			m_currentType = DOING;
			m_vecPostion.x = 448;
			m_vecPostion.y = 180;
		}
		else {
			m_guideLineOnOff = (m_TimeBtwStart / 100) % 2 == 0;
			m_TimeBtwStart -= tick;
		}
	}
	else if (m_currentType == DOING) {
		m_angle = (m_angle + (1 * tick/2));
		m_angle %= 360;
		m_vecPostion.y += 1 * 500.0f*tick / 1000;
		if (m_vecPostion.y >= 768)
			m_currentType = END;
	}

	return m_currentType;
}

void CMyEnemyAttack1::OnDraw()
{
	if (m_currentType == START) //스타트 시간일 때.
	{
		if(m_guideLineOnOff)
			m_pSprite_guideLine->Draw(462, 180, 0);
	}
	else if (m_currentType == DOING) {
		m_pSprite_attack->DrawRotation(m_vecPostion.x, m_vecPostion.y, 
			m_angle);
	}
}

bool CMyEnemyAttack1::IsCollision(RECT rect)
{
	if(m_currentType != DOING)
		return false;

	RECT attackRect = { m_vecPostion.x, m_vecPostion.y,
	m_vecPostion.x + 128, m_vecPostion.y + 128 };

	RECT temp;
	return (IntersectRect(&temp, &rect, &attackRect));
}

void CMyEnemyAttack1::OnStart()
{
	m_currentType = START;
	m_TimeBtwStart = 1000;
}
