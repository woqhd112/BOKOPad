// CustomScroll.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomScroll.h"
#include "afxdialogex.h"


// CustomScroll 대화 상자

IMPLEMENT_DYNAMIC(CustomScroll, CDialogEx)

CustomScroll::CustomScroll(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCROLL_BAR, pParent)
	, DlgInterface(this, false)
{
	CreateFrame();
	m_nStickMaxHeight = 0;
	m_nStartScrollPos = 0;
	m_nMoveMiddleLength = 0;
	m_nMiddlePointError = 0;
	m_nPrevMousePosY = 0;
	m_nStickEndPos = 0;
	m_nPos = 0;
	m_bMovingMouse = false;
	m_nHideScrollCount = 0;
	m_bScrollExactlyHideEvent = false;
}

CustomScroll::~CustomScroll()
{
}

void CustomScroll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_TOP, m_btn_top);
	DDX_Control(pDX, IDC_MFCBUTTON_BOTTOM, m_btn_bottom);
	DDX_Control(pDX, IDC_MFCBUTTON_MIDDLE, m_btn_middle);
}


BEGIN_MESSAGE_MAP(CustomScroll, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_TOP, &CustomScroll::OnBnClickedMfcbuttonTop)
	ON_BN_CLICKED(IDC_MFCBUTTON_BOTTOM, &CustomScroll::OnBnClickedMfcbuttonBottom)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CustomScroll 메시지 처리기


BOOL CustomScroll::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CustomScroll::SetExactlyHide(bool hide)
{
	m_bScrollExactlyHideEvent = hide;

	if (hide == true)
		this->ShowWindow(SW_HIDE);
	else 
		this->ShowWindow(SW_SHOW);
}

void CustomScroll::SetScrollInfo(ScrollInfo info, ScrollConnect con)
{
	m_con = con;
	Init(info);
}

void CustomScroll::Initialize()
{
	CRect pRect, tRect;
	GetParent()->GetClientRect(pRect);
	m_nStartScrollPos = pRect.right - SCROLL_SIZE_WIDTH;
	this->MoveWindow(m_nStartScrollPos, pRect.top, SCROLL_SIZE_WIDTH, pRect.Height());

	this->GetClientRect(tRect);

	m_btn_top.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, CBT_DEFAULT, CBE_CLICK, true);
	m_btn_bottom.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, CBT_DEFAULT, CBE_CLICK, true);
	m_btn_middle.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, CBT_DEFAULT, CBE_CLICK, true);

	m_nStickMaxHeight = tRect.Height() - (SCROLL_AND_BUTTON_MARGIN + BUTTON_SIZE + BUTTON_AND_STICK_MARGIN) * 2;

	m_btn_top.MoveWindow(SCROLL_AND_BUTTON_MARGIN, SCROLL_AND_BUTTON_MARGIN, BUTTON_SIZE, BUTTON_SIZE);
	m_btn_bottom.MoveWindow(SCROLL_AND_BUTTON_MARGIN, tRect.bottom - SCROLL_AND_BUTTON_MARGIN - BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE);
	m_btn_middle.MoveWindow(SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS, STICK_SIZE_WIDTH, m_nStickMaxHeight);
	//m_btn_middle.ShowWindow(SW_HIDE);

	m_btn_top.SetWindowTextA("∧");
	m_btn_bottom.SetWindowTextA("∨");
	m_btn_middle.SetWindowTextA("");

	InitFrame();

	ExecuteScroll(SCROLL_LINE_NOTHING);

	CRect rect;
	this->GetClientRect(rect);
	m_nStickEndPos = rect.Height() - SCROLL_AND_BUTTON_MARGIN - BUTTON_SIZE - BUTTON_AND_STICK_MARGIN;

	SetTimer(NULL, 2000, NULL);
}


void CustomScroll::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


BOOL CustomScroll::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == m_btn_middle.GetSafeHwnd())
		{
			if (DragDown(pMsg))
				return TRUE;
		}
		else if (pMsg->hwnd == this->GetSafeHwnd())
		{
			EmptySpaceClickEvent(pMsg);
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (DragUp(pMsg))
			return TRUE;
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (DragMove(pMsg))
			return FALSE;
	}
	return __super::PreTranslateMessage(pMsg);
}

void CustomScroll::EmptySpaceClickEvent(MSG* pMsg)
{
	CRect rect;
	m_btn_middle.GetWindowRect(rect);

	CPoint pt = pMsg->pt;

	// 막대바와 화살표버튼의 마진값보다 커야 클릭되게..
	if (rect.bottom < pt.y && (pt.y - rect.bottom) > BUTTON_AND_STICK_MARGIN)
	{
		// 바 아래 클릭 시
		GetParent()->SendMessageA(WM_VSCROLL, SB_PAGEDOWN, this->GetScrollPos(SB_VERT));
	}
	else if (rect.top > pt.y && rect.top - pt.y > BUTTON_AND_STICK_MARGIN)
	{
		// 바 위 클릭 시
		GetParent()->SendMessageA(WM_VSCROLL, SB_PAGEUP, this->GetScrollPos(SB_VERT));
	}
}

bool CustomScroll::OperateScroll(int nSBCode, int nPos)
{
	if (!m_bInit)
		return false;

	int delta = 0;
	if (nSBCode == SB_LINEUP || nSBCode == SB_PAGEUP)
	{
		delta = -m_nWheelSize;
		m_nPos -= m_nWheelSize;
	}
	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_PAGEDOWN)
	{
		delta = m_nWheelSize;
		m_nPos += m_nWheelSize;
	}
	else if (nSBCode == SB_THUMBTRACK)
	{
		/*if (nPos < m_prePos)
			delta = -m_nWheelSize;
		else
			delta = m_nWheelSize;*/
		//delta = nPos - m_nScrollPos;
	}
		//TRACE("pos : %d\n", nPos);
		//TRACE("delta : %d\n", delta);



	if (m_nPos < 0)
		m_nPos = 0;

	if (m_nScrollProcessCount < 0)
		m_nScrollProcessCount = 0;

	int scrollpos = m_nScrollPos + delta;
	int nMaxPos = m_nAllPageSize - m_nWheelSize;

	if (scrollpos < 0)
	{
		delta = -m_nScrollPos;
	}
	else
	{
		if (scrollpos > nMaxPos)
		{
			delta = nMaxPos - m_nScrollPos;
		}
	}

	m_prePos = nPos;

	if (delta != 0)
	{
		ProcessScrollCount(nSBCode);
		m_nScrollPos += delta;
		if (m_con == SC_CONNECT)
		{
			m_pProcessDlg->SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
			m_pProcessDlg->ScrollWindow(0, -delta);
		}
		ThisMoveWindow(m_nStartScrollPos, 0);
		m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS + (m_nMoveMiddleLength * (m_nScrollProcessCount)), 0, 0, SWP_NOSIZE);
		
		return true;
	}

	return false;
}

UINT CustomScroll::OperateWheel(short zDelta)
{

	UINT nFlag;
	if (zDelta > 0)	// 위스크롤
	{
		nFlag = SB_LINEUP;
		//m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS + (m_nMoveMiddleLength * (m_nScrollProcessCount)), 0, 0, SWP_NOSIZE);
	}
	else // 아래스크롤
	{
		nFlag = SB_LINEDOWN;
		//m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS + (m_nMoveMiddleLength * (m_nScrollProcessCount + 1)), 0, 0, SWP_NOSIZE);
	}


	return nFlag;
}

void CustomScroll::ExecuteScroll(int scrollLineFlag)
{
	ScrollProcess::ExecuteScroll(scrollLineFlag);

	if (scrollLineFlag == SCROLL_LINE_ADD)
	{
		m_nMoveMiddleLength = int(m_nStickMaxHeight / (m_nPageCount));
		m_btn_middle.SetWindowPos(NULL, 0, 0, STICK_SIZE_WIDTH, m_nMoveMiddleLength, SWP_NOMOVE);
	}
	else if (scrollLineFlag == SCROLL_LINE_DELETE)
	{
		if (m_nPageCount <= 0)
			return;

		m_nMoveMiddleLength = int(m_nStickMaxHeight / (m_nPageCount));
		m_btn_middle.SetWindowPos(NULL, 0, 0, STICK_SIZE_WIDTH, m_nMoveMiddleLength, SWP_NOMOVE);
	}
}

void CustomScroll::ThisMoveWindow(int x, int y)
{
	this->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);
}

bool CustomScroll::DragDown(MSG* pMsg)
{
	m_bDragProcessing = true;


	CRect rect;
	m_btn_middle.GetWindowRect(rect);
	m_nMiddlePointError = pMsg->pt.y - rect.top;

	CPoint pt = pMsg->pt;
	ScreenToClient(&pt);
	m_nPrevMousePosY = pt.y;

	return true;
}

bool CustomScroll::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	CPoint pt = pMsg->pt;
	ScreenToClient(&pt);

	CRect thisRect, middleRect;
	GetWindowRect(thisRect);
	m_btn_middle.GetWindowRect(middleRect);

	int nMiddleTop = middleRect.top - thisRect.top;

	// 스틱 이벤트 처리
	int nExecutePosY = 0;

	// 현재 계산된 스틱이 디폴트 시작위치보다 작을경우 (더 위에 있을 경우)는 처리안함
	if (nMiddleTop < STICK_DEFAULT_START_POS)
	{
		return false;
	}
	// 같을경우
	else if (nMiddleTop == STICK_DEFAULT_START_POS)
	{
		// 마우스 포인트가 위로 올라갈 경우 (스틱이 맨 위인데 마우스 포인트를 올릴경우임)
		if (m_nPrevMousePosY > pt.y)
			nExecutePosY = STICK_DEFAULT_START_POS;
		else // 마우스 포인트가 아래로 내려갈 경우
			nExecutePosY = pt.y - m_nMiddlePointError;
	}
	// 더 클경우
	else
	{
		nExecutePosY = pt.y - m_nMiddlePointError;
	}

	// 적용할 포지션이 디폴트 시작위치보다 작을경우는 그 값으로 맞춤
	if (nExecutePosY < STICK_DEFAULT_START_POS)
		nExecutePosY = STICK_DEFAULT_START_POS;
	else if (nExecutePosY + middleRect.Height() > m_nStickEndPos)
	{
		nExecutePosY = m_nStickEndPos - middleRect.Height();
	}

	m_nPos = nExecutePosY - STICK_DEFAULT_START_POS;
	if (m_nPos < 0)
		m_nPos = 0;

	//TRACE("pos : %d\n", m_nPos);

	m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, nExecutePosY, 0, 0, SWP_NOSIZE);

	// 잘 안먹힌다..
	// 나중에 구현하기
	//GetParent()->SendMessageA(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, SB_THUMBPOSITION), m_nPos);

	m_nPrevMousePosY = pt.y;

	return true;
}

bool CustomScroll::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	m_bDragProcessing = false;


	return true;
}


void CustomScroll::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


HBRUSH CustomScroll::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CustomScroll::OnBnClickedMfcbuttonTop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 스크롤 한줄 위로 (제일 상단일 땐 리턴)
	GetParent()->SendMessageA(WM_VSCROLL, SB_LINEUP, 0);

	//m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS + (m_nMoveMiddleLength * m_nScrollProcessCount), 0, 0, SWP_NOSIZE);
}


void CustomScroll::OnBnClickedMfcbuttonBottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 스크롤 한줄 아래로 (제일 하단일 땐 리턴)
	GetParent()->SendMessageA(WM_VSCROLL, SB_LINEDOWN, 0);

	//m_btn_middle.SetWindowPos(NULL, SCROLL_AND_STICK_MARGIN, STICK_DEFAULT_START_POS + (m_nMoveMiddleLength * m_nScrollProcessCount + 1), 0, 0, SWP_NOSIZE);
}

void CustomScroll::ShowScroll()
{
	if (m_bMovingMouse == false)
	{
		m_bMovingMouse = true;

		if (m_nPageCount > 0)
			this->ShowWindow(SW_SHOW);
	}
}

void CustomScroll::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	/*m_bMovingMouse = true;

	if (m_nPageCount > 0)
		this->ShowWindow(SW_SHOW);*/

	if (m_bScrollExactlyHideEvent)
	{
		this->ShowWindow(SW_HIDE);
		__super::OnTimer(nIDEvent);
		return;
	}

	CPoint pt;
	GetCursorPos(&pt);

	CRect rect;
	this->GetClientRect(rect);

	ScreenToClient(&pt);
	if (!PtInRect(rect, pt))
	{
		m_bMovingMouse = false;
		m_nHideScrollCount++;
	}
	else
	{
		m_bMovingMouse = true;

		if (m_nPageCount > 0)
			this->ShowWindow(SW_SHOW);
	}

	if (m_nHideScrollCount >= 2)
	{
		m_nHideScrollCount = 0;
		this->ShowWindow(SW_HIDE);
	}

	__super::OnTimer(nIDEvent);
}


ScrollProcess::ScrollProcess()
{
	m_nPageCount = 0;
	m_nWheelSize = 60;
	m_nAllPageSize = 0;
	m_nScrollPos = 0;
	m_nScrollProcessCount = 0;
	//m_nOnePageSize = 0;
	m_bInit = false;
	m_prePos = 0;
}

ScrollProcess::~ScrollProcess()
{

}

void ScrollProcess::Init(ScrollInfo& info)
{
	m_pProcessDlg = info.scrollExecuteCtrl;
	m_nWheelSize = info.wheelSize;
	m_bInit = true;
}

void ScrollProcess::ExecuteScroll(int scrollLineFlag)
{
	if (!m_bInit)
		return;

	if (scrollLineFlag == SCROLL_LINE_ADD)
	{
		m_nAllPageSize += m_nWheelSize;
		m_nPageCount++;
	}
	else if (scrollLineFlag == SCROLL_LINE_DELETE)
	{
		m_nAllPageSize -= m_nWheelSize;
		m_nPageCount--;
		if (m_nPageCount < 0)
		{
			m_nPageCount = 0;
			m_nAllPageSize = 0;
		}
		if (m_nScrollProcessCount > m_nPageCount)
			m_nScrollProcessCount = m_nPageCount;
	}

	int nScrollMax = 0;
	if (m_nWheelSize < m_nAllPageSize)
	{
		nScrollMax = m_nAllPageSize - 1;
		m_nScrollPos = min(m_nScrollPos, m_nAllPageSize - m_nWheelSize - 1);
	}


	/*SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = nScrollMax;
	si.nPos = m_nScrollPos;
	si.nPage = m_nWheelSize;
	m_pProcessDlg->SetScrollInfo(SB_VERT, &si, TRUE);*/
}

void ScrollProcess::ProcessScrollCount(int nSBCode)
{
	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nScrollProcessCount--;
		break;
	case SB_PAGEUP:
		m_nScrollProcessCount--;
		break;
	case SB_PAGEDOWN:
		m_nScrollProcessCount++;
		break;
	case SB_LINEDOWN:
		m_nScrollProcessCount++;
		break;
	default:
		break;
	}
}
//
//bool ScrollProcess::OperateScroll(int nSBCode, int nPos)
//{
//	if (!m_bInit)
//		return false;
//
//	int delta = 0;
//	if (nSBCode == SB_LINEUP || nSBCode == SB_PAGEUP)
//	{
//		delta = -m_nWheelSize;
//	}
//	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_PAGEDOWN)
//	{
//		delta = m_nWheelSize;
//	}
//	/*else if (nSBCode == SB_THUMBTRACK)
//	{
//		if (m_prePos < nPos && nPos % m_nWheelSize == 0)
//		{
//			delta = m_nWheelSize;
//			nSBCode = SB_PAGEDOWN;
//		}
//		else if (m_prePos > nPos && nPos % m_nWheelSize == 0)
//		{
//			delta = -m_nWheelSize;
//			nSBCode = SB_PAGEUP;
//		}
//	}
//
//	m_prePos = nPos;*/
//
//	if (m_nScrollProcessCount < 0)
//		m_nScrollProcessCount = 0;
//
//	int scrollpos = m_nScrollPos + delta;
//	int nMaxPos = m_nAllPageSize - m_nWheelSize;
//
//	if (scrollpos < 0)
//	{
//		delta = -m_nScrollPos;
//	}
//	else
//	{
//		if (scrollpos > nMaxPos)
//		{
//			delta = nMaxPos - m_nScrollPos;
//		}
//	}
//
//	if (delta != 0)
//	{
//		ProcessScrollCount(nSBCode);
//		m_nScrollPos += delta;
//		m_pProcessDlg->SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
//		m_pProcessDlg->ScrollWindow(0, -delta);
//
//		return true;
//	}
//
//	return false;
//}

//UINT ScrollProcess::OperateWheel(short zDelta)
//{
//
//	UINT nFlag;
//	if (zDelta > 0)	// 위스크롤
//	{
//		nFlag = SB_LINEUP;
//	}
//	else // 아래스크롤
//	{
//		nFlag = SB_LINEDOWN;
//	}
//
//	return nFlag;
//}

int ScrollProcess::GetLineCount() const
{
	return m_nPageCount;
}

int ScrollProcess::GetScrollCount() const
{
	return m_nScrollProcessCount;
}

int ScrollProcess::GetScrollSize() const
{
	return m_nScrollPos;
}

int ScrollProcess::GetWheelSize() const
{
	return m_nWheelSize;
}

void ScrollProcess::ResetScroll()
{
	m_nAllPageSize = 0;
	m_nScrollPos = 0;
	m_nPageCount = 0;
	m_nScrollProcessCount = 0;
}