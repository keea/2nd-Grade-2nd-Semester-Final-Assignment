#include "CMyHpObject.h"


CMyHpObject::CMyHpObject(Vector2 vecPos) : m_vecPostion(vecPos)
{
	hp = 1024;
}

void CMyHpObject::Draw(dsTexture * pTexture)
{
	pTexture->Draw(0,  0, 0 , 0, hp, 10, 0);
}

CMyHpObject::~CMyHpObject()
{
}
