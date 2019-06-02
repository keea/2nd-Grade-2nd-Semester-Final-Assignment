// testNonBlock.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <winsock2.h>
#include "packet.h"
#include "CirQueue.h"
#include <time.h>
#include <map>

#define MAX_LOADSTRING 100

//해쉬맵 테스트
std::map<std::string, GAMETEXT> g_testHast;

CCirQueue g_Queue;

LONG			 g_OldEditFunc;
char              g_myStrID[20];
DWORD       g_myID;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void Send(char *data, int size);
void Draw(HWND hWnd, PAINTSTRUCT * ps);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTNONBLOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TESTNONBLOCK);

	// Main message loop:
	while (1) 
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{

		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TESTNONBLOCK);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TESTNONBLOCK;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
SOCKET g_hSocket;
#define WM_SOCKET   WM_USER+1

#include <stdio.h>
// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//printf("[오류] %s", (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
	case WM_SOCKET:
		{
			// 오류 발생 여부 확인
			if(WSAGETSELECTERROR(lParam))
			{
				err_display(WSAGETSELECTERROR(lParam));
				break;
			}

			// 메시지 처리
			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_READ:
				{
					#define BUFSIZE  1024

					char buf[1024];
					int ret = recv(wParam, buf, BUFSIZE, 0);
					if(ret == SOCKET_ERROR)
					{
						if(WSAGetLastError() != WSAEWOULDBLOCK)
						{							
						}
						break;
					}

					g_Queue.OnPutData(buf, ret);
					PACKETHEADER *pHeader = g_Queue.GetPacket();
					while (pHeader != NULL)
					{
						switch (pHeader->PktID)
						{
							case PKT_RPOINT:
							{
								RPOINT *pPoint = (RPOINT *)pHeader;
								HDC hDC = GetDC(hWnd);
								SetPixel(hDC, pPoint->x, pPoint->y, RGB(255, 0, 0));
								ReleaseDC(hWnd, hDC);
							}
							break;
							case PKT_REQ_LOGIN:
							{
								LOGIN *pLogin = (LOGIN *)pHeader;
								int x = rand() % 2000;
								int y = rand() % 1000;

								GAMETEXT temp;
								strcpy(temp.gameText, pHeader->userStrID);
								temp.posX = rand() % 500;
								temp.posY = rand() % 500;
								g_testHast[std::string(temp.gameText)] = temp;

								//HDC hDC = GetDC(hWnd);
								//TextOut(hDC, x, y, pLogin->userStrID, strlen(pLogin->userStrID));
								//ReleaseDC(hWnd, hDC);

								InvalidateRect(hWnd, NULL, FALSE);

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
	case WM_CREATE:
		{
			WSADATA wsaData;
			WORD	wVersionRequested;
			int     bOK;

			srand(time(NULL));

			wVersionRequested = MAKEWORD( 2, 2 );
			
			bOK = WSAStartup(wVersionRequested,&wsaData);

			g_hSocket = socket(AF_INET, SOCK_STREAM, 0);

			sockaddr_in addr;

			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			addr.sin_port = htons(30000);

			if (connect(g_hSocket, (struct sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) 
			{
				return FALSE;
			}

			if(WSAAsyncSelect(g_hSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) != 0)
			{
				OutputDebugString("WSAAsyncSelect Error.......\n");

				closesocket(g_hSocket);				
			}
		}
		break;		
		case	 WM_RBUTTONDOWN:
			{
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				RPOINT pt;
				pt.PktID = PKT_RPOINT;
				pt.PktSize = sizeof(RPOINT);
				pt.x = x;
				pt.y = y;

				Send((char *)&pt, pt.PktSize);
			}
			break;
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN)
				{
					HWND hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER , 100, 100, 200, 25, hWnd, NULL, hInst, NULL);
					g_OldEditFunc = SetWindowLong(hEdit, GWL_WNDPROC, (LONG)EditFunction);
					SetFocus(hEdit);
				}

			}
			break;
		case WM_PAINT:
		{
			Draw(hWnd, &ps);
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);

			closesocket(g_hSocket);	
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


LRESULT CALLBACK EditFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			GetWindowText(hDlg, g_myStrID, 20);
			SetWindowLong(hDlg, GWL_WNDPROC, (LONG)g_OldEditFunc);
			DestroyWindow(hDlg);

			LOGIN login;
			login.PktID = PKT_REQ_LOGIN;
			login.PktSize = sizeof(LOGIN);
			strcpy(login.userStrID, g_myStrID);
			Send((char *)&login, login.PktSize);
		}
	}
	return TRUE;		
	}

	return ((WNDPROC)g_OldEditFunc)(hDlg, message, wParam, lParam);
}

void Send(char *data, int size)
{
	int ret = send(g_hSocket, data, size, 0);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::MessageBox(NULL, "send failed!", "error", MB_OK);
		}
	}
}

void Draw(HWND hWnd, PAINTSTRUCT * ps) {
	static HDC hdc, MemDC;
	static HBITMAP BackBit, oldBackBit;
	static RECT bufferRT;
	MemDC = BeginPaint(hWnd, ps);

	GetClientRect(hWnd, &bufferRT);
	hdc = CreateCompatibleDC(MemDC);
	BackBit = CreateCompatibleBitmap(MemDC, bufferRT.right, bufferRT.bottom);
	oldBackBit = (HBITMAP)SelectObject(hdc, BackBit);
	PatBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
	
	//여기에 그리기 코드
	for (auto it = g_testHast.begin(); it != g_testHast.end(); it++) {
		
		TextOut(hdc, it->second.posX, it->second.posY, (it->first).c_str(), strlen((it->first).c_str()));
		//DrawText(hdc, (it->first).c_str(), strlen((it->first).c_str()), &bufferRT, DT_SINGLELINE | DT_VCENTER);
	}


	/** 더블버퍼링 끝처리 입니다. **/
	GetClientRect(hWnd, &bufferRT);
	BitBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, hdc, 0, 0, SRCCOPY);
	SelectObject(hdc, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(hdc);
	EndPaint(hWnd, ps);
}