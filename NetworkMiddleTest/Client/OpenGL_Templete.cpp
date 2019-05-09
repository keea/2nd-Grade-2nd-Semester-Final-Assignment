// OpenGL_Templete.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <winsock2.h>
#include "global.h"
#include "dsTexture.h"
#include <time.h>
#include "dsOpenALSoundManager.h"
#include "CirQueue.h"
#include "FighterPlane.h"

SOCKET g_hSocket;
#define WM_SOCKET   WM_USER+1

CCirQueue g_Queue;

CFrameOpenGL  g_OpenGL;
HDC           g_hDC;

CFighterPlane g_myAir("myAir");

void OnIdle(float deltaTime);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

SOCKADDR_IN g_toHost;
// 데이터 보내기
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

	g_myAir.Create("./data/1942.png", 5, 323, 67, 49);

	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	dsSound *pSound = pSoundManger->LoadSound("back.wav", true);
	if (pSound)
	{
		pSound->Play();
	}	


	MSG msg;
	DWORD tick = GetTickCount();
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
			DWORD cur_tick = GetTickCount();
			float deltaTime = (float)(cur_tick - tick) / 1000.0f;
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
		// 오류 발생 여부 확인
		if (WSAGETSELECTERROR(lParam))
		{
			//err_display(WSAGETSELECTERROR(lParam));
			break;
		}

		// 메시지 처리
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
				case PKT_JOIN_P2:
				{
					MessageBox(hWnd, "참가했다.", "JOIN P2", MB_OK);
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

		// 윈속 초기화
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
		serveraddr.sin_port = htons(9000);
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		bOK = bind(g_hSocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (bOK == SOCKET_ERROR)
		{

		}

		// 소켓 주소 구조체 초기화		
		ZeroMemory(&g_toHost, sizeof(g_toHost));
		g_toHost.sin_family = AF_INET;
		g_toHost.sin_port = htons(8000);
		g_toHost.sin_addr.s_addr = inet_addr("127.0.0.1");


		if (WSAAsyncSelect(g_hSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) != 0)
		{
			OutputDebugString("WSAAsyncSelect Error.......\n");

			closesocket(g_hSocket);
		}
		SetWindowText(hWnd, "Server!!");

		//보낼 데이터 정의
		char buf[128];
		PACKETHEADER *pHeader = (PACKETHEADER *)buf;
		pHeader->PktID = PKT_JOIN_P1;
		pHeader->PktSize = sizeof(PACKETHEADER);

		Send(g_hSocket, buf, pHeader->PktSize);
	
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

	BYTE key[256];
	::GetKeyboardState(key);

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
		g_myAir.ChangeSpeed(10.0f);
	}
	if (key[VK_SUBTRACT] & 0x80)
	{
		g_myAir.ChangeSpeed(-10.0f);
	}
	g_myAir.Move(xPos, yPos, deltaTime);


	g_OpenGL.BeginRender();

	//g_myAir.Draw(200, 200, 0);
	g_myAir.DrawFighter(0);

	g_OpenGL.EndRender(g_hDC);
}

