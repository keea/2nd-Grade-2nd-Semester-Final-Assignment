#include "CMyBullet.h"
#include "CMyBulletObject.h"
#include "Debug.h"



CMyBullet::CMyBullet()
{
}


CMyBullet::~CMyBullet()
{
	for (int i = 0; i < m_bullets.size(); i++) {
		CMyBulletObject * pBullet = m_bullets[i];
		delete pBullet;
	}

	m_bullets.clear();
}

void CMyBullet::CreateBullet(Vector2 position, Vector2 dir, float angle)
{
	CMyBulletObject * pBulletObj = new CMyBulletObject(position, dir, angle);
	m_bullets.push_back(pBulletObj);
}

bool CMyBullet::Create(char * filename)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	m_pSprite = new dsTexture(filename);
	return m_pSprite->LoadFromFilename(filename);
}

void CMyBullet::OnDraw(int x, int y)
{
	for (int i = 0; i < m_bullets.size(); i++) {
		CMyBulletObject * pBullet = m_bullets[i];
		pBullet->Draw(m_pSprite);
	}

	//총알 잘 삭제되나 확인.
	keea::Debug("총알갯수 %d\r\n", m_bullets.size());
}

void CMyBullet::OnUpdate(DWORD tick)
{
	std::vector<int> destoryIndex; //파괴할 총알 인덱스번호

	m_interval += tick;

	if (m_interval > 100)
	{
		m_interval = 0;
	}

	//거꾸로 순환해서 사용해서 중간에 값을 빼버리도록 함.
	//앞에서 끝으로 순환해서 사용할 경우 out of range 오류가 나기 때문.
	for (int i = m_bullets.size() - 1; i >= 0; i--) {
		CMyBulletObject * pBullet = m_bullets[i];
		pBullet->OnUpdate(tick);
		if (pBullet->isOutScreen()) {
			delete pBullet;
			//원소지우기(첫번째 원소에서 destoryIndex[i]만큼 띄어진 원소지우기)
			m_bullets.erase(m_bullets.begin() + i);
		}
	}
}

int CMyBullet::GetDamage(RECT rect)
{
	int damage = 0;

	for (int i = m_bullets.size() - 1; i >= 0; i--) {
		CMyBulletObject * pBullet = m_bullets[i];
		if (pBullet->isAttack(rect)) {
			delete pBullet;
			m_bullets.erase(m_bullets.begin() + i);
			damage += 1;
		}
	}

	return damage;
}
