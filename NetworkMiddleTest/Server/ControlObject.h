#pragma once
#include <map>
#include "GameObject.h"
#include "AirObject.h"

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

private:
	dsTexture *m_pImage;
	typedef std::map<std::string, GameObject *> MAP_OBJECTS;
	MAP_OBJECTS objects;
};

