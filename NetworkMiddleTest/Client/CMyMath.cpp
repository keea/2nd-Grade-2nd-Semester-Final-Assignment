#include "CMyMath.h"

//���� ����
float Vector_Dot(Vector2 v0, Vector2 v1)
{
	float r;
	r = (v0.x * v1.x) + (v0.y * v1.y);
	return r;
}

float Vector_Length(Vector2 v)
{
	// ���ʹ� x��� y���� ��ġ���� ����� ���̸� �����Ѵ�.
	// x��� y���� �����̹Ƿ� ���ʹ� ��� ���� �����ﰢ���̶�� �� �� �ִ�.
	// �����ﰢ���� ������ ������ ����
	float l = sqrtf(v.x * v.x + v.y*v.y);
	return l;
}

Vector2 Vector_Normalize(Vector2 v)
{
	//���� ������ �ʿ��� �� ������ ũ�⸦ 1�� ����� �ش�.
	float l = Vector_Length(v);
	Vector2 n;
	n.x = v.x / l;
	n.y = v.y / l;
	return n;
}