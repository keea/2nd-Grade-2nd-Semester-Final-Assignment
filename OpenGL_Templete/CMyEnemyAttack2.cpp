#include "CMyEnemyAttack2.h"
#include "Debug.h"



CMyEnemyAttack2::CMyEnemyAttack2()
{
	for (int i = 0; i < 4; i++) {
		m_pSprites[i] = NULL;
	}

	m_TimeBtwStart = 1500;
	m_currentType = START;
}


CMyEnemyAttack2::~CMyEnemyAttack2()
{
	for (int i = 3; i >= 0; i--) {
		delete m_pSprites[i];
		m_pSprites[i] = NULL;
	}
}

bool CMyEnemyAttack2::Create()
{
	bool result = true;
	for (int i = 0; i < 4; i++) {
		if (m_pSprites[i] != NULL)
			delete m_pSprites[i];
		
		char s1[126];
		sprintf(s1, "enemyPatten02_%d.png", i);

		m_pSprites[i] = new dsTexture(s1);
		result = m_pSprites[i]->LoadFromFilename(s1);
	}

	return result;
}

CURRENT_TYPE CMyEnemyAttack2::OnUpdate(DWORD tick)
{
	if (m_currentType == START) {
		if (m_TimeBtwStart >= 1500) {
			m_imageIndex = 3;
			m_currentType = DOING;
		}
		else {
			m_TimeBtwStart += tick;
			if ((m_imageIndex + 1) * 500 <= m_TimeBtwStart) {
				keea::Debug("인덱스와 시간 %d - %d\r\n", m_imageIndex, m_TimeBtwStart);
				m_imageIndex += 1;
			}
		}
	}
	else if (m_currentType == DOING) {
		if (m_TimeBtwSpan <= 0) {
			m_currentType = END;
		}
		else {
			m_TimeBtwSpan -= tick;
		}
	}
	return m_currentType;
}

void CMyEnemyAttack2::OnDraw()
{
	if (m_imageIndex >= 4)
		m_imageIndex = 3;

	keea::Debug("테스트 : %d\r\n", m_imageIndex);
	m_pSprites[m_imageIndex]->Draw(m_vecPostion.x, m_vecPostion.y, 0);
}

bool CMyEnemyAttack2::IsCollision(RECT rect)
{
	if (m_currentType != DOING)
		return false;

	RECT attackRect = { m_vecPostion.x, m_vecPostion.y,
	m_vecPostion.x + 512, m_vecPostion.y + 512 };

	RECT temp;

	return (IntersectRect(&temp, &rect, &attackRect));
}

void CMyEnemyAttack2::OnStart()
{
	m_currentType = START;
	m_TimeBtwSpan = 1000;
	m_TimeBtwStart = 0;
	m_imageIndex = 0;
	int randNum = rand() % 2;
	m_vecPostion = { (float)((randNum * 512)), 200.0f };
}
