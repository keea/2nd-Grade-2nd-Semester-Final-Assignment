////////////////////////////////////////////////////////////
// packet.h
////////////////////////////////////////////////////////////

#ifndef _PACKET_H_
#define _PACKET_H_

#include <windows.h>

#pragma pack(push)
#pragma pack(1)

#define PKT_JOIN_P1			0x00000001
#define PKT_JOIN_P2			0x00000002

typedef struct _tgPacketHeader
{
	DWORD	PktID;
	WORD	PktSize;
}PACKETHEADER;


#pragma pack(pop)

#endif
