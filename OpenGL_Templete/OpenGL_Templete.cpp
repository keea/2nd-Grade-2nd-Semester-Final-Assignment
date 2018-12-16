// OpenGL_Templete.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "global.h"
#include "dsTexture.h"
#include <time.h>
#include "dsOpenALSoundManager.h"
#include "MyCharacter.h"
#include "CMyBullet.h"
#include "CMyEnemy.h"

CFrameOpenGL  g_OpenGL;
HDC           g_hDC;

CMyCharacter  g_Char;
DWORD         g_tick;
CMyBullet	  g_bullets; //총알 전체 관리하는 객체
CMyEnemy	  g_enemy;

void OnIdle(HWND hwnd);
void GetMousePostion(HWND hwnd, POINT * pos);
RECT g_ClientRect;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "OpenGL_Templete";

	if (RegisterClass(&wndclass) == 0)
	{
		return 0;
	}

	RECT WindowRect;                            // 사각형의 좌상단과 / 우하단 값을 저장함
	WindowRect.left = (long)0;                    // 왼쪽 값을 0으로 지정
	WindowRect.right = (long)1024;               // 오른쪽 값을 요청된 너비로 지정
	WindowRect.top = (long)0;                     // 위쪽 값을 0으로 지정
	WindowRect.bottom = (long)768;             // 아래쪽 값을 요청된 높이로 지정

	DWORD        dwExStyle;                        // 윈도우 확장 스타일
	DWORD        dwStyle;                          // 윈도우 스타일

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // 윈도우 확장 스타일
	dwStyle = WS_OVERLAPPEDWINDOW;                    // 윈도우즈 스타일

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // 실제 요청된 크기로 윈도우 크기를 조정한다

	HWND hwnd = CreateWindowEx(dwExStyle,                // 본 윈도우용 확장스타일
		"OpenGL_Templete",							    // 클래스 이름
		"DSGame",									   // 윈도우 타이틀
		WS_CLIPSIBLINGS |							    // 필수 윈도우 스타일
		WS_CLIPCHILDREN |						        // 필수 윈도우 스타일
		dwStyle,								    // 선택된 윈도우 스타일
		0, 0,									       // 창 위치
		WindowRect.right - WindowRect.left,			  // 조정된 창 너비를 계산함
		WindowRect.bottom - WindowRect.top,			   // 조정된 창 높이를 계산함
		NULL,												     // 부모 윈도우 없음
		NULL,									     // 메뉴 없음
		hInstance,										// 인스턴스
		NULL);										 // WM_CREATE에 아무것도 전달하지 않음


	if (hwnd == NULL)
	{
		return 0;
	}
	

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	GetClientRect(hwnd, &g_ClientRect);
	g_Char.Create("player.png");
	g_bullets.Create("bullet.png");
	g_enemy.Create("enemy.png");

	g_Char.Init(g_ClientRect.right >> 1, g_ClientRect.bottom - 90);
	g_enemy.Init();

	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	dsSound *pSound = pSoundManger->LoadSound("back.wav", true);
	if (pSound)
	{
		pSound->Play();
	}	

	g_tick = GetTickCount();

	MSG msg;
	while (1)
	{
		//윈도우 메세지가 있을경우 메세지를 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //메세지가 없을 경우 게임 루프를 실행한다.
		{
			OnIdle(hwnd);
		}
	}

	ReleaseDC(hwnd, g_hDC);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 0x57:// w키 입력시
			g_Char.SetMoveDirection(MOVEDIR::UP);
			break;
		case 0x41: //a키 입력시
			g_Char.SetMoveDirection(MOVEDIR::LEFT);
			break;
		case 0x53: //s키 입력시
			g_Char.SetMoveDirection(MOVEDIR::DOWN);
			break;
		case 0x44: //d키 입력시
			g_Char.SetMoveDirection(MOVEDIR::RIGHT);
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		Vector2 postion = g_Char.GetVecPos();
		Vector2 dir = g_Char.GetVecAtkDir();
		float angle = g_Char.GetAngle();

		g_bullets.CreateBullet(postion, dir, angle);

	}
	break;
	case WM_CREATE:
	{
		g_hDC = GetDC(hWnd);

		g_OpenGL.Create(g_hDC);
	}
	break;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		g_OpenGL.SetSize(width, height);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void OnIdle(HWND hwnd)
{
	DWORD	tickCount = GetTickCount();

	g_OpenGL.BeginRender();	

	POINT pos;
	GetMousePostion(hwnd, &pos);

	g_bullets.OnUpdate(tickCount - g_tick);
	

	g_Char.SetRotationAngle(pos);
	g_Char.OnUpdate(tickCount - g_tick);
	
	g_enemy.HaveDamage(g_bullets.GetDamage(g_enemy.GetEnemyRect()));
	g_enemy.OnUpdate(tickCount - g_tick);

	bool isCollision = g_enemy.IsCollision(g_Char.GetPlayerRect());
	if (isCollision)
		g_Char.OnCollisionEnter();

	g_enemy.OnDraw();
	g_bullets.OnDraw(0, 0);
	g_Char.OnDraw(300, 300);
	

	g_OpenGL.EndRender(g_hDC);

	g_tick = tickCount;
}

//마우스 포지션을 바꾸는 함수.
void GetMousePostion(HWND hwnd, POINT * pos)
{
	//마우스 정보 가져오기
	GetCursorPos(pos);
	ScreenToClient(hwnd, pos); //마우스좌표를 클라이언트 좌표로 변경.

	std::string s("마우스 좌표값 :  ");
	s += std::to_string(pos->x);
	s += std::string(",");
	s += std::to_string(pos->y);
	s += std::string("\r\n");

	const char * c = s.c_str();
	OutputDebugString(c);
}

