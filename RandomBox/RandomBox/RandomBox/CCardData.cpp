#include "stdafx.h"
#include "CCardData.h"


CCardData::CCardData()
{
}

CCardData::CCardData(int count, CString name, COLORREF color)
{
	mCount = count; //ī�� ����
	mName = name; // �̸�
	mColor = color; //����
}

int CCardData::GetCount()
{
	return mCount;
}

void CCardData::SetInitCount()
{
	mCount = 0;
}

void CCardData::AddCount()
{
	mCount += 1;
}

COLORREF CCardData::GetColor()
{
	return mColor;
}

CString CCardData::GetName()
{
	return mName;
}


CCardData::~CCardData()
{
}
