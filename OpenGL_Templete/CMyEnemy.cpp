#include "CMyEnemy.h"
#include "CMyEnemyAttack1.h"


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
	
	delete m_pAtkPatten[0];

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

	CMyEnemyAttack1 * atk1 = new CMyEnemyAttack1();
	atk1->Create();

	m_attackCount = 1;

	m_pAtkPatten[0] = atk1;

	return m_pSprite->LoadFromFilename(filename);
}

void CMyEnemy::OnDraw()
{
	m_pSprite->Draw(m_rect.left, m_rect.top, 0, 0, 512, 150, 0);
	m_pHpSprite->Draw(0, 0, 0, 0, m_hp, 15, 0);
	if(m_isAttackUpdate)
		m_pAtkPatten[m_attackNum]->OnDraw();
}

void CMyEnemy::OnUpdate(DWORD tick)
{

	if (m_beforeAttackStatus == END) {
		m_isAttackUpdate = false;
		if (m_BtwTimeGap >= 2000)
			m_BtwTimeGap -= 500;

		m_BtwTimeAttack = m_BtwTimeGap;
		m_beforeAttackStatus = NOTHING;
	}

	if (m_beforeAttackStatus == NOTHING) {
		if (m_BtwTimeAttack <= 0) {
			m_attackNum = rand() % m_attackCount;
			m_isAttackUpdate = true;
			m_pAtkPatten[m_attackNum]->OnStart();
		}
		else {
			m_BtwTimeAttack -= tick;
		}
	}

	if(m_isAttackUpdate)
		m_beforeAttackStatus = m_pAtkPatten[m_attackNum]->OnUpdate(tick);
}

void CMyEnemy::HaveDamage(int damage)
{
	m_hp -= damage;
}

bool CMyEnemy::IsCollision(RECT rect)
{
	return m_pAtkPatten[0]->IsCollision(rect);
}

void CMyEnemy::Init()
{
	m_BtwTimeAttack = 1000;
	m_attackNum = 0;
	m_BtwTimeGap = 3000;
	m_beforeAttackStatus = NOTHING;
	m_isAttackUpdate = false;
}


