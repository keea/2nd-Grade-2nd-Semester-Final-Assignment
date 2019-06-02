////////////////////////////////////////////////////////////
// packet.h
////////////////////////////////////////////////////////////

#ifndef _PACKET_H_
#define _PACKET_H_

#include <windows.h>

#pragma pack(push)
#pragma pack(1)

typedef struct _tgPacketHeader
{
	DWORD	PktID;
	WORD	PktSize;

	DWORD userID;
	char        userStrID[20];
}PACKETHEADER;


#define PKT_REQ_LOGIN		0xa0000001
typedef struct  _tgLogin : PACKETHEADER
{
}LOGIN;

#define PKT_RPOINT				0x01000010
typedef struct  _tgRPoint : PACKETHEADER
{
	int x;
	int y;
} RPOINT;

#pragma pack(pop)

#endif
