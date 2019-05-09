#include <iostream>
#include <ctime>
#include "MyImage.h"
#include "dsSpriteManager.h"
#include "dsOpenALSoundManager.h"
#include "Keyboard.h"
#include "Node.h"
#include "BEAN.h"
#include "Utility.h"
#include "ControlWord.h"
#include "ScoreManager.h"

int g_currentScene;
Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;
Keyboard g_keyboard;
Node g_node;

DWORD ani_speed;
DWORD ani_interval;

ControlWord * g_ctrlWord;
MixManager * g_mixManager;
ScoreManager g_scoreManager;

bool isAddScore;
bool is_click_keyboard;
void init();
void ResultScene(HWND hWnd, DWORD tick);
void GameScene(HWND hWnd, DWORD tick);
void MainScene(HWND hWnd, DWORD tick);
void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);

dsSound *pSound[SOUND::S_NUM];

// 할 것.
// 점 수 (완료)
// 시간 제한 (1분) - 완료
// 게임 시작 (완료)
// 게임 결과창 (완료)

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void SetClientSize(HWND hWnd, int nDestClientWidth = 414, int nDestClientHeight = 768);

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

	RECT rc = { 0,0,414,736 };
	::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow("HelloWin", "Game Window!!", 
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 
							  rc.right - rc.left,
							  rc.bottom - rc.top,
                              NULL, NULL, hInstance, NULL);

	SetClientSize(hwnd);

	if(hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);

	CreateBuffer(hwnd, hDC);

	dsSpriteManager *pSpriteManager = GetSpriteManager();

	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	pSound[S_GAME_BGM] = pSoundManger->LoadSound("100BPM.wav", true);
	pSound[S_CLICK] = pSoundManger->LoadSound("click.wav", false);
	pSound[S_BGM] = pSoundManger->LoadSound("default_bgm.wav", true);

	if (pSound[S_BGM])
	{
		pSound[S_BGM]->Play();
	}

	g_currentScene = SCENE::GAME_MAIN;
	init(); //초기화

	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


	ani_speed = 100;

	MSG msg;
	DWORD tick = GetTickCount();
	while(1)
	{	
		//윈도우 메세지가 있을경우 메세지를 처리한다.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //메세지가 없을 경우 게임 루프를 실행한다.
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

void init() {
	wstring * test = nullptr;
	int size = 0;
	test = FileIo::InputFile(&size);
	g_ctrlWord = new ControlWord(test, size);
	g_mixManager = new MixManager();

	g_scoreManager.Init();
	g_keyboard.Init();
	g_keyboard.SetQuest(g_ctrlWord->GetQuestNm());
	g_node.SetQuest(g_ctrlWord->GetQuestNm(), g_mixManager->getPos());
	g_node.Init();
	is_click_keyboard = false;
	isAddScore = false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		{
		if (g_currentScene == SCENE::GAME) {
			if (g_keyboard.getAbleClick()) {
				POINT p = { LOWORD(lParam) , HIWORD(lParam) };
				g_node.setClick(true);
				is_click_keyboard = g_keyboard.OnClick(p, g_node.getSortBox(g_keyboard.getStep()));
				isAddScore = true;
			}
		}
		else if (g_currentScene == SCENE::GAME_RESULT) {
			g_currentScene = SCENE::GAME_MAIN;
			
		}
		else if (g_currentScene == SCENE::GAME_MAIN) {
			g_currentScene = SCENE::GAME;
			init();
		}
		}
		break;
	case WM_CREATE:
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

void OnUpdate(HWND hWnd, DWORD tick)
{
	if(hWnd == NULL)
		return;

	//Color color(255, 255, 255);
	g_BackBuffer->Clear(BACKGROUND_COLOR);

	if (g_currentScene == SCENE::GAME) {

		if (pSound[S_BGM]->isPlaying())
			pSound[S_BGM]->Stop();

		if (!pSound[S_GAME_BGM]->isPlaying()) {
			pSound[S_GAME_BGM]->Play();
		}
		GameScene(hWnd, tick);
	}
	else if (g_currentScene == SCENE::GAME_MAIN) {
		if (pSound[S_GAME_BGM]->isPlaying())
			pSound[S_GAME_BGM]->Stop();

		if (!pSound[S_BGM]->isPlaying()) {
			pSound[S_BGM]->Play();
		}

		MainScene(hWnd, tick);
	}
	else if (g_currentScene == SCENE::GAME_RESULT) {

		if (pSound[S_GAME_BGM]->isPlaying())
			pSound[S_GAME_BGM]->Stop();

		if (!pSound[S_BGM]->isPlaying()) {
			pSound[S_BGM]->Play();
		}

		ResultScene(hWnd, tick);
	}
}

void MainScene(HWND hWnd, DWORD tick) {
	Font F(FONT_CUTE, 32, FontStyleRegular, UnitPixel);
	StringFormat SF;
	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);
	RectF R(0,(736/2)-50, 414,50);

	SolidBrush B(H_DARK_YELLOW);
	g_BackBuffer->DrawString(L"클릭 후 게임 시작!!!", -1, &F, R, &SF, &B);
}

void ResultScene(HWND hWnd, DWORD tick) {
	Font F(FONT_CUTE, 32, FontStyleRegular, UnitPixel);
	StringFormat SF;
	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);
	SolidBrush B(H_PINK);

	wstring tag_nm[] = { L"SCORE : ", L"PERFECT : ", L"GREAT : ", L"COOL : ", L"MISS : ", L"COMBO : " };
	for (int i = 0; i < 6; i++) {
		RectF R(0, (45*i)+50, 414, 45*(i+1));
		wstring str = tag_nm[i];
		str = str + to_wstring(g_scoreManager.GetScore(i));
		g_BackBuffer->DrawString(str.c_str(), -1, &F, R, &SF, &B);
	}
}

void GameScene(HWND hWnd, DWORD tick) {
	bool isNewNode = false;
	bool isNext = g_node.getIsClick();

	g_keyboard.Draw(g_BackBuffer);
	int result = g_node.Update(g_BackBuffer, tick, &isNewNode);

	if (isNewNode) {
		g_mixManager->setRandNum(); //노드의 값을 랜덤으로 바꾼다.

		if (g_keyboard.getStep() > 2) {
			g_ctrlWord->NextQuest(); //문제 내용을 랜덤으로 바꾼다.
			g_keyboard.SetQuest(g_ctrlWord->GetQuestNm()); //다음 문제로 넘어가기.
			g_keyboard.ResetStep(); //다음 문제의 첫번째로 이동.
		}
		g_node.SetQuest(g_ctrlWord->GetQuestNm(), g_mixManager->getPos());
	}

	Font F(FONT_CUTE, 80, FontStyleRegular, UnitPixel);
	RectF R(0, 200, 414, 100);
	SolidBrush B(H_RESULT_YELLOW);
	StringFormat SF;

	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);

	if (g_keyboard.getIsAnswer()) { //정답일 경우.
		bool isPass = false;
		switch (g_node.getJud())
		{
		case JUDGMENT::PERFECT:
			g_BackBuffer->DrawString(L"PERFECT", -1, &F, R, &SF, &B);
			isPass = true;
			if (isAddScore) {
				g_scoreManager.AddScore(T_PERFECT);
				pSound[S_CLICK]->Play();
			}
				
			break;
		case JUDGMENT::GREAT:
			g_BackBuffer->DrawString(L"GREAT", -1, &F, R, &SF, &B);
			isPass = true;
			if (isAddScore) {
				g_scoreManager.AddScore(T_GREATE);
				pSound[S_CLICK]->Play();
			}
			break;
		case JUDGMENT::MISS: //제대로 눌렸으니까 점수라도 주자.
			g_BackBuffer->DrawString(L"COOL", -1, &F, R, &SF, &B);
			if (isAddScore)
				g_scoreManager.AddScore(T_COOL);
			break;
		}
	}// 제대로 입력하지 못했다면 MISS를 띄운다.
	else if (!g_keyboard.getAbleClick() && !g_keyboard.getIsAnswer()) {
		if(is_click_keyboard)
			g_BackBuffer->DrawString(L"MISS", -1, &F, R, &SF, &B);
		if (isAddScore) {
			g_scoreManager.AddScore(T_MISS);
			g_scoreManager.resetCombo();
		}	
	}

	//새로운 노드인데 클릭이 안된 상태
	if ( isNewNode) {
		is_click_keyboard = false;

		if(!g_keyboard.getAbleClick())
			g_keyboard.setAbleClick(true);
		else {
			g_scoreManager.AddScore(T_MISS);
			g_scoreManager.resetCombo();
		}
	}
	isAddScore = false;

	//점수 출력하자. (y 값 736)
	g_scoreManager.Draw(g_BackBuffer);

	if (result != SCENE::SCENE_NONE) {
		g_currentScene = result;
	}
}

void SetClientSize(HWND hWnd, int nDestClientWidth, int nDestClientHeight)
{
	::SetWindowPos(hWnd, NULL, 0, 0, nDestClientWidth, nDestClientHeight, SWP_NOMOVE);
	RECT stWindowRect;
	RECT stClientRect;
	GetWindowRect(hWnd, &stWindowRect);
	GetClientRect(hWnd, &stClientRect);

	int nClientWidth = stClientRect.right - stClientRect.left;
	int nClientHeight = stClientRect.bottom - stClientRect.top;
	int nWindowWidth = stWindowRect.right - stWindowRect.left;
	int nWindowHeight = stWindowRect.bottom - stWindowRect.top;

	nWindowWidth += nWindowWidth - nClientWidth;
	nWindowHeight += nWindowHeight - nClientHeight;

	//해상도 구하기
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

	//알맞게 변환된 윈도우를 화면 정중앙에 배치한다.
	::SetWindowPos(hWnd, NULL,
		nResolutionX / 2 - nWindowWidth / 2,
		nResolutionY / 2 - nWindowHeight / 2,
		nWindowWidth, nWindowHeight, NULL);
}