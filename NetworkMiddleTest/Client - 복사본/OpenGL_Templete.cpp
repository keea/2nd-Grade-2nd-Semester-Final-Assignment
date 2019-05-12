// OpenGL_Templete.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <winsock2.h>
#include "global.h"
#include "dsTexture.h"
#include <time.h>
#include "dsOpenALSoundManager.h"
#include "CirQueue.h"
#include "FighterPlane.h"
#include "ControlObject.h"
#include "AirObject.h"


SOCKET g_hSocket;
#define WM_SOCKET   WM_USER+1

CCirQueue g_Queue;

CFrameOpenGL  g_OpenGL;
HDC           g_hDC;

//CFighterPlane g_myAir("myAir");
ControlObject g_control("img");
AirObject * g_myAir;
AirObject * g_enemyAir;
float g_createBulletPosX;
bool g_isBulletCreate = false;

void OnIdle(float deltaTime);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

SOCKADDR_IN g_toHost;
GAMESTATE g_state;
vector<BulletObject * > bullets;
int g_createBullet = 0;
float g_bulletCollTime = 0;

#define DEFAULT_POS_X 250
#define DEFAULT_POS_Y 350
#define COOLTIME	3000

void Game(float deltaTime);

// ������ ������
int Send(SOCKET sock, char *buf, int size)
{
	int retval = sendto(sock, buf, size,
		0, (SOCKADDR *)&g_toHost, sizeof(g_toHost));
	if (retval == SOCKET_ERROR) 
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::MessageBox(NULL, "sendto error!", "Error", MB_OK);
		}		
	}

	return retval;
}


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

	//g_myAir.Create("./data/1942.png", 5, 323, 67, 49);

	

	g_control.Create("./data/1942.png");
	g_myAir = (AirObject *)g_control.CreateObject("myAir", 5, 323, 67, 49, TYPE::AIR);
	g_myAir->SetPosition(DEFAULT_POS_X, DEFAULT_POS_Y);

	//���� ������ ����
	char buf[128];
	GAMEDATA_MOVE *pHeader = (GAMEDATA_MOVE *)buf;
	pHeader->PktID = PKT_JOIN;
	pHeader->PktSize = sizeof(GAMEDATA_MOVE);
	float x = 0;
	float y = 0;
	g_state = IDEL;

	g_myAir->GetPosition(&x, &y);
	pHeader->PosX = x;
	pHeader->state = g_state;

	
	Send(g_hSocket, buf, pHeader->PktSize);

	/*dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	dsSound *pSound = pSoundManger->LoadSound("back.wav", true);
	if (pSound)
	{
		pSound->Play();
	}*/	


	MSG msg;
	DWORD tick = GetTickCount();
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
			DWORD cur_tick = GetTickCount();
			float deltaTime = (float)(cur_tick - tick);
			g_bulletCollTime -= deltaTime;
			OnIdle(deltaTime);

			tick = cur_tick;
		}
	}

	ReleaseDC(hwnd, g_hDC);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SOCKET:
	{
		// ���� �߻� ���� Ȯ��
		if (WSAGETSELECTERROR(lParam))
		{
			//err_display(WSAGETSELECTERROR(lParam));
			break;
		}

		// �޽��� ó��
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
#define BUFSIZE  1024

			SOCKADDR_IN clientaddr;
			char buf[1024];
			int addrlen = sizeof(clientaddr);
			int retval = recvfrom(g_hSocket, buf, BUFSIZE,
				0, (SOCKADDR *)&clientaddr, &addrlen);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
				}
				break;
			}

			g_Queue.OnPutData(buf, retval);
			PACKETHEADER *pHeader = g_Queue.GetPacket();
			while (pHeader != NULL)
			{
				switch (pHeader->PktID)
				{
					//case : break;
				case PKT_JOIN:
				{
					MessageBox(hWnd, "�����ߴ�.", "JOIN P2", MB_OK);
					g_state = READY;

					GAMEDATA_MOVE * packet = (GAMEDATA_MOVE *)buf;

					g_enemyAir = (AirObject *)g_control.CreateObject("enemyAir", 5, 323, 67, 49, TYPE::AIR);
					g_enemyAir->SetPosition(packet->PosX, 10);

					if (packet->state == IDEL) { //��Ŷ�� ���� ���°� ���� �ƴ� ���
						packet->PktID = PKT_JOIN;
						packet->PktSize = sizeof(GAMEDATA_MOVE);
						float x = 0;
						float y = 0;
						g_myAir->GetPosition(&x, &y);
						packet->PosX = x;
						packet->state = g_state;
						Send(g_hSocket, buf, packet->PktSize);
					}
					else {
						g_state = GAME;
						PACKETHEADER * packet = (PACKETHEADER *)buf;
						packet->PktID = PKT_START;
						packet->PktSize = sizeof(PACKETHEADER);
						packet->state = g_state;
						Send(g_hSocket, buf, packet->PktSize);
					}

				}
				break;

				case PKT_START:
				{
					g_state = GAME;
				}
					break;

				case PKT_GAME_MOVE: 
				{
					GAMEDATA_MOVE * packet = (GAMEDATA_MOVE *)buf;
					g_enemyAir->SetPosition(packet->PosX, 10);
				}
				break;
				case PKT_GAME_FIRE:
				{
					GAMEDATA_BULLET * packet = (GAMEDATA_BULLET *)buf;
					string name = "bullet" + g_createBullet;
					BulletObject * obj = (BulletObject *)g_control.CreateObject(name, 75, 90, 4, 4, TYPE::BULLET);
					obj->SetPosition(packet->createBulletPosX, 10); // �̻���??
					obj->SetIsMyBullet(false);
					bullets.push_back(obj);
					g_createBullet++;

				}
					break;
				}
				g_Queue.OnPopData(pHeader->PktSize);

				pHeader = g_Queue.GetPacket();
			}

		}
		break;
		case FD_CLOSE:
		{
			closesocket(wParam);
		}
		break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
	}
	break;
	case WM_CREATE:
	{
		WSADATA wsaData;
		WORD	wVersionRequested;
		int     bOK;

		// ���� �ʱ�ȭ
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return -1;

		// socket()
		g_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (g_hSocket == INVALID_SOCKET)
		{

		}

		// bind()
		SOCKADDR_IN serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(8000);
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		bOK = bind(g_hSocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (bOK == SOCKET_ERROR)
		{

		}

		// ���� �ּ� ����ü �ʱ�ȭ		
		ZeroMemory(&g_toHost, sizeof(g_toHost));
		g_toHost.sin_family = AF_INET;
		g_toHost.sin_port = htons(9000);
		g_toHost.sin_addr.s_addr = inet_addr("127.0.0.1");


		if (WSAAsyncSelect(g_hSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) != 0)
		{
			OutputDebugString("WSAAsyncSelect Error.......\n");

			closesocket(g_hSocket);
		}
		SetWindowText(hWnd, "P2");
	
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
		closesocket(g_hSocket); g_hSocket = NULL;
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void OnIdle(float deltaTime)
{
	DWORD	tickCount;

	switch (g_state)
	{
	case IDEL:
		break;
	case READY:
		break;
	case GAME:
		Game(deltaTime);
		break;
	case RESULT:
		break;
	default:
		break;
	}
	
	g_OpenGL.BeginRender();
	g_control.DrawImage(deltaTime);
	g_OpenGL.EndRender(g_hDC);

}

void Game(float deltaTime) {
	BYTE key[256];
	::GetKeyboardState(key);

	if (!bullets.empty()) {
		vector<BulletObject*>::iterator iter = bullets.begin();
		float x, y;
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->GetPosition(&x, &y);

			RECT rc;
			if (IntersectRect(&rc, &(bullets[i]->GetRect()), &(g_myAir->GetRect()))) {
				g_myAir->SetDie();
			}

			if (y <= 0 || y>=768) //�� �� ����Ⱑ �浹�� ������...
			{
				string name = bullets[i]->GetName();
				bullets.erase(iter + i);
				g_control.DeleteObject(name);
			}
		}
	}

	float xPos = 0.0f, yPos = 0.0f;

	if (key[VK_LEFT] & 0x80)
	{
		xPos = -1.0f;
	}
	if (key[VK_RIGHT] & 0x80)
	{
		xPos = 1.0f;
	}
	if (key[VK_ADD] & 0x80)
	{
		g_myAir->ChangeSpeed(10.0f);
	}
	if (key[VK_SUBTRACT] & 0x80)
	{
		g_myAir->ChangeSpeed(-10.0f);
	}
	if (key[VK_SPACE] & 0x80) {

		if (g_bulletCollTime <= 0) {
			OutputDebugString("��");
			string name = "bullet" + g_createBullet;
			BulletObject * obj = (BulletObject *)g_control.CreateObject(name, 75, 90, 4, 4, TYPE::BULLET);
			float x, y;
			g_myAir->GetPosition(&x, &y);
			obj->SetPosition(x + (g_myAir->GetWidth() / 4), y); // �̻���??
			obj->SetIsMyBullet(true);
			g_createBulletPosX = x + (g_myAir->GetWidth() / 4);

			bullets.push_back(obj);
			g_createBullet++;
			g_bulletCollTime = COOLTIME;

			char buf[1024];

			GAMEDATA_BULLET *pHeader = (GAMEDATA_BULLET *)buf;
			pHeader->PktID = PKT_GAME_FIRE;
			pHeader->PktSize = sizeof(GAMEDATA_BULLET);
			pHeader->createBulletPosX = g_createBulletPosX;
			Send(g_hSocket, buf, pHeader->PktSize);

			OutputDebugString("��");
		}
	}

	if (!bullets.empty()) {
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->Move(deltaTime);
		}
	}

	g_myAir->Move(xPos, yPos, deltaTime);

	if (xPos != 0.0f) {
		char buf[1024];
		GAMEDATA_MOVE *pHeader = (GAMEDATA_MOVE *)buf;
		pHeader->PktID = PKT_GAME_MOVE;
		pHeader->PktSize = sizeof(GAMEDATA_MOVE);

		float x = 0;
		float y = 0;
		g_myAir->GetPosition(&x, &y);
		pHeader->PosX = x;
		Send(g_hSocket, buf, pHeader->PktSize);
	}
	

}

