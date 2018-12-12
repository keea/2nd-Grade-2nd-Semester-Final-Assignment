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

	//�Ѿ� �� �����ǳ� Ȯ��.
	keea::Debug("�Ѿ˰��� %d\r\n", m_bullets.size());
}

void CMyBullet::OnUpdate(DWORD tick)
{
	std::vector<int> destoryIndex; //�ı��� �Ѿ� �ε�����ȣ

	m_interval += tick;

	if (m_interval > 100)
	{
		m_interval = 0;
	}

	//�Ųٷ� ��ȯ�ؼ� ����ؼ� �߰��� ���� ���������� ��.
	//�տ��� ������ ��ȯ�ؼ� ����� ��� out of range ������ ���� ����.
	for (int i = m_bullets.size() - 1; i >= 0; i--) {
		CMyBulletObject * pBullet = m_bullets[i];
		pBullet->OnUpdate(tick);
		if (pBullet->isOutScreen()) {
			delete pBullet;
			//���������(ù��° ���ҿ��� destoryIndex[i]��ŭ ����� ���������)
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
