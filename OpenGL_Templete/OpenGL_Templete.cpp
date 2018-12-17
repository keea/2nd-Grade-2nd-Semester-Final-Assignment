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

dsTexture		*g_pSpriteTitle;
dsTexture		*g_pSpriteDie;
dsTexture		*g_pSpriteWin;
dsTexture		*g_pSpriteBack;

void OnIdle();
void GetMousePostion(HWND hwnd, POINT * pos);
RECT g_ClientRect;
HWND g_hwnd;
bool g_IsGameStop;

void(*g_fnOnUpdate)(DWORD tick); //�Լ�������
void Init();
void OnTitle(DWORD tickCount);
void OnWin(DWORD tickCount);
void OnLose(DWORD tickCount);
void OnGame(DWORD tickCount);

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
	
	g_hwnd = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	g_pSpriteTitle = new dsTexture("title.png");
	g_pSpriteTitle->LoadFromFilename("title.png");

	g_pSpriteDie = new dsTexture("die.png");
	g_pSpriteDie->LoadFromFilename("die.png");

	g_pSpriteWin = new dsTexture("you_win.png");
	g_pSpriteWin->LoadFromFilename("you_win.png");

	g_pSpriteBack = new dsTexture("background.png");
	g_pSpriteBack->LoadFromFilename("background.png");

	GetClientRect(hwnd, &g_ClientRect);
	g_Char.Create("player.png");
	g_bullets.Create("bullet.png");
	g_enemy.Create("enemy.png");

	g_fnOnUpdate = OnTitle;

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
			OnIdle();
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
			if (g_fnOnUpdate == OnGame)
				g_Char.SetMoveDirection(MOVEDIR::UP);
			break;
		case 0x41: //aŰ �Է½�
			if (g_fnOnUpdate == OnGame)
				g_Char.SetMoveDirection(MOVEDIR::LEFT);
			break;
		case 0x53: //sŰ �Է½�
			if (g_fnOnUpdate == OnGame)
				g_Char.SetMoveDirection(MOVEDIR::DOWN);
			break;
		case 0x44: //dŰ �Է½�
			if (g_fnOnUpdate == OnGame)
				g_Char.SetMoveDirection(MOVEDIR::RIGHT);
			break;
		case VK_SPACE:
			if (g_fnOnUpdate == OnTitle) {
				Init();
				g_fnOnUpdate = OnGame;
			}
			else if (g_fnOnUpdate == OnWin || g_fnOnUpdate == OnLose) {
				g_fnOnUpdate = OnTitle;
			}
			break;
		}
	}
	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case 0x57:
			if (g_fnOnUpdate == OnGame)
				g_Char.StopMove(UP);
			break;
		case 0x53:
			if (g_fnOnUpdate == OnGame)
				g_Char.StopMove(DOWN);
			break;
		case 0x44:
			if (g_fnOnUpdate == OnGame)
				g_Char.StopMove(RIGHT);
			break;
		case 0x41:
			if (g_fnOnUpdate == OnGame)
				g_Char.StopMove(LEFT);
			break;
		default:
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (g_fnOnUpdate == OnGame) {
			Vector2 postion = g_Char.GetVecPos();
			Vector2 dir = g_Char.GetVecAtkDir();
			float angle = g_Char.GetAngle();

			g_bullets.CreateBullet(postion, dir, angle);
		}
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


void OnIdle()
{
	DWORD	tickCount = GetTickCount();

	g_OpenGL.BeginRender();	

	g_fnOnUpdate(tickCount);
	

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

void Init()
{
	g_Char.Init(g_ClientRect.right >> 1, g_ClientRect.bottom - 90);
	g_enemy.Init();
	g_bullets.Init();
	g_IsGameStop = false;
}

void OnTitle(DWORD tickCount) {
	g_pSpriteTitle->Draw(0, 0, 0);
}

void OnGame(DWORD tickCount) {
	POINT pos;
	GetMousePostion(g_hwnd, &pos);
	static void(*tempOnUpdate)(DWORD tick);
	static int m_TimeBtwStop = 0;

	if (!g_IsGameStop) {
		m_TimeBtwStop = 0;
		g_bullets.OnUpdate(tickCount - g_tick);


		g_Char.SetRotationAngle(pos);
		g_Char.OnUpdate(tickCount - g_tick);

		g_enemy.HaveDamage(g_bullets.GetDamage(g_enemy.GetEnemyRect()));
		g_enemy.OnUpdate(tickCount - g_tick);

		bool isCollision = g_enemy.IsCollision(g_Char.GetPlayerRect());
		if (isCollision)
			g_Char.OnCollisionEnter();

		if (g_Char.IsHpZero()) {
			g_IsGameStop = true;
			tempOnUpdate = OnLose;
		}
		else if (g_enemy.IsHpZero()) {
			g_IsGameStop = true;
			tempOnUpdate = OnWin;
		}
	}
	else {
		if (m_TimeBtwStop >= 1000) {
			g_fnOnUpdate = tempOnUpdate;
		}
		else {
			m_TimeBtwStop += tickCount - g_tick;
		}
	}

	g_pSpriteBack->Draw(0, 0, 0);
	g_enemy.OnDraw();
	g_bullets.OnDraw(0, 0);
	g_Char.OnDraw(300, 300);

}

void OnWin(DWORD tickCount) {
	g_pSpriteWin->Draw(0, 0, 0);
}

void OnLose(DWORD tickCount) {
	g_pSpriteDie->Draw(0, 0, 0);
}

