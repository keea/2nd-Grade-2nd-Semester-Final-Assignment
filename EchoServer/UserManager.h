#pragma once

#include "User.h"
#include <map>

class CUserManager
{
protected:
	static  CUserManager *m_pUserManager;

	typedef std::map<SOCKET, CUser *> MAP_SOCKET_USER;
	typedef std::map<std::string, CUser *> MAP_NAME_USER;

	MAP_SOCKET_USER m_mapSocketUser;
	MAP_NAME_USER   m_mapNameUser;

	CRITICAL_SECTION       m_CriticalSection;


public:
	CUserManager(void);
	virtual ~CUserManager(void);

	static CUserManager *GetUserManager();
	static void ReleaseUserManager();

	void AddUser(SOCKET hSocket, CUser *pUser);
	void AddUser(std::string name, CUser *pUser);

	CUser *FindUser(SOCKET hSocket);
	CUser *FindUser(std::string name);

	void RemoveUser(SOCKET hSocket);
	void RemoveUser(std::string name);

	//---------------------------------- 패킷 처리 관련 ----------------------------//
	void OnSendAllUser(WSABUF *pWsaBuff);
	void OnSendAllUser(); //전체 유저한테 보낸다.
	void OnSendSeeAllUserExceptMe(CUser *pMe);
	void OnSendSeeAllUserInfoToMe(CUser *pMe);// 다른 사람 정보를 나한테 보낸다.
};

