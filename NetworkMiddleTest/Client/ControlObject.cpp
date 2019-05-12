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
	OBJECTS::iterator it = objects.begin();
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

GameObject * ControlObject::CreateObject(std::string name, int srcX, int srcY, int srcW, int srcH, TYPE type)
{
	GameObject * gameObject;
	switch (type)
	{
	case NONE:
		gameObject = new GameObject(name, srcX, srcY, srcW, srcH, type);
		objects[name] = gameObject;
		break;
	case AIR:
		gameObject = new AirObject(name, type);
		objects[name] = gameObject;
		break;
	case BULLET:
		gameObject = new BulletObject(name, srcX, srcY, srcW, srcH, type);
		objects[name] = gameObject;
		break;
	default:
		break;
	}

	return gameObject;
}

void ControlObject::DrawImage(unsigned int tick)
{
	if (m_pImage == NULL)
		return;

	OBJECTS::iterator it = objects.begin();
	for (; it != objects.end(); it++) {
		GameObject * pSubObj = (GameObject *)it->second;//second는 map의 값(value)
		pSubObj->Draw(m_pImage, tick);
	}
}

void ControlObject::DeleteObject(std::string name)
{
	GameObject * obj = objects[name];
	delete obj; //메모리 해제
	objects.erase(name); //해쉬맵에서도 제거
}
