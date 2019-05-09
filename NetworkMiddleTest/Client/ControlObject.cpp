#include "ControlObject.h"



ControlObject::ControlObject()
{
	m_pImage = NULL;
}


ControlObject::ControlObject(std::string name)
{
	m_pImage = new dsTexture(name);
}

ControlObject::~ControlObject()
{
	MAP_OBJECTS::iterator it = objects.begin();
	for (; it != objects.end(); it++) 
	{
		GameObject * pSubObj = (GameObject *)it->second;

		delete pSubObj;
	}
	objects.clear();

	delete m_pImage;
	m_pImage = NULL;
}

BOOL ControlObject::Create(std::string imagename)
{
	if (m_pImage == NULL)
		return NULL;

	if (!m_pImage->LoadFromFilename(imagename.c_str()))
		return FALSE;

	return TRUE;
}

GameObject * ControlObject::CreateObject(std::string name, int srcX, int srcY, int srcW, int srcH)
{
	GameObject * gameObject = new GameObject(name, srcX, srcY, srcW, srcH);
	objects[name] = gameObject;

	return gameObject;
}

void ControlObject::DrawImage(unsigned int tick)
{
	if (m_pImage == NULL)
		return;

	MAP_OBJECTS::iterator it = objects.begin();
	for (; it != objects.end(); it++) {
		GameObject * pSubObj = (GameObject *)it->second;//second´Â mapÀÇ °ª(value)
		pSubObj->Draw(m_pImage, tick);
	}
}
