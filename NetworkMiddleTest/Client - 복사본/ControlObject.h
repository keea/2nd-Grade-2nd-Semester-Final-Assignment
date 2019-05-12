#pragma once
#include <map>
#include "AirObject.h"
#include "BulletObject.h"

class ControlObject
{
public:
	ControlObject();
	ControlObject(std::string name);
	~ControlObject();
	BOOL Create(std::string imagename);
	GameObject * CreateObject(std::string name, int srcX, int srcY,
		int srcW, int srcH, TYPE type);
	void DrawImage(unsigned int tick);
	void DeleteObject(std::string name);

private:
	dsTexture *m_pImage;
	typedef std::map<std::string, GameObject *> OBJECTS;
	OBJECTS objects;
};

