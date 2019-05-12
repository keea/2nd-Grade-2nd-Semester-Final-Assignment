////////////////////////////////////////////////////////////
// packet.h
////////////////////////////////////////////////////////////

#ifndef _PACKET_H_
#define _PACKET_H_

#include <windows.h>

#pragma pack(push)
#pragma pack(1)

#define PKT_JOIN			0x00000001
#define PKT_START			0x00000002
#define PKT_GAME			0x00000003

enum GAMESTATE {
	IDEL,
	READY, //게임 준비.
	GAME, //게임 중
	RESULT, //결과
};

typedef struct _tgPacketHeader
{
	DWORD	PktID;
	WORD	PktSize;
	GAMESTATE state;
}PACKETHEADER;

struct AIR_PACKET : PACKETHEADER {
	float PosX;
};

#pragma pack(pop)

#endif
