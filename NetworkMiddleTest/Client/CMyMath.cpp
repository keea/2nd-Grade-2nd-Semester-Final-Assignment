#include "CMyMath.h"

//벡터 내적
float Vector_Dot(Vector2 v0, Vector2 v1)
{
	float r;
	r = (v0.x * v1.x) + (v0.y * v1.y);
	return r;
}

float Vector_Length(Vector2 v)
{
	// 벡터는 x축과 y축의 위치에서 방향과 길이를 정의한다.
	// x축과 y축은 수직이므로 벡터는 어떻게 보면 직각삼각형이라고 할 수 있다.
	// 직각삼각형의 빗변이 벡터의 길이
	float l = sqrtf(v.x * v.x + v.y*v.y);
	return l;
}

Vector2 Vector_Normalize(Vector2 v)
{
	//방향 정보만 필요할 때 벡터의 크기를 1로 만들어 준다.
	float l = Vector_Length(v);
	Vector2 n;
	n.x = v.x / l;
	n.y = v.y / l;
	return n;
}