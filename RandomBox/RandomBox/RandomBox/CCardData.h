#pragma once
class CCardData
{
private:
	int mCount;
	COLORREF mColor;
	CString mName;
public:
	CCardData();
	CCardData(int count, CString name, COLORREF color);
	int GetCount();
	COLORREF GetColor();
	CString GetName();
	void SetInitCount();
	void AddCount();
	~CCardData();
};

