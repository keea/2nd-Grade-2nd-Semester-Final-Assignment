#include "EmptyUserContainer.h"


CEmptyUserContainer *CEmptyUserContainer::m_pEmptyUser = NULL;
CEmptyUserContainer::CEmptyUserContainer(void)
{
	CUser *pUser;
	for(int i = 0; i < MAX_USER_ARRAY; i++)
	{
		pUser = &m_User[i];
		m_mapUser[pUser] = pUser;
	}

	// �Ӱ迵�� �ʱ�ȭ
	InitializeCriticalSection(&m_cs);
}


CEmptyUserContainer::~CEmptyUserContainer(void)
{
	// �Ӱ迵�� ����
	DeleteCriticalSection(&m_cs);
}

void CEmptyUserContainer::Push(CUser *pUser)
{
	if(pUser == NULL)
		return;

	EnterCriticalSection(&m_cs);

	MAP_USER::iterator it = m_mapUser.find(pUser);
	if(it != m_mapUser.end())
	{
		LeaveCriticalSection(&m_cs);
		return;
	}
	m_mapUser[pUser] = pUser;

	LeaveCriticalSection(&m_cs);

	printf("��������(push) : %d\n", m_mapUser.size());
}

CUser *CEmptyUserContainer::Pop()
{
	CUser *pUser = NULL;

	EnterCriticalSection(&m_cs);

	if(m_mapUser.size() == 0)
	{
		LeaveCriticalSection(&m_cs);
		return NULL;
	}
	MAP_USER::iterator it = m_mapUser.begin();
	pUser = it->second;
	
	m_mapUser.erase(it);	

	printf("��������(pop) : %d\n", m_mapUser.size());

	LeaveCriticalSection(&m_cs);

	return pUser;
}

CEmptyUserContainer *CEmptyUserContainer::GetEmptyUserContainer()
{
	if(m_pEmptyUser == NULL)
	{
		m_pEmptyUser = new CEmptyUserContainer;
	}

	return m_pEmptyUser;
}

void CEmptyUserContainer::ReleaseEmptyUserContainer()
{
	if(m_pEmptyUser == NULL)
		return;

	delete m_pEmptyUser; m_pEmptyUser = NULL;
}