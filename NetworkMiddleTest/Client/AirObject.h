#pragma once
#include "GameObject.h"
class AirObject : public GameObject
{
public:
	AirObject(std::string name, TYPE type)
		:GameObject(name, m_dieAni[0].left, m_dieAni[0].top, m_dieAni[0].right, m_dieAni[0].bottom, type) {
		m_speed = 300.0f;
		SetRect(&m_dieAni[0], 5, 323, 67, 49);
		SetRect(&m_dieAni[1], 74, 323, 67, 49);
		SetRect(&m_dieAni[2], 141, 323, 67, 49);
		SetRect(&m_dieAni[3], 208, 323, 67, 49);
		SetRect(&m_dieAni[4], 5, 381, 67, 49);
		SetRect(&m_dieAni[5], 7, 381, 67, 49);
		SetRect(&m_dieAni[6], 141, 381, 67, 49);
		SetRect(&m_dieAni[7], 208, 381, 67, 49);
		SetRect(&m_dieAni[8], 267, 323, 67, 49);

		m_updateTick = 0;
		m_aniIndex = 0;
		m_isDie = false;
	};

	void Move(float x, float y, float deltaTime);

	void ChangeSpeed(float value);

	virtual void Draw(dsTexture * pTexture, unsigned int tick);

	void SetDie();

	int GetWidth();

	RECT GetRect();

	~AirObject();

private:
	float        m_speed;
	RECT		 m_dieAni[9];
	float		 m_updateTick;
	int			 m_aniIndex;
	bool		 m_isDie;
};

