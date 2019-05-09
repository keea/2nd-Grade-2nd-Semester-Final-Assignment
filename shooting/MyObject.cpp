
#include "MyObject.h"


CMyObject::CMyObject()
{
	m_pSprite = NULL;
}


CMyObject::~CMyObject()
{
	delete m_pSprite;
	m_pSprite = NULL;
}
