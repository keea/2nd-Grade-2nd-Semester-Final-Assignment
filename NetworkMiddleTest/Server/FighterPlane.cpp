#include "FighterPlane.h"


CFighterPlane::CFighterPlane(string name)
{
	m_pFighter = new dsTexture(name);
	SetRect(&m_rcImage, 0, 0, 0, 0);
	m_xPos = m_yPos = 0.0f;
	m_speed = 300.0f;
}

CFighterPlane::CFighterPlane()
{
	m_pFighter = NULL;
	SetRect(&m_rcImage, 0, 0, 0, 0);
	m_xPos = m_yPos = 0.0f;
	m_speed = 300.0f;
}


CFighterPlane::~CFighterPlane()
{
	delete m_pFighter; m_pFighter = NULL;
}

BOOL CFighterPlane::Create(string imagename)
{
	if (m_pFighter == NULL)
		return FALSE;

	if (!m_pFighter->LoadFromFilename(imagename.c_str()))
		return FALSE;

	return TRUE;
}

BOOL CFighterPlane::Create(string imagename, 
	                       int srcX, int srcY, 
	                       int srcW, int srcH)
{
	if (!Create(imagename))
		return FALSE;

	SetRect(&m_rcImage, srcX, srcY, srcW, srcH);
}

void CFighterPlane::GetPosition(float &x, float &y)
{
	x = m_xPos;
	y = m_yPos;
}

void CFighterPlane::SetPosition(float x, float y)
{
	m_xPos = x;
	m_yPos = y;
}

void CFighterPlane::Move(float x, float y, float deltaTime)
{
	m_xPos += (x * m_speed * deltaTime);
	m_yPos += (y * m_speed * deltaTime);
}

void CFighterPlane::DrawFighter(unsigned int tick)
{
	if (m_pFighter == NULL)
		return;

	m_pFighter->SetPosition(m_xPos, m_yPos);
	m_pFighter->Draw(m_rcImage.left, m_rcImage.top,
				m_rcImage.right, m_rcImage.bottom, tick);
}

void CFighterPlane::ChangeSpeed(float value)
{
	m_speed += value;
	if (m_speed < 0.0f)
		m_speed = 0.0f;
}