#include "AirObject.h"


void AirObject::Move(float x, float y, float deltaTime)
{
	float posX = m_xPos;
	m_xPos += (x * m_speed * (deltaTime/1000.0f));

	if (m_xPos < 0 || m_xPos >(550 - m_dieAni[0].right))
		m_xPos = posX;

	m_yPos += (y * m_speed * (deltaTime/1000.0f));
}

void AirObject::ChangeSpeed(float value)
{
	m_speed += value;
	if (m_speed < 0.0f)
		m_speed = 0.0f;
}

void AirObject::Draw(dsTexture * pTexture, unsigned int tick)
{
	m_updateTick += tick;
	if (m_isDie) {
		if (m_updateTick > 15.0f)
			m_aniIndex = (m_aniIndex + 1) % 9;
		m_updateTick = 0.0f;
	}

	pTexture->SetPosition(m_xPos, m_yPos);
	pTexture->Draw(m_dieAni[m_aniIndex].left,
		m_dieAni[m_aniIndex].top,
		m_dieAni[m_aniIndex].right,
		m_dieAni[m_aniIndex].bottom, tick);
}

void AirObject::SetDie()
{
	m_isDie = true;
}

int AirObject::GetWidth()
{
	return  m_dieAni[m_aniIndex].right;
}

RECT AirObject::GetRect()
{
	RECT rc;
	SetRect(&rc, m_xPos, m_yPos, m_xPos + m_dieAni[0].right, m_yPos + m_dieAni[0].bottom);
	return rc;
}

AirObject::~AirObject()
{
}
