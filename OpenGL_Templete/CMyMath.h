#pragma once
//M_PI 사용하기 위해
#define _USE_MATH_DEFINES 
#include <math.h>

//각도 표기 -> 라디안 표기 변환용
#define DEGREE_RADIAN(_deg)	( M_PI*(_deg)/180.0f )
//라디안 표기 -> 각도 표기 변환용
#define RADIAN_DEGREE(_rad)	((_rad)*180.0f/M_PI)

struct Vector2
{
	float x;
	float y;

	bool operator ==(const Vector2 v) {
		return x == v.x && y == v.y;
	}
};

//백터끼리 내적한다.
float Vector_Dot(Vector2 v0, Vector2 v1);

//백터 길이 계산.
float Vector_Length(Vector2 v);

//백터 정규화
Vector2 Vector_Normalize(Vector2 v);

