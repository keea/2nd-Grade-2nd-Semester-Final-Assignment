#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#include "EmptyUserContainer.h"
#include "UserManager.h"
#include "iocp.h"

// 소켓 함수 오류 출력
void ShowErrorMsg(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	CIOCP::GetIOCP();
	CEmptyUserContainer::GetEmptyUserContainer();
	CUserManager::GetUserManager();

	if(	CIOCP::GetIOCP()->CreateIOCP() == FALSE )
	{
		ShowErrorMsg("CreateIOCP()");

		return 0;
	}

	char str[32];

	while(1)
	{
		scanf("%s", str);
		if(!strcmp(str, "exit"))
			break;
	}

	CEmptyUserContainer::GetEmptyUserContainer()->ReleaseEmptyUserContainer();
	CUserManager::GetUserManager()->ReleaseUserManager();
	CIOCP::GetIOCP()->ReleaseIOCP();

	return 0;
}