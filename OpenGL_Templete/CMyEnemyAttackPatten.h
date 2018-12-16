#pragma once
#include <Windows.h>
enum CURRENT_TYPE {
	NOTHING,
	START,
	DOING,
	END,
};

class CMyEnemyAttackPatten
{
protected:
	CURRENT_TYPE m_currentType;
public:
	CMyEnemyAttackPatten();
	~CMyEnemyAttackPatten();
	
	virtual CURRENT_TYPE OnUpdate(DWORD tick) = 0;
	virtual void OnDraw() = 0;
	virtual bool IsCollision(RECT rect) = 0;
	virtual void OnStart() = 0;
};

