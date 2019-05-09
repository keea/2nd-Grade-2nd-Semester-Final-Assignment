#include "GameObject.h"



GameObject::GameObject()
{
	SetRect(&m_rcImage, 0, 0, 0, 0);
	m_xPos = m_yPos = 0.0f;
}

GameObject::GameObject(std::string name, int srcX, int srcY, int srcW, int srcH, TYPE type)
{
	this->name = name;
	SetRect(&m_rcImage, srcX, srcY, srcW, srcH);
	m_xPos = m_yPos = 0.0f;
	m_type = type;
}


GameObject::~GameObject()
{
}

void GameObject::GetPosition(float & x, float & y)
{
	x = m_xPos;
	y = m_yPos;
}

void GameObject::SetPosition(float x, float y)
{
	m_xPos = x;
	m_yPos = y;
}

void GameObject::Draw(dsTexture * pTexture, unsigned int tick)
{
	pTexture->SetPosition(m_xPos, m_yPos);
	pTexture->Draw(m_rcImage.left, m_rcImage.top, m_rcImage.right, m_rcImage.bottom, tick);
}
