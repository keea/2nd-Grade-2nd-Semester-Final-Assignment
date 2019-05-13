// OpenGL_Templete.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
// 할 것 : 게임 오버 처리

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

void OnIdle(float deltaTime, HWND hwnd);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

SOCKADDR_IN g_toHost;
GAMESTATE g_state;
vector<BulletObject * > bullets;
int g_createBullet = 0;
float g_bulletCollTime = 0;

#define DEFAULT_POS_X 250
#define DEFAULT_POS_Y 350
#define COOLTIME	1000

void Game(float deltaTime);
void Die(float deltaTime, HWND hwnd);
void Init();

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

	//g_myAir.Create("./data/1942.png", 5, 323, 67, 49);

	

	g_control.Create("./data/1942.png");
	g_myAir = (AirObject *)g_control.CreateObject("myAir", 5, 323, 67, 49, TYPE::AIR);
	g_myAir->SetPosition(DEFAULT_POS_X, DEFAULT_POS_Y);

	//보낼 데이터 정의
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
			float deltaTime = (float)(cur_tick - tick);
			g_bulletCollTime -= deltaTime;
			OnIdle(deltaTime, hwnd);

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
				case PKT_JOIN:
				{
					MessageBox(hWnd, "참가했다.", "JOIN P2", MB_OK);
					g_state = READY;

					GAMEDATA_MOVE * packet = (GAMEDATA_MOVE *)buf;

					g_enemyAir = (AirObject *)g_control.CreateObject("enemyAir", 5, 323, 67, 49, TYPE::AIR);
					g_enemyAir->SetPosition(packet->PosX, 10);

					if (packet->state == IDEL) { //패킷의 게임 상태가 레디가 아닌 경우
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
					if (g_state == GAME) {
						GAMEDATA_BULLET * packet = (GAMEDATA_BULLET *)buf;
						string name = "bullet" + g_createBullet;
						BulletObject * obj = (BulletObject *)g_control.CreateObject(name, 75, 90, 4, 4, TYPE::BULLET);
						obj->SetPosition(packet->createBulletPosX, 10);
						obj->SetIsMyBullet(false);
						bullets.push_back(obj);
						g_createBullet++;
					}
				}
					break;

				case PKT_GAME_OVER:
				{
					if (g_state == DIE) { //내가 게임 오버를 알리고 상대가 응답해주는 경우
						//게임 오버 안내 띄우기.
						if (IDYES == MessageBox(NULL, TEXT("패배하셨습니다. 계속 진행하시겠습니까?"), TEXT("패배"), MB_YESNO | MB_ICONERROR)) {
							Init();
							//보낼 데이터 정의
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
						}
						else {
							Init();
							SendMessage(hWnd, WM_CLOSE, 0, 0); //윈도우 종료.
						}
					}
					else { // 상대가 죽었다는 것을 받는 경우.
						g_state = DIE; //die로 변경하여, 움직이게 못함.
						g_enemyAir->SetDie(); //그리고 죽음 처리(?)
					}
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
		serveraddr.sin_port = htons(8000);
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		bOK = bind(g_hSocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (bOK == SOCKET_ERROR)
		{

		}

		// 소켓 주소 구조체 초기화		
		ZeroMemory(&g_toHost, sizeof(g_toHost));
		g_toHost.sin_family = AF_INET;
		g_toHost.sin_port = htons(9000);
		g_toHost.sin_addr.s_addr = inet_addr("127.0.0.1");


		if (WSAAsyncSelect(g_hSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) != 0)
		{
			OutputDebugString("WSAAsyncSelect Error.......\n");

			closesocket(g_hSocket);
		}
		SetWindowText(hWnd, "P1");
	
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


void OnIdle(float deltaTime, HWND hwnd)
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
	case DIE:
		Die(deltaTime, hwnd);
		break;
	default:
		break;
	}
	
	g_OpenGL.BeginRender();
	g_control.DrawImage(deltaTime);
	g_OpenGL.EndRender(g_hDC);

}

void Die(float deltaTime, HWND hwnd) {
	if (g_myAir->GetIsEnd()) { //내가 죽었을 경우.
		//게임 오버 데이터 보내기.
		char buf[128];
		PACKETHEADER *pHeader = (PACKETHEADER *)buf;
		pHeader->PktID = PKT_GAME_OVER;
		pHeader->PktSize = sizeof(PACKETHEADER);
		pHeader->state = g_state; //죽음었다는 것을 보냄.
		Send(g_hSocket, buf, pHeader->PktSize);
	}

	if (g_enemyAir->GetIsEnd()) {
		//점수 증가.

		//승리 문구를 띄운다.
		MessageBox(hwnd, "승리하셨습니다.", "승리", MB_OK);

		char buf[128];
		PACKETHEADER *pHeader = (PACKETHEADER *)buf;
		pHeader->PktID = PKT_GAME_OVER;
		pHeader->PktSize = sizeof(PACKETHEADER);
		pHeader->state = g_state; //죽음었다는 것을 보냄.
		Send(g_hSocket, buf, pHeader->PktSize);

		//초기화 하기 : 총알, 게임 상태값. 오브젝트 상태값.
		Init(); 
	}
}

void Init()
{
	g_myAir->SetPosition(DEFAULT_POS_X, DEFAULT_POS_Y);
	g_state = IDEL;
	g_control.DeleteObject("enemyAir"); //적 오브젝트 제거


	while (!bullets.empty())
	{
		string name = bullets.back()->GetName();
		g_control.DeleteObject(name);
		bullets.pop_back();
	}

	g_createBullet = 0;

	//if (!bullets.empty()) {
	//	vector<BulletObject*>::iterator iter = bullets.begin();
	//	for (int i = 0; i < bullets.size();) {
	//		string name = bullets[i]->GetName();
	//		bullets.erase(iter + i);
	//		g_control.DeleteObject(name);
	//	}
	//	//bullets.clear();
	//	g_createBullet = 0;
	//}
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
			string str = "총알 y값 : "+to_string(bullets[i]->GetRect().top) + " 총알 x값 : " + to_string(bullets[i]->GetRect().left)+" 내 비행기 x값 : "+ to_string(g_myAir->GetRect().right);
			//250 ,  350
			str += "\n";

			const char * ch = str.c_str();
			
			OutputDebugString(ch);
			if (IntersectRect(&rc, &(bullets[i]->GetRect()), &(g_myAir->GetRect()))) {
				//자신의 캐릭터가 총알에 맞은 경우
				g_myAir->SetDie();
				g_state = DIE;
			}

			if (y <= 0 || y >= 768) //화면 밖을 나가는 경우
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
			OutputDebugString("값");
			string name = "bullet" + g_createBullet;
			BulletObject * obj = (BulletObject *)g_control.CreateObject(name, 75, 90, 4, 4, TYPE::BULLET);
			float x, y;
			g_myAir->GetPosition(&x, &y);
			obj->SetPosition(x + (g_myAir->GetWidth() / 4), y-10); // 이상함??
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

			OutputDebugString("끝");
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

