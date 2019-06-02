#include "User.h"
#include "packet.h"
#include "UserManager.h"

static DWORD g_userID = 10000;

CUser::CUser(void)
{
	m_recvBuffer.len = RECV_BUFF_SIZE;
	m_recvBuffer.buf = m_packetBuffer;

	m_sdBuffer.len = RECV_BUFF_SIZE;
	m_sdBuffer.buf = m_sendBuffer;

	m_pIOOverlapped = new PER_IO_OVERLAPPED;
	m_pIOSendOverlapped = new PER_IO_OVERLAPPED;
	m_pSocketContext = new PER_SOCKET_CONTEXT;
}


CUser::~CUser(void)
{
	delete m_pIOOverlapped; m_pIOOverlapped = NULL;
	delete m_pIOSendOverlapped;  m_pIOSendOverlapped = NULL;
	delete m_pSocketContext; m_pSocketContext = NULL;
}


void CUser::OnInitUserSocket()
{
	if( m_pSocketContext->hSocket == INVALID_SOCKET ) return;
	if( m_pIOOverlapped->io_type == IO_NONE ) return; //이미 끊긴 유저다

	int      nRet;
	DWORD    dwRecvNumBytes = 0;
	DWORD    dwFlags = 0;	
	
	memset(m_pIOOverlapped, 0, sizeof(PER_IO_OVERLAPPED)); m_pIOOverlapped->io_type = IO_RECV;

	nRet = WSARecv( m_pSocketContext->hSocket, &m_recvBuffer, 1, 
					&dwRecvNumBytes, &dwFlags, (WSAOVERLAPPED *)m_pIOOverlapped, NULL );
	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{			
		//OutputDebugString("WSARecv Failed!.....................................\n");
		CloseUserSocket();
	}
}

void CUser::ReleaseSocketContext()
{
	m_pSocketContext->hSocket = INVALID_SOCKET;
	m_pSocketContext->pUser = NULL;

	memset(m_pIOOverlapped, 0, sizeof(PER_IO_OVERLAPPED));
	memset(m_pIOSendOverlapped, 0, sizeof(PER_IO_OVERLAPPED));
}

BOOL CUser::ConnectUser(SOCKET client_socket)
{
	m_hSocket = client_socket; 

	memset(m_pIOOverlapped, 0, sizeof(PER_IO_OVERLAPPED));
	memset(m_pIOSendOverlapped, 0, sizeof(PER_IO_OVERLAPPED));

	m_pSocketContext->hSocket = client_socket;
	m_pSocketContext->pUser = this;

	m_pIOOverlapped->io_type = IO_RECV;
	m_pIOSendOverlapped->io_type = IO_SEND;
 
	m_name = "";
	m_userID = g_userID++;

	CIOCP *pIOCP = CIOCP::GetIOCP();
	if(pIOCP == FALSE)
		return FALSE;

	if( !pIOCP->RegisterCompletionPort(m_pSocketContext) )
	{		
		return FALSE;
	}

	OnInitUserSocket();

	return TRUE;
}

// 서버가 접속한 유저의 소켓을 닫을때.
// 완료 포트한테 해당 유저의 소켓을 닫으라구 알려준다.
void CUser::CloseUserSocket()
{
	CIOCP *pIOCP = CIOCP::GetIOCP();
	
	if( m_pSocketContext->hSocket == INVALID_SOCKET) return;

	m_pIOOverlapped->io_type = IO_NONE;
	shutdown(m_pSocketContext->hSocket, SD_RECEIVE); //받기금지

	BOOL ret = pIOCP->PostCompletionStatus((DWORD)m_pSocketContext, 0, (WSAOVERLAPPED *)m_pIOOverlapped);
#ifdef _DEBUG
	if(ret == FALSE)
	{
		//OutputDebugString("[ERROR]소켓 닫기 메시지 전송 실패[CUser::CloseUserSocket]\n");
	}
#endif
	
}

//--------------------------------- 패킷 수신 관련 ---------------------------------//
BOOL CUser::OnSendGamePacket(int recv_byte)
{	
	int recvsize = m_packetQueue.OnPutData(m_recvBuffer.buf, recv_byte);
	if(recvsize == -1)
	{
		return FALSE;
	}


	PACKETHEADER *pPacket;

	while( 1 )
	{
		if( (pPacket = m_packetQueue.GetPacket()) == NULL )
		{
			break;		
		}

		pPacket->userID = m_userID;

		char *pSendData = GetSendBuffer();
		memcpy(pSendData, pPacket, pPacket->PktSize);
		m_sdBuffer.len = pPacket->PktSize;

		CUserManager *pManager = CUserManager::GetUserManager();
		if(pManager != NULL)
		{
			pManager->OnSendAllUser(&m_sdBuffer);
		}

		m_packetQueue.OnPopData(pPacket->PktSize);
	}



	OnInitUserSocket();

	return TRUE;
}

//--------------------------------- 패킷 송신 관련 ---------------------------------//
BOOL CUser::OnSendPacket()
{
	if( m_pSocketContext->hSocket == INVALID_SOCKET ) return FALSE;

	DWORD sendByte;
	PACKETHEADER *pPacket = (PACKETHEADER *)GetSendBuffer();
	m_sdBuffer.len = pPacket->PktSize;

	int ret = WSASend(m_pSocketContext->hSocket, &m_sdBuffer, 1, &sendByte, 0, (WSAOVERLAPPED *)m_pIOSendOverlapped, NULL);
	if( ret == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
#ifdef _DEBUG
			ret = WSAGetLastError();
#endif
			CloseUserSocket();

			return FALSE;
		}
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
BOOL CUser::OnSendPacket(WSABUF *pWsaBuff)
{
	if( m_pSocketContext->hSocket == INVALID_SOCKET ) return FALSE;

	DWORD sendByte;
	int ret = WSASend(m_pSocketContext->hSocket, pWsaBuff, 1, &sendByte, 0, (WSAOVERLAPPED *)m_pIOSendOverlapped, NULL);
	if( ret == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
#ifdef _DEBUG
			ret = WSAGetLastError();
#endif
			CloseUserSocket();

			return FALSE;
		}	
	}


	return TRUE;
}