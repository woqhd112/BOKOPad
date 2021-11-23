// CustomEditCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomEditCtrl.h"
#include "NoteListCtrl.h"
#include "afxdialogex.h"


// CustomEditCtrl 대화 상자

IMPLEMENT_DYNAMIC(CustomEditCtrl, CDialogEx)

CustomEditCtrl::CustomEditCtrl(int id, int notSEQ, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDIT, pParent)
	, DlgInterface(this, false)
{
	m_nEditID = id;
	m_nNotSEQ = notSEQ;
	CreateFrame();
}

CustomEditCtrl::~CustomEditCtrl()
{
	m_scroll.DestroyWindow();
	m_edit_custom.DestroyWindow();
}

void CustomEditCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CUSTOM, m_edit_custom);
}


BEGIN_MESSAGE_MAP(CustomEditCtrl, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CustomEditCtrl 메시지 처리기


BOOL CustomEditCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_edit_custom.SetNotSEQ(m_nNotSEQ);

	InitFrame();
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CustomEditCtrl::Initialize()
{
	//bool bResult = m_edit_custom.Create(WS_VISIBLE | WS_BORDER | ES_MULTILINE/* | ES_AUTOVSCROLL | WS_VSCROLL*/, CRect(0, 0, 0, 0), this, m_nEditID);
	m_scroll.Create(CustomScroll::IDD, this);
	m_edit_custom.ShowWindow(SW_SHOW);

	ScrollProcess::ScrollInfo info;
	info.scrollExecuteCtrl = this;
	info.wheelSize = 10;
	m_scroll.SetScrollInfo(info, SC_DISCONNECT);
	m_scroll.ShowWindow(SW_HIDE);
}

void CustomEditCtrl::MoveWindows(int x, int y, int cx, int cy)
{
	this->MoveWindow(x, y, cx, cy);
	m_edit_custom.MoveWindow(0, 0, cx - 20, cy);
}

void CustomEditCtrl::SetFont(CFont* pFont, BOOL bRedraw)
{
	m_edit_custom.SetFont(pFont, bRedraw);
}

void CustomEditCtrl::ExecuteTimer()
{
	m_edit_custom.ExecuteTimer();
}

void CustomEditCtrl::SetWindowTexts(LPCTSTR lpszString)
{
	m_edit_custom.SetWindowTextA(lpszString);
	m_edit_custom.ScrollSync();
}

void CustomEditCtrl::GetWindowTexts(CString& lpszString)
{
	m_edit_custom.GetWindowTextA(lpszString);
}

bool CustomEditCtrl::DragDown(MSG* pMsg)
{
	return false;
}

bool CustomEditCtrl::DragMove(MSG* pMsg)
{
	return false;
}

bool CustomEditCtrl::DragUp(MSG* pMsg)
{
	return false;
}

void CustomEditCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


HBRUSH CustomEditCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(DI_BLACK_COLOR);
		pDC->SetBkColor(DI_EDIT_COLOR);
		return m_editBrush;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CustomEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	OnVScroll(m_scroll.OperateWheel(zDelta), 0, GetScrollBarCtrl(SB_VERT));

	return TRUE;
	//return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void CustomEditCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_scroll.OperateScroll(nSBCode, nPos);
	m_edit_custom.SetScrollEditPos(nSBCode);

	//__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CustomEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		if (pMsg->hwnd != m_edit_custom.GetSafeHwnd())
		{
			NoteListCtrl* gParent = (NoteListCtrl*)GetParent();
			if (gParent->bMainScrollFocus == false)
			{
				SendMessageA(pMsg->message, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		CRect rect;
		m_scroll.GetWindowRect(rect);
		if (PtInRect(rect, pMsg->pt))
		{
			m_scroll.ShowScroll();
		}
	}

	return __super::PreTranslateMessage(pMsg);
}
