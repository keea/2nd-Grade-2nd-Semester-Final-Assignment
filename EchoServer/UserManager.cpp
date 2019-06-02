#include "UserManager.h"

CUserManager *CUserManager::m_pUserManager = NULL;
CUserManager::CUserManager(void)
{
	InitializeCriticalSection(&m_CriticalSection);
}


CUserManager::~CUserManager(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}

CUserManager *CUserManager::GetUserManager()
{
	if(m_pUserManager == NULL)
	{
		m_pUserManager = new CUserManager();
	}
	return m_pUserManager;
}

void CUserManager::ReleaseUserManager()
{
	if(m_pUserManager == NULL)
		return;

	delete m_pUserManager; m_pUserManager = NULL;
}

void CUserManager::AddUser(SOCKET hSocket, CUser *pUser)
{
	m_mapSocketUser[hSocket] = pUser;
}

void CUserManager::AddUser(std::string name, CUser *pUser)
{
	if(FindUser(name) != NULL)
		return;

	m_mapNameUser[name] = pUser;
}

CUser *CUserManager::FindUser(SOCKET hSocket)
{
	MAP_SOCKET_USER::iterator it = m_mapSocketUser.find(hSocket);
	if(it != m_mapSocketUser.end())
	{
		return it->second;
	}

	return NULL;
}

CUser *CUserManager::FindUser(std::string name)
{
	MAP_NAME_USER::iterator it = m_mapNameUser.find(name);
	if(it != m_mapNameUser.end())
	{
		return it->second;
	}

	return NULL;
}

void CUserManager::RemoveUser(SOCKET hSocket)
{
	MAP_SOCKET_USER::iterator it = m_mapSocketUser.find(hSocket);
	if(it != m_mapSocketUser.end())
	{
		m_mapSocketUser.erase(it);
	}
}
void CUserManager::RemoveUser(std::string name)
{
	MAP_NAME_USER::iterator it = m_mapNameUser.find(name);
	if(it != m_mapNameUser.end())
	{
		m_mapNameUser.erase(it);
	}
}



void CUserManager::OnSendAllUser(WSABUF *pWsaBuff)
{
	CUser *pUser;

	//Lock();

	MAP_SOCKET_USER::iterator  iterator_user = m_mapSocketUser.begin();

	for( ;iterator_user != m_mapSocketUser.end(); iterator_user++)
	{
		pUser = (iterator_user->second);
		if( pUser == NULL ) continue;

		pUser->OnSendPacket(pWsaBuff);
	}

	//UnLock();

}


void CUserManager::OnSendSeeAllUserExceptMe(CUser *pMe)
{
	if(!pMe) return;

/*
	CUser *pUser;

	Lock();

	MAP_USER::iterator  iterator_user = m_mapUser.begin();

	for( ;iterator_user != m_mapUser.end(); iterator_user++)
	{
		pUser = (iterator_user->second);

		//if(pMe->GetUserID() == pUser->GetUserID()) continue;

		pUser->OnSendPacket(pMe->GetSendWsaBuf());
	}

	UnLock();
*/
}

void CUserManager::OnSendSeeAllUserInfoToMe(CUser *pMe)
{
	if(!pMe) return;

/*
	CUser *pUser;

	Lock();

	MAP_USER::iterator  iterator_user = m_mapUser.begin();

	for( ;iterator_user != m_mapUser.end(); iterator_user++)
	{
		pUser = (iterator_user->second);

		if(pMe->GetUserID() == pUser->GetUserID()) continue;

//		pUser->OnSendMyInfo(pMe);
	}

	UnLock();
*/
}
