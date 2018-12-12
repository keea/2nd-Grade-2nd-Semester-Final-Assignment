#pragma once
//M_PI ����ϱ� ����
#define _USE_MATH_DEFINES 
#include <math.h>

//���� ǥ�� -> ���� ǥ�� ��ȯ��
#define DEGREE_RADIAN(_deg)	( M_PI*(_deg)/180.0f )
//���� ǥ�� -> ���� ǥ�� ��ȯ��
#define RADIAN_DEGREE(_rad)	((_rad)*180.0f/M_PI)

struct Vector2
{
	float x;
	float y;

	bool operator ==(const Vector2 v) {
		return x == v.x && y == v.y;
	}
};

//���ͳ��� �����Ѵ�.
float Vector_Dot(Vector2 v0, Vector2 v1);

//���� ���� ���.
float Vector_Length(Vector2 v);

//���� ����ȭ
Vector2 Vector_Normalize(Vector2 v);

