#include "stdafx.h"
#include "resource.h"
#include "MyWnd.h"
#include "Dialog.h"
#include "ChangeStrType.h"
#include <iostream>


//���� �ʱ�ȭ �ϴ� �Լ�.
void MyWnd::SetInitGame()
{
	btnInfo.Init();
	for (int i = 0; i < 20; i++) {
		string name = btnInfo.GetBtn(i)->getName();
		mp_btn[i]->SetTitleString(ConvertLPCSTRToLPWSTR(name.c_str()));
		mp_btn[i]->RedrawControl();
	}

	//üũ �ð��� 0���� �ʱ�ȭ �մϴ�.
	m_check_time = 0;
	//�ð��� üũ�ϴ� ���� ���� ��ư�� ���� �������� �����Ѵ�.
	m_start_time = GetTickCount();
	ShowTime(); //��Ʈ�ѿ� ǥ�õ� �ð� ������ �����Ѵ�.
	SetTimer(1, 50);

}

//�޽��� �ڽ��� ����.
void MyWnd::ShowMessageBox(string text)
{
	//�ϴ� Ÿ�̸Ӹ� �����.
	KillTimer(1);//Ÿ�̸Ӹ� ���� ��Ų��.

	string msg = text + "\r\n ������ ��� �����Ͻðڽ��ϱ�?";
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
	m_check_time = 0; //���� �ð��� �ʱ�ȭ
}

int MyWnd::OnCreate(CREATESTRUCT * ap_create_info)
{
	TW_Wnd::OnCreate(ap_create_info);

	RECT rc = { 0,0,266,363 };
	::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//�ð� ���� ����� ��Ʈ���� ����.
	mp_show_time = new TW_FrameText(this, L"00 : 00", 5, 31, 256, 60);
	//��Ʈ���� ��Ʈ�� ���� ũ�� �����ϰ� �߰��� ���ĵǵ��� ����
	mp_show_time->SetFont(L"���� ���", 26.0f, ALIGNMENT_CENTER);
	//mp_show_time ��ü�� �� ������ ��Ʈ�ѷ� ����Ѵ�.
	AddControl(mp_show_time);

	btnInfo.Init();

	for (int i = 0; i < 20; i++) {
		string name = btnInfo.GetBtn(i)->getName();
		mp_btn[i] = new TW_Button(this, ConvertLPCSTRToLPWSTR(name.c_str()),
			5 + (i % 4) * 64, (69 + (i / 4) * 50)+31, 63, 49);
		//������ ��ư�� �� �������� ��Ʈ�ѷ� �߰�
		AddControl(mp_btn[i]);
	}

	SetTimer(1, 50);
	m_start_time = GetTickCount() - m_check_time;

	return 0;
}

void MyWnd::OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, HWND ah_ctrl)
{
	//20���� ��ư�� 1001������ 1020�������� ID�� ������.
	if (a_ctrl_id >= 1001 && a_ctrl_id < 1001 + 20) {
		if (btnInfo.GetBtn(a_ctrl_id - 1001)->GetIsAnswer()) {
			KillTimer(1);//Ÿ�̸Ӹ� ���� ��Ų��.
			int rank_time = m_check_time;

			//��ŷâ�� ����.
			DialogBox(gp_app->GetInstanceHandle(), MAKEINTRESOURCE(IDD_ITEM_MENU),
				mh_wnd, MyMenuDialogProc);
		}
		else {
			ShowMessageBox("���� ����");
		}
		SetInitGame();
	}
}

void MyWnd::ShowTime()
{
	TW_String str;
	
	//���� �ð��� seconds : milliseconds �������� ����ϰ� ����.
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