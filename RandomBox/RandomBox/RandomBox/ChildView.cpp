
// ChildView.cpp: CChildView 클래스의 구현
//

#include "stdafx.h"
#include "RandomBox.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
CChildView::CChildView()
{
	srand(time(NULL));

	cardDatas.SetSize(END);

	cardDatas[NOMAL] = CCardData(0, _T("N"), RGB(255, 255, 255)); //하얀
	cardDatas[RARE] = CCardData(0, _T("R"), RGB(102, 204, 102)); //초록
	cardDatas[SUPER_RARE] = CCardData(0, _T("SR"), RGB(153, 204, 204)); //파랑
	cardDatas[ULTRA_RARE] = CCardData(0, _T("UR"), RGB(255, 204, 0)); //노랑

	InitData();
	isDrawing = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_COMMAND(ID_INIT_INVEN, &CChildView::OnInitInven)
ON_WM_CONTEXTMENU()
ON_COMMAND(1001, &CChildView::OnColorYellow)
END_MESSAGE_MAP()



// CChildView 메시지 처리기
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	GetClientRect(&windowRect);
	DrawResult(&dc);

	if (!isDrawing) {
		DrawInfoPercentage(&dc);
		DrawButton(&dc);
	}else{
		DrawCardInfo(&dc);
	}
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//뽑기 클릭시
	if (!isDrawing && PtInRect(&btnRect, point)) {
		TRACE(_T("버튼 클릭했다.\r\n"));
		PickUp(10);
		isDrawing = true;
	}
	else if (isDrawing && PtInRect(&cardRect, point)) {
		TRACE(_T("버튼 갯수 : %d\r\n"), drawsCard.GetSize());

		if (drawsCard.GetSize() > 0) {
			int rating = drawsCard.GetAt(0);
			cardDatas[rating].AddCount();
			drawsCard.RemoveAt(0);
			cntInven += 1;
		}
		if (drawsCard.GetSize() <= 0)
			isDrawing = false;
	}

	Invalidate();
}

void CChildView::PickUp(int cnt)
{
	double p[] = { 1,4,10,85 };
	cardMax = cnt;
	for (int i = 0; i < cardMax; i++) {
		double r = rand() / (double)RAND_MAX; //rand 함수로 반환될 수 있는 수의 최대값 ( 0x7fff ) {0.0 ~ 1.0}
		double dr = r * 100.0f; //{0.0 ~ 100.0}
		double cumulative = 0.0f;

		for (int i = 0; i < END; i++) {
			cumulative += p[i];
			if (dr <= cumulative) {
				TRACE(_T("뽑은 값 : %f 확률 : %f\r\n"), dr, p[i]);
				int rating = (END - 1) - i;
				drawsCard.Add(rating);
				break;
			}
		}
	}
}

void CChildView::DrawButton(CPaintDC * dc)
{
	//버튼 크기(윈도우 사이즈가 변해도 비율 유지 위해.)
	btnRect.left = 100;
	btnRect.top = windowRect.bottom - 150;
	btnRect.right = (windowRect.right / 2) - 100;
	btnRect.bottom = windowRect.bottom - 100;

	//버튼 브러시
	CBrush btnBrush(RGB(255, 255, 0));
	dc->SelectObject(&btnBrush);
	dc->Rectangle(&btnRect);

	//폰트 작업.
	CFont font;
	//폰트 설정
	font.CreatePointFont(300, _T("궁서"));
	dc->SelectObject(&font);
	//폰트 색상 및 배경 설정.
	dc->SetBkColor(RGB(255, 255, 0));
	//글자 출력
	dc->DrawText(CString("10회 뽑기"), &btnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CChildView::DrawInfoPercentage(CPaintDC * dc)
{
	//확률 설명
	CRect textRect;
	textRect.left = 100;
	textRect.top = 150;
	textRect.right = (windowRect.right / 2) - 100;
	textRect.bottom = windowRect.bottom - 250;
	CFont font1;
	font1.CreatePointFont(300, _T("바탕"));
	dc->SelectObject(&font1);
	dc->DrawText(CString("확률\r\n\r\nN : 85%\r\nR : 10%\r\nSR : 4%\r\nUR : 1%"), &textRect, DT_CENTER | DT_VCENTER);
}

void CChildView::DrawResult(CPaintDC * dc)
{
	//인벤토리 창 출력
	CRect textRect;
	textRect.left = (windowRect.right / 2) + 100;
	textRect.top = 150;
	textRect.right = windowRect.right;
	textRect.bottom = windowRect.bottom - 100;

	CFont font1;
	font1.CreatePointFont(300, _T("바탕"));
	dc->SelectObject(&font1);
	CString text;
	text = _T("인벤토리\r\n\r\n");

	for (int i = 0; i < RATING::END; i++) {
		text.Format(_T("%s%s : %d개\r\n"), text, cardDatas[i].GetName(), cardDatas[i].GetCount());
	}
	text.Format(_T("%s\r\n=========\r\n전체 : %d개"), text, cntInven);
	dc->DrawText(text, &textRect, DT_CENTER | DT_VCENTER);
}

void CChildView::DrawCardInfo(CPaintDC * dc)
{
	cardRect.left = (windowRect.right / 4) - 74;
	cardRect.top = (windowRect.bottom / 2) - 105;
	cardRect.right = (windowRect.right / 4) + 74;
	cardRect.bottom = (windowRect.bottom / 2) + 105;

	//버튼 브러시
	CBrush btnBrush(cardDatas[drawsCard.GetAt(0)].GetColor());
	dc->SelectObject(&btnBrush);
	dc->Rectangle(&cardRect);

	//폰트 작업.
	CFont font;
	//폰트 설정
	font.CreatePointFont(300, _T("궁서"));
	dc->SelectObject(&font);
	//폰트 색상 및 배경 설정.
	dc->SetBkColor(cardDatas[drawsCard.GetAt(0)].GetColor());
	//글자 출력
	dc->DrawText(cardDatas[drawsCard.GetAt(0)].GetName(), &cardRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc->SetBkColor(RGB(255,255,255));
	CString count;
	count.Format(_T("%d / %d"), drawsCard.GetCount(), cardMax);

	dc->SetTextAlign(TA_RIGHT); //오른쪽 정렬
	dc->TextOutW(cardRect.right, cardRect.bottom+25, count);
}

void CChildView::InitData()
{
	cntInven = 0;
	for (int i = 0; i < RATING::END; i++) {
		cardDatas[i].SetInitCount();
	}
	
}

void CChildView::OnColorYellow()
{

}


void CChildView::OnInitInven()
{
	InitData();
	Invalidate();
}


void CChildView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenuW(IDR_MAINFRAME);
	CMenu * pMenu = menu.GetSubMenu(4);
	pMenu->AppendMenuW(MF_STRING, 1001, _T("노란색(&Y)"));
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}
