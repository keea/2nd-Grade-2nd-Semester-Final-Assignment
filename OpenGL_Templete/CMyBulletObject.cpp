#include "CMyBulletObject.h"

CMyBulletObject::CMyBulletObject(Vector2 vecPos, Vector2 vecDir, float angle)
	:m_vecDirection(vecDir), m_vecPosition(vecPos), m_angle(angle)
{
	bulletSizeRect = { (LONG)m_vecPosition.x, (LONG)m_vecPosition.y,
		(LONG)m_vecPosition.x + 20, (LONG)m_vecPosition.y + 30 };
}

void CMyBulletObject::OnUpdate(DWORD tick)
{
	//������ ũ�⸦ 1�� ���� ����Ѵ�.
	m_vecPosition.x += (Vector_Normalize(m_vecDirection).x * 200.0f * tick / 1000.0f);
	m_vecPosition.y += (Vector_Normalize(m_vecDirection).y * 200.0f * tick / 1000.0f);

	bulletSizeRect = { (LONG)m_vecPosition.x, (LONG)m_vecPosition.y,
		(LONG)m_vecPosition.x + 20, (LONG)m_vecPosition.y + 30 };
}

void CMyBulletObject::Draw(dsTexture * pTexture)
{
	pTexture->DrawRotation(m_vecPosition.x, m_vecPosition.y, m_angle);
}

bool CMyBulletObject::isOutScreen()
{
	RECT ScreenRect = { 0,0,1024,768 }; //ȭ�� ũ��

	//������ ���� ȭ�� �ȿ� �ִ°�?
	if (ScreenRect.left <= bulletSizeRect.left && ScreenRect.right >= bulletSizeRect.right
		&& ScreenRect.top <= bulletSizeRect.top && ScreenRect.bottom >= bulletSizeRect.bottom) {
		return false;
	}

	else {
		return true;
	}
}

bool CMyBulletObject::isAttack(RECT rect)
{
	RECT temp;
	if (IntersectRect(&temp, &rect, &bulletSizeRect)) {
		return true;
	}
	return false;
}

CMyBulletObject::~CMyBulletObject()
{

}
