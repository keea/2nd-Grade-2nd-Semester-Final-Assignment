//packet.h
#ifndef _PACKET_H_
#define _PACKET_H_

#include <Windows.h>

#pragma  pack(push)
#pragma  pack(1)

typedef struct _tgPacketHeader
{
	DWORD	PktID;
	WORD	PktSize;

	DWORD userID;
	char        userStrID[20];
}PACKETHEADER;


#define PACKET_HEAD_SIZE   6


#define PKT_REQ_LOGIN	0xa0000001


#pragma pack(pop)
#endif