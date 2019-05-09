#pragma once
#include "w01\twapi_w01.h"
#include "w01\twapi_w01_lib_info.h"
#include "ButtonInfo.h"
class MyWnd : public TW_Wnd
{
private:
	// ���� �ð��� ����ϱ� ���� ��Ʈ�� ��ü
	TW_FrameText * mp_show_time;

	//��ư ��ü�� ����ų ������ ����
	TW_Button * mp_btn[20];
	ButtonInfo btnInfo;

	//����� �ð��� ���� �ð��� ������ ����
	UINT32 m_check_time, m_start_time;

	void SetInitGame();

	void ShowMessageBox(string text);

public:
	MyWnd();
	virtual int OnCreate(CREATESTRUCT *ap_create_info);
	virtual void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, HWND ah_ctrl);
	void ShowTime(); //�ð��� �����ִ� �Լ�
	virtual void OnTimer(unsigned int a_timer_id);
};

