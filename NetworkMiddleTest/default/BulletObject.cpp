#include "BulletObject.h"

void BulletObject::Move(float deltaTime)
{
	float yPos = 0.0f;
	if (isMyBullet) {
		yPos = -1.0f;
	}
	else {
		yPos = 1.0f;
	}
	m_yPos += (yPos * m_speed * (deltaTime / 1000.0f));
}

void BulletObject::SetIsMyBullet(bool isMy)
{
	isMyBullet = isMy;
}

std::string BulletObject::GetName()
{
	return name;
}

RECT BulletObject::GetRect()
{
	RECT rc;
	SetRect(&rc, m_xPos, m_yPos, m_xPos + m_rcImage.right, m_yPos + m_rcImage.bottom);
	return rc;
}


BulletObject::~BulletObject()
{
}
