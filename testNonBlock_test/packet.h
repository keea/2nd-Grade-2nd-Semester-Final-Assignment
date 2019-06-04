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
	DWORD time;
}LOGIN;

#define PKT_REQ_LOGOUT		0xa0000002
typedef struct  _tgLogout : PACKETHEADER
{
}LOGOUT;



#define PKT_GAME_TEXT		0x10000001
typedef struct  _tgGameText : PACKETHEADER
{
	char	gameText[20];
	int posX = 0;
	int posY = 0;
}GAMETEXT;

#define PKT_RPOINT				0x01000010
typedef struct  _tgRPoint : PACKETHEADER
{
	int x;
	int y;
} RPOINT;

#pragma pack(pop)

#endif
