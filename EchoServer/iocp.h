//////////////////////////////////////////////////////////////////////
// Iocp.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _IOCP_H_
#define _IOCP_H_

#include <winsock2.h>

#define MAX_WORKER_THREAD   50 //��ũ�������� �ִ밳��

#define EXIT_WORK_THREAD   1 //��ũ�����带 �����ϱ� ���ؼ� ������.
#define DISCONNECT_USER    2 //Ŭ���̾�Ʈ�� ���� �����Ų��.
                             //���� ��Ŷ�� �������� ���� �����Ų��.


#define IO_RECV     0
#define IO_SEND    1
#define IO_NONE    2

class CUser;
typedef struct _PER_IO_OVERLAPPED : public WSAOVERLAPPED
{
	DWORD  io_type;
}PER_IO_OVERLAPPED, *PPER_IO_OVERLAPPED;


typedef struct _PER_SOCKET_CONTEXT 
{
	SOCKET          hSocket;
	CUser           *pUser;
} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

class CIOCP
{
private:
	static CIOCP		*m_pIOCP;

public:
	   HANDLE            m_hIOCP;

	   CRITICAL_SECTION  m_CriticalSection;             

protected:
	      HANDLE    m_hThreads[MAX_WORKER_THREAD];
		  BYTE      m_ThreadCount;		  

		  BOOL          m_isEnd;

protected:
	BOOL CreateListenSocket();

	void  Lock() { EnterCriticalSection(&m_CriticalSection); } 
	void  UnLock() { LeaveCriticalSection(&m_CriticalSection); }

public:
	CIOCP();
	virtual ~CIOCP();

	BOOL CreateIOCP();
	void CleanUp();

	static CIOCP *GetIOCP();
	static void ReleaseIOCP();

	void SetEndIOCP() { m_isEnd = TRUE; }
	BOOL IsEndIOCP() { return m_isEnd; }


	BOOL RegisterCompletionPort(PPER_SOCKET_CONTEXT lpPerSocketContext);
	BOOL GetCompletionStatus(DWORD* pOutCompletionKey, LPDWORD pdwOutBytesTransferred, WSAOVERLAPPED** pOutOverlapped, int* pErrCode=NULL, DWORD dwWaitingTime=INFINITE);
	BOOL PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred=0, WSAOVERLAPPED* pOverlapped=NULL, int* pErrCode=NULL);
};

#endif