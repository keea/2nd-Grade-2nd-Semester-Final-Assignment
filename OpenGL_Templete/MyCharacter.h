#pragma once

#include "MyObject.h"
#include "CMyMath.h"

enum MOVEDIR {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
};


class CMyCharacter : public CMyObject
{
protected:	
	DWORD			 m_interval;

	Vector2			 m_vecDirection;
	Vector2          m_vecPosition;

	Vector2			 m_vecAtkDirection;
	POINT			 m_ptCenter;
	
	float			 m_angle;

	int				 m_width;
	int				 m_height;
	int				 m_speed;
	

public:
	CMyCharacter();
	~CMyCharacter();

	bool Create(char *filename);
	void OnDraw(float x, float y);
	void OnUpdate(DWORD tick);
	void Init(float x, float y);

	void SetRotationAngle(POINT targetPos);
	void SetMoveDirection(MOVEDIR dir);

	Vector2 GetVecAtkDir() { return m_vecAtkDirection; }
	Vector2 GetVecPos() { return m_vecPosition; }
	Vector2 GetCenterPos();
	float GetAngle() { return m_angle; }
};

