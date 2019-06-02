#pragma once

#include "User.h"
#include <map>

#define MAX_USER_ARRAY	3000



class CEmptyUserContainer
{
private:
	CUser m_User[MAX_USER_ARRAY];
	static CEmptyUserContainer *m_pEmptyUser;

protected:
	typedef std::map<CUser *, CUser *> MAP_USER;

	MAP_USER m_mapUser;

	CRITICAL_SECTION  m_cs;

public:
	CEmptyUserContainer(void);
	virtual ~CEmptyUserContainer(void);

	void Push(CUser *pUser);
	CUser *Pop();

	static CEmptyUserContainer *GetEmptyUserContainer();
	static void ReleaseEmptyUserContainer();
};

