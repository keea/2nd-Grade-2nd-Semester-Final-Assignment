#pragma once

#include <WinSock2.h>
#include <string>
#include "iocp.h"
#include "CirQueue.h"

#define RECV_BUFF_SIZE    10000

class CUser
{
private:
	PPER_SOCKET_CONTEXT    m_pSocketContext;
	PPER_IO_OVERLAPPED     m_pIOOverlapped;	
	PPER_IO_OVERLAPPED     m_pIOSendOverlapped;	


protected:
	SOCKET			m_hSocket;
	std::string		m_name;
	DWORD      m_userID;

	WSABUF             m_recvBuffer;
	char               m_packetBuffer[RECV_BUFF_SIZE];

	WSABUF             m_sdBuffer;
	char               m_sendBuffer[RECV_BUFF_SIZE];


	CCirQueue              m_packetQueue;

public:
	CUser(void);
	virtual ~CUser(void);

	BOOL ConnectUser(SOCKET hSocket);

	void OnInitUserSocket();
	void CloseUserSocket();
	void ReleaseSocketContext();

	char *GetSendBuffer() { return m_sendBuffer; }

	//--------------------------------- 패킷 수신 관련 ---------------------------------//
	BOOL OnSendGamePacket(int recv_byte);

	//--------------------------------- 패킷 송신 관련 ---------------------------------//
	BOOL   OnSendPacket();
	BOOL   OnSendPacket(WSABUF *pWsaBuff);
};

