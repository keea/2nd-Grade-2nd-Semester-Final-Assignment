// Fine.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Fine.h"
#include "w01/twapi_w01.h"
#include "w01/twapi_w01_lib_info.h"
#include "WinApp.h"
#include <time.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL));

	WinApp tips_app(hInstance, L"find");
	return tips_app.NormalProcess();
}
