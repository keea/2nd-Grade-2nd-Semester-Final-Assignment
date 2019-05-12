#include "AirObject.h"

void AirObject::Move(float x, float y, float deltaTime)
{
	m_xPos += (x * m_speed * deltaTime);
	m_yPos += (y * m_speed * deltaTime);
}

void AirObject::ChangeSpeed(float value)
{
	m_speed += value;
	if (m_speed < 0.0f)
		m_speed = 0.0f;
}

AirObject::~AirObject()
{
}
