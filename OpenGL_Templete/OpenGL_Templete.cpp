// OpenGL_Templete.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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
CMyBullet	  g_bullets; //�Ѿ� ��ü �����ϴ� ��ü
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

	RECT WindowRect;                            // �簢���� �»�ܰ� / ���ϴ� ���� ������
	WindowRect.left = (long)0;                    // ���� ���� 0���� ����
	WindowRect.right = (long)1024;               // ������ ���� ��û�� �ʺ�� ����
	WindowRect.top = (long)0;                     // ���� ���� 0���� ����
	WindowRect.bottom = (long)768;             // �Ʒ��� ���� ��û�� ���̷� ����

	DWORD        dwExStyle;                        // ������ Ȯ�� ��Ÿ��
	DWORD        dwStyle;                          // ������ ��Ÿ��

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // ������ Ȯ�� ��Ÿ��
	dwStyle = WS_OVERLAPPEDWINDOW;                    // �������� ��Ÿ��

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // ���� ��û�� ũ��� ������ ũ�⸦ �����Ѵ�

	HWND hwnd = CreateWindowEx(dwExStyle,                // �� ������� Ȯ�彺Ÿ��
		"OpenGL_Templete",							    // Ŭ���� �̸�
		"DSGame",									   // ������ Ÿ��Ʋ
		WS_CLIPSIBLINGS |							    // �ʼ� ������ ��Ÿ��
		WS_CLIPCHILDREN |						        // �ʼ� ������ ��Ÿ��
		dwStyle,								    // ���õ� ������ ��Ÿ��
		0, 0,									       // â ��ġ
		WindowRect.right - WindowRect.left,			  // ������ â �ʺ� �����
		WindowRect.bottom - WindowRect.top,			   // ������ â ���̸� �����
		NULL,												     // �θ� ������ ����
		NULL,									     // �޴� ����
		hInstance,										// �ν��Ͻ�
		NULL);										 // WM_CREATE�� �ƹ��͵� �������� ����


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
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
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
		case 0x57:// wŰ �Է½�
			g_Char.SetMoveDirection(MOVEDIR::UP);
			break;
		case 0x41: //aŰ �Է½�
			g_Char.SetMoveDirection(MOVEDIR::LEFT);
			break;
		case 0x53: //sŰ �Է½�
			g_Char.SetMoveDirection(MOVEDIR::DOWN);
			break;
		case 0x44: //dŰ �Է½�
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

//���콺 �������� �ٲٴ� �Լ�.
void GetMousePostion(HWND hwnd, POINT * pos)
{
	//���콺 ���� ��������
	GetCursorPos(pos);
	ScreenToClient(hwnd, pos); //���콺��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ����.

	std::string s("���콺 ��ǥ�� :  ");
	s += std::to_string(pos->x);
	s += std::string(",");
	s += std::to_string(pos->y);
	s += std::string("\r\n");

	const char * c = s.c_str();
	OutputDebugString(c);
}

