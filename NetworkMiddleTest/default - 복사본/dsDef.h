/*
 *  dsDef.h
  */

#ifndef _DSDEF_H_
#define _DSDEF_H_


#include "global.h"

#include <string>
#include <map>
#include <vector>
#include "dsVector2.h"
#include "dsVector3.h"




enum LANDSCAPE_MODE
{
	LANDSCAPE_NORMAL	=	0,
	LANDSCAPE_RIGHT		=	1,
	LANDSCAPE_LEFT		=	2
};


struct dsRECTf
{
	float left, top, right, bottom;
};

struct dsRECTs
{
	short left, top, right, bottom;
};

struct dsRECTi
{
	int left, top, right, bottom;
};


struct dsPointf
{
	float x, y;
};

struct dsPoints
{
	short x, y;
};

struct dsPointi
{
	int x, y;
};

struct dsDrawOP
{
	std::string objname; //오브젝트 이름
	std::string name; //aniname
	unsigned char idxDir; //한 애니메이션에서의 방향
	dsPoints pos;
	unsigned int tick;
	unsigned int interval;
	unsigned char iFrame;
	bool isLoop;
	bool isMirror; //y축 회전 
	bool isCollisionObject; //충돌체크 맵에 추가?
	bool isCollider; //이 오브젝트로 충돌체크 할 것인가?
	
	void *pData;
	
	GLfloat rectDraw[8]; //화면에 출력할 사각형 영역
	GLfloat rectCoord[8]; //텍스쳐 좌표
};

#endif