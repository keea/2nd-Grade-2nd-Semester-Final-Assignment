#include "stdafx.h"
#include "resource.h"
#include "MyWnd.h"
#include "Dialog.h"
#include "ChangeStrType.h"
#include <iostream>


//게임 초기화 하는 함수.
void MyWnd::SetInitGame()
{
	btnInfo.Init();
	for (int i = 0; i < 20; i++) {
		string name = btnInfo.GetBtn(i)->getName();
		mp_btn[i]->SetTitleString(ConvertLPCSTRToLPWSTR(name.c_str()));
		mp_btn[i]->RedrawControl();
	}

	//체크 시간을 0으로 초기화 합니다.
	m_check_time = 0;
	//시간을 체크하는 시작 값을 버튼을 누른 시점으로 변경한다.
	m_start_time = GetTickCount();
	ShowTime(); //컨트롤에 표시된 시간 정보를 갱신한다.
	SetTimer(1, 50);

}

//메시지 박스를 띄운다.
void MyWnd::ShowMessageBox(string text)
{
	//일단 타이머를 멈춘다.
	KillTimer(1);//타이머를 중지 시킨다.

	string msg = text + "\r\n 게임을 계속 진행하시겠습니까?";
	int check = MessageBox(mh_wnd, ConvertLPCSTRToLPWSTR(msg.c_str()),
		ConvertLPCSTRToLPWSTR(text.c_str()), MB_ICONQUESTION | MB_OKCANCEL);

	if (check == IDOK) {
	}
	else {
		PostQuitMessage(0);
	}
}

MyWnd::MyWnd()
{
	m_check_time = 0; //계산된 시간을 초기화
}

int MyWnd::OnCreate(CREATESTRUCT * ap_create_info)
{
	TW_Wnd::OnCreate(ap_create_info);

	RECT rc = { 0,0,266,363 };
	::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//시간 값을 출력할 컨트롤을 생성.
	mp_show_time = new TW_FrameText(this, L"00 : 00", 5, 31, 256, 60);
	//컨트롤의 폰트를 좀더 크게 변경하고 중간에 정렬되도록 설정
	mp_show_time->SetFont(L"맑은 고딕", 26.0f, ALIGNMENT_CENTER);
	//mp_show_time 객체를 이 윈도의 컨트롤로 등록한다.
	AddControl(mp_show_time);

	btnInfo.Init();

	for (int i = 0; i < 20; i++) {
		string name = btnInfo.GetBtn(i)->getName();
		mp_btn[i] = new TW_Button(this, ConvertLPCSTRToLPWSTR(name.c_str()),
			5 + (i % 4) * 64, (69 + (i / 4) * 50)+31, 63, 49);
		//생성된 버튼을 이 윈도우의 컨트롤로 추가
		AddControl(mp_btn[i]);
	}

	SetTimer(1, 50);
	m_start_time = GetTickCount() - m_check_time;

	return 0;
}

void MyWnd::OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, HWND ah_ctrl)
{
	//20개의 버튼은 1001번에서 1020번까지의 ID를 가진다.
	if (a_ctrl_id >= 1001 && a_ctrl_id < 1001 + 20) {
		if (btnInfo.GetBtn(a_ctrl_id - 1001)->GetIsAnswer()) {
			KillTimer(1);//타이머를 중지 시킨다.
			int rank_time = m_check_time;

			//랭킹창을 띄운다.
			DialogBox(gp_app->GetInstanceHandle(), MAKEINTRESOURCE(IDD_ITEM_MENU),
				mh_wnd, MyMenuDialogProc);
		}
		else {
			ShowMessageBox("게임 오버");
		}
		SetInitGame();
	}
}

void MyWnd::ShowTime()
{
	TW_String str;
	
	//계산된 시간을 seconds : milliseconds 형식으로 출력하게 구성.
	str.Format(L"%02d : %02d", m_check_time / 1000, m_check_time % 100);
	mp_show_time->SetTitleString(str);
	mp_show_time->RedrawControl();
}

void MyWnd::OnTimer(unsigned int a_timer_id)
{
	if (1 == a_timer_id) {
		m_check_time = GetTickCount() - m_start_time;
		ShowTime();
	}
}