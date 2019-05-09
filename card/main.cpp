#include "MyImage.h"
#include "dsSpriteManager.h"
#include "dsOpenALSoundManager.h"
#include "ImageManager.h"
#include "CardImageManager.h"
#include "CardControl.h"
#include "NumImageManager.h"
#include "GameTimeControl.h"
#include "MainControl.h"
#include "ScoreControl.h"
#include "ResultControl.h"
#include <ctime>

Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;

void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);
void GameInit();
void GameUpdate(DWORD tick);
bool IsSameScene(int * currentScene, int * beforeScene);

//�̹��� �Ŵ���
ImageManager * g_pImageManager[IMAGE_MANAGER_NUM];

//ī�� ����
CardControl gCardControl;

//���� ����
MainControl gMainControl;

//��� ����
ResultControl gResultControl;

//Ÿ�̸� ����
GameTimeControl gGameTimeControl;

//���� ����
ScoreControl gScoreControl;

int gCurrentScene = SCENE::MAIN; //���� ��
int gBeforeScene; //���� ��

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,     int nCmdShow)
{
	srand((unsigned int)time(NULL));
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = "HelloWin";
	
	if(RegisterClass(&wndclass) == 0)
	{
		return 0;		
	}

	HWND hwnd = CreateWindow("HelloWin", "Card Game", 
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 
							  1204, 768,
                              NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);

	CreateBuffer(hwnd, hDC);

	dsSpriteManager *pSpriteManager = GetSpriteManager();

	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();

	g_pImageManager[CARD] = new CardImageManager(pSpriteManager);
	g_pImageManager[NUMBER] = new NumImageManager(pSpriteManager);

	for (int i = 0; i < IMAGE_MANAGER_NUM; i++) {
		g_pImageManager[i]->InitImage();
	}

	gCardControl.Init(g_pImageManager[CARD]);
	gMainControl.Init(g_pImageManager[CARD]);
	gResultControl.Init(g_pImageManager[CARD]);
	
	gBeforeScene = 999;

	//dsSound *pSound = pSoundManger->LoadSound("test.wav", true);
	//if (pSound)
	//{
	//	pSound->Play();
	//}

	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

	MSG msg;
	DWORD tick = GetTickCount();
	while(1)
	{	
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
		{		
			DWORD curTick = GetTickCount();
			OnUpdate(hwnd, curTick - tick);
			tick = curTick;

			g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
		}
	}

	ReleaseBuffer(hwnd, hDC);

	return 0;
}

//�ʱ�ȭ �Լ�
void GameInit() {
	//ī�� ��Ʈ�� ����
	gCardControl.CreateCard();
	gScoreControl.Init();
	gGameTimeControl.Init();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static RECT rt;

	switch(msg)
	{
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (gCurrentScene == MAIN)
			gMainControl.UpperMouseEvent(x, y);
		else if(gCurrentScene == RESULT)
			gResultControl.UpperMouseEvent(x, y);
	}
	break;
	case WM_LBUTTONDOWN:
		{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		switch (gCurrentScene)
		{
		case SCENE::MAIN:
			gMainControl.ClickEvent(x, y, &gCurrentScene);
			break;
		case SCENE::GAME:
			gCardControl.ClickEvent(x, y, &gScoreControl);
			break;
		case SCENE::RESULT:
			gResultControl.ClickEvent(x, y, &gCurrentScene);
			break;
		default:
			break;
		}
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CreateBuffer(HWND hWnd, HDC hDC)
{
	GdiplusStartupInput			gdiplusStartupInput;
	ULONG_PTR					gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	RECT rc;
	GetClientRect(hWnd, &rc);

	g_Bitmap = new Bitmap(rc.right - rc.left, rc.bottom - rc.top);
	g_BackBuffer = new Graphics(g_Bitmap);
	g_BackBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
	
	g_MainBuffer = new Graphics(hDC);
	g_MainBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
}

void ReleaseBuffer(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);

	delete g_Bitmap;
	delete g_BackBuffer;
	delete g_MainBuffer;
}

void GameUpdate(DWORD tick) {
	bool isGameClear = false; //ī�带 �� ã�Ҵ��� �Ǵ��Ѵ�.
	bool isTimeOver = false; //�ð� �ʰ����� �Ǵ��Ѵ�.

	gCardControl.DisplayCard(g_BackBuffer);
	gGameTimeControl.DisplayTimer(g_BackBuffer, g_pImageManager[NUMBER]->GetImages());

	gScoreControl.DisplayScore(g_BackBuffer, g_pImageManager[NUMBER]->GetImages(),
		g_pImageManager[NUMBER]->GetCreateImage("combo.png"));
	gCardControl.CardsUpdate(tick, &gScoreControl);

	gGameTimeControl.TimerUpdate(tick, &isTimeOver);
	gCardControl.ChangeScene(&isGameClear);

	if (isTimeOver || isGameClear) {
		//Ÿ�ӿ��� �ϸ�(1) lose(1), �ƴϸ� win(0)
		gResultControl.SetResultType(isTimeOver);
		gCurrentScene = RESULT;
	}
		

}

void OnUpdate(HWND hWnd, DWORD tick)
{
	Color color(255, 255, 255);
	g_BackBuffer->Clear(color);

	if(hWnd == NULL)
		return;

	switch (gCurrentScene)
	{
	case SCENE::MAIN:
		IsSameScene(&gCurrentScene, &gBeforeScene);
		gMainControl.Display(g_BackBuffer);
		break;

	case SCENE::GAME:
		if (!IsSameScene(&gCurrentScene, &gBeforeScene)) {
			GameInit();
		}
		GameUpdate(tick);
		break;

	case SCENE::RESULT:
		IsSameScene(&gCurrentScene, &gBeforeScene);
		gResultControl.Display(g_BackBuffer);
		break;
	default:
		break;
	}
}

bool IsSameScene(int * currentScene, int * beforeScene) {
	if (*beforeScene != *currentScene) {
		*beforeScene = *currentScene;
		return false;
	}
	else
		return true;
}