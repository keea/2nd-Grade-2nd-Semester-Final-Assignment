#include "stdafx.h"
#include "WinApp.h"

void WinApp::InitInstance()
{
	mp_wnd = new MyWnd;
	mp_wnd->Create(L"�ٸ� ���� ã��", 50, 50, 266, 363);
	mp_wnd->ShowWindow();
	mp_wnd->UpdateWindow();
}
