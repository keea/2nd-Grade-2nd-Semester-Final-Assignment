#pragma once

#include "dsTexture.h"
#include <string>

using namespace std;

class CFighterPlane
{
protected:
	dsTexture	*m_pFighter;
	RECT		 m_rcImage;
	float        m_xPos;
	float        m_yPos;

	float        m_speed;

public:
	CFighterPlane();
	CFighterPlane(string name);
	virtual ~CFighterPlane();

	BOOL Create(string imagename);
	BOOL Create(string imagename, int srcX, int srcY, int srcW, int srcH);

	void GetPosition(float &x, float &y);
	void SetPosition(float x, float y);

	void ChangeSpeed(float value);

	void Move(float x, float y, float deltaTime);

	void DrawFighter(unsigned int tick);
};

