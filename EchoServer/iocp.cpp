//////////////////////////////////////////////////////////////////////
// Iocp.cpp
//
//////////////////////////////////////////////////////////////////////

#include "Iocp.h"
#include "UserManager.h"
#include "EmptyUserContainer.h"


DWORD WINAPI Accept(LPVOID pAcceptOL);
DWORD WINAPI WorkerThread(LPVOID WorkThreadContext);

//������ ������ ��ȣ.
//������ �����Ҷ� ���� 1�� �����ؼ� �ο��Ѵ�.
static DWORD g_userID = 100;


CIOCP		*CIOCP::m_pIOCP = NULL;

CIOCP *CIOCP::GetIOCP()
{
	if(m_pIOCP == NULL)
	{
		m_pIOCP = new CIOCP();
	}

	return m_pIOCP;
}

void CIOCP::ReleaseIOCP()
{
	if(m_pIOCP != NULL)
		delete m_pIOCP;

	m_pIOCP = NULL;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIOCP::CIOCP() 
{
	m_hIOCP = NULL;
	m_ThreadCount = 0;


	m_isEnd = FALSE;

	//CreateIOCP();
}

CIOCP::~CIOCP() 
{
	
	CleanUp();
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
BOOL CIOCP::CreateIOCP() 
{
    SYSTEM_INFO         systemInfo;
    WSADATA             wsaData;
    int                 nRet;

	if(m_hIOCP) return TRUE;

    GetSystemInfo(&systemInfo);
    m_ThreadCount = (BYTE)systemInfo.dwNumberOfProcessors * 2 + 1;
    if ((nRet = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
    {
        return FALSE;
    }
    
    InitializeCriticalSection(&m_CriticalSection);

	// ����̽� �ڵ�� IOCP Ŀ�� ��ü�� ���� ��Ű��... 
	// IOCP�� ���� ��Ű�� �۾��̴�.
    m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);//m_ThreadCount);
    //////////////////////////////////////////////////////////////////////////////////////////
    // CPU�� ������ ������ ���� ���� �Ѵ�. ������ ���ڸ� 0 ���� �ߴµ� ������ ����.
    // �������� 0�̸� ����Ʈ�� CPU���ڷ� �Ѵ�.
    //////////////////////////////////////////////////////////////////////////////////////////

    if (m_hIOCP == NULL) 
	{
        CleanUp();
        return FALSE;
    }

//	m_ThreadCount += 5;

	HANDLE  hThread;
    DWORD   dwThreadId;
    for (BYTE dwCPU=0; dwCPU < m_ThreadCount; dwCPU++) 
	{
        
        // (4) �Ϸ� �̺�Ʈ���� ���������� Ǯ�� ���� ����.
        hThread = CreateThread(NULL, 0, WorkerThread, this, 0, &dwThreadId);
        if (hThread == NULL) 
		{            
            CleanUp();
            return FALSE;
        }
        m_hThreads[dwCPU] = hThread;

    }

	// accept�� ȣ���Ѵ�.
    hThread = CreateThread(NULL, 0, Accept, this, 0, &dwThreadId);
	//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);// THREAD_PRIORITY_HIGHEST

	return TRUE;
}            

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CIOCP::CleanUp()
{
    // Cause worker threads to exit
    if (m_hIOCP) 
	{
        for (BYTE i = 0; i < m_ThreadCount; i++)
		{
			//Sleep(10);
			BOOL ret = PostCompletionStatus(EXIT_WORK_THREAD);
#ifdef _DEBUG
			if(ret == FALSE)
			{
				//OutputDebugString("[ERROR]���� �ݱ� �޽��� ���� ����[CUser::CloseUserSocket]\n");
			}
#endif
            //PostQueuedCompletionStatus(m_hIOCP, 0, EXIT_WORK_THREAD, NULL);      //�����带 �����ϰ� �����.			 
		}
    }


    //Make sure worker threads exits.
    if (WAIT_FAILED == WaitForMultipleObjects( m_ThreadCount,  m_hThreads, FALSE,  10000)) 
	{
        MessageBox(NULL, "WaitForMultipleObjects failed", "Error", MB_OK);
	}

    for (BYTE i = 0; i < m_ThreadCount; i++) 
	{
        if (m_hThreads[i] != INVALID_HANDLE_VALUE) 
			CloseHandle(m_hThreads[i]);
            m_hThreads[i] = INVALID_HANDLE_VALUE;
    }

	if (m_hIOCP) 
	{
        CloseHandle(m_hIOCP);
        m_hIOCP = NULL;
    }

    DeleteCriticalSection(&m_CriticalSection);

    WSACleanup();
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
BOOL CIOCP::RegisterCompletionPort(PPER_SOCKET_CONTEXT lpPerSocketContext)
{
	if (lpPerSocketContext == NULL) 
	{
		//OutputDebugString( "RegisterCompletionPort() fail.\n" );
		return FALSE;
	}

	Lock();

  // �Ҵ�� ����ü�� ������ m_hIOCP �����Ѵ�.  IOCP ��ü�� ���� �Ѵ�.
  // IOCP Ŀ�� ��ü�� ���� ��Ų��.	
	if(!CreateIoCompletionPort((HANDLE)lpPerSocketContext->hSocket, m_hIOCP, (DWORD)lpPerSocketContext, 0)) 
	{
		//OutputDebugString( "RegisterCompletionPort() fail.\n" );

		UnLock();
		return FALSE;
	}

	UnLock();

  return TRUE;
}

//--------------------------------------------------------------------------------------------
//
BOOL CIOCP::GetCompletionStatus(DWORD* pOutCompletionKey, LPDWORD pdwOutBytesTransferred, WSAOVERLAPPED** pOutOverlapped, int* pErrCode, DWORD dwWaitingTime)
{
	BOOL ret=GetQueuedCompletionStatus(m_hIOCP, pdwOutBytesTransferred, pOutCompletionKey, pOutOverlapped, dwWaitingTime);

	if(!ret && pErrCode != NULL)
	{
       *pErrCode = GetLastError();	   
	}

	return ret;
}

//--------------------------------------------------------------------------------------------
//
BOOL CIOCP::PostCompletionStatus(DWORD CompletionKey, DWORD dwBytesTransferred, WSAOVERLAPPED* pOverlapped, int* pErrCode)
{
	BOOL ret=PostQueuedCompletionStatus(m_hIOCP, dwBytesTransferred, CompletionKey, pOverlapped);

	if(!ret && pErrCode != NULL)
	{
       *pErrCode = GetLastError();	   
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
DWORD WINAPI Accept(LPVOID pAcceptOL)
{
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET)
		return 0;
	
	// bind()
	int ret;
	SOCKADDR_IN serveraddr;

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(30000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(ret == SOCKET_ERROR)
	{
		MessageBox(NULL, "bind failed!", "error", MB_OK);
		closesocket(listen_sock);
		return 0;
	}

	int send_size;
	int send_len = sizeof(send_size);
	if(getsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&send_size, &send_len) != SOCKET_ERROR)
	{
		send_size *= 100;
		if(setsockopt(listen_sock,SOL_SOCKET,SO_SNDBUF,(char*)&send_size,sizeof(send_size)) == SOCKET_ERROR)
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			char filename[512];
			wsprintf(filename, "btz_connect_error[%d%02d%02d].txt", st.wYear, st.wMonth, st.wDay);

			FILE *fp = fopen(filename, "at");
			if(fp != NULL)
			{
				fprintf(fp, "[connect fail1] time: %d\n", send_size);
				fclose(fp);
			}
		}
	}
	else
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		char filename[512];
		wsprintf(filename, "btz_connect_error[%d%02d%02d].txt", st.wYear, st.wMonth, st.wDay);

		FILE *fp = fopen(filename, "at");
		if(fp != NULL)
		{
			fprintf(fp, "[connect fail2]\n");
			fclose(fp);
		}
	}
	
	// listen()
	ret = listen(listen_sock, SOMAXCONN);
	if(ret == SOCKET_ERROR) 
	{
		MessageBox(NULL, "listen failed!", "error", MB_OK);
		closesocket(listen_sock);
		return 0;
	}

	
	CUser                   *pUser;
	int nZero;
	SOCKET client_sock;
	int addrlen;
	SOCKADDR_IN clientaddr;
	while(1)
	{

		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET)
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			char filename[512];
			wsprintf(filename, "btz_connect_error[%d%02d%02d].txt", st.wYear, st.wMonth, st.wDay);

			FILE *fp = fopen(filename, "at");
			if(fp != NULL)
			{
				fprintf(fp, "[connect fail3]\n");
				fclose(fp);
			}
			continue;
		}


		CEmptyUserContainer *pUserContainer = CEmptyUserContainer::GetEmptyUserContainer();

		CUser *pUser = pUserContainer->Pop();
		pUser->ConnectUser(client_sock);

		CUserManager *pUserMgr =CUserManager::GetUserManager();
		pUserMgr->AddUser(client_sock, pUser);

		/*
		pUser = CUserManager::GetSingleton().GetListReadyUser();			
		if(pUser != NULL)
		{	
			CConnectManager::GetSingleton().ConnectUser(g_userID, pUser);

			pUser->InitUser(g_userID++, client_sock, clientaddr);				
		}
		*/

	}
		          
	return 0;
}




//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkerThread(LPVOID WorkThreadContext)
{
    BOOL                 ret = FALSE;
    PPER_IO_OVERLAPPED   lpOverlapped = NULL;
    PPER_SOCKET_CONTEXT  lpPerSocketContext = NULL;
    DWORD                user_id = 0;
    DWORD                dwIOSize;   
	int                  ErrCode;


    while (TRUE) 
	{
		CIOCP *pIOCP = CIOCP::GetIOCP();

		if(pIOCP == NULL)
			continue;

        // continually loop to service io completion packets
		// ���Լ��� ȣ��Ǹ� Thread pool �� �����ȴ�.
		ret = pIOCP->GetCompletionStatus(reinterpret_cast<DWORD*>(&lpPerSocketContext),
			                                        &dwIOSize,
													reinterpret_cast<WSAOVERLAPPED **>(&lpOverlapped),
													&ErrCode);

        

		if(ret)
		{
			if( ((int)lpPerSocketContext) == EXIT_WORK_THREAD)
				break;

			if(lpOverlapped == NULL)
				continue;
		}
		else
		{
			//������ 64(ERROR_NETNAME_DELETED)�� ���ɼ��� ����.
			//�� ������ ��Ʈ��ũ �̸��� ����� �� �����ϴ�
			//close
			if(lpOverlapped != NULL) 
			{
				//CConnectManager::GetSingleton().DisconnectUser(lpPerSocketContext);
			}

			continue;
		}

		//�̹� ���� �������..
		if( (lpPerSocketContext == NULL) || 
			(lpPerSocketContext->hSocket == INVALID_SOCKET) || 
			(lpPerSocketContext->pUser == NULL) ) continue;


		// Ŭ���̾�Ʈ�� ���� ���� 
		if(dwIOSize == 0)
		{
			//CConnectManager::GetSingleton().DisconnectUser(lpPerSocketContext);
			continue;
		}

		if(lpOverlapped->io_type == IO_RECV)
		{
			lpPerSocketContext->pUser->OnSendGamePacket(dwIOSize);
			//if(lpPerSocketContext->pUser->OnSendGamePacket(dwIOSize) == FALSE)
			//	CConnectManager::GetSingleton().DisconnectUser(lpPerSocketContext);
		}
		else if(lpOverlapped->io_type == IO_SEND)
		{			
		}
		else
		{
			//CConnectManager::GetSingleton().DisconnectUser(lpPerSocketContext);
		}
    }   

	ExitThread(0);

    return 0;
}