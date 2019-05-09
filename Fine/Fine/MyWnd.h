#pragma once
#include "w01\twapi_w01.h"
#include "w01\twapi_w01_lib_info.h"
#include "ButtonInfo.h"
class MyWnd : public TW_Wnd
{
private:
	// 계산된 시간을 출력하기 위한 컨트롤 객체
	TW_FrameText * mp_show_time;

	//버튼 객체를 가리킬 포인터 변수
	TW_Button * mp_btn[20];
	ButtonInfo btnInfo;

	//진행된 시간과 시작 시간을 저장할 변수
	UINT32 m_check_time, m_start_time;

	void SetInitGame();

	void ShowMessageBox(string text);

public:
	MyWnd();
	virtual int OnCreate(CREATESTRUCT *ap_create_info);
	virtual void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, HWND ah_ctrl);
	void ShowTime(); //시간을 보여주는 함수
	virtual void OnTimer(unsigned int a_timer_id);
};

