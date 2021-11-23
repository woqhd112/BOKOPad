// TimelineDetail.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "TimelineDetail.h"
#include "afxdialogex.h"


// TimelineDetail 대화 상자

IMPLEMENT_DYNAMIC(TimelineDetail, CDialogEx)

TimelineDetail::TimelineDetail(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE_DETAIL, pParent)
	, DlgInterface(this, false)
{
	Log_Manager->OnPutLog("TimelineDetail 생성자 호출", LogType::LT_PROCESS);
	CreateFrame();
}

TimelineDetail::~TimelineDetail()
{
	Log_Manager->OnPutLog("TimelineDetail 소멸자 호출", LogType::LT_PROCESS);
}

void TimelineDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIMELINE_DETAIL_THIS, m_edit_detail_this);
	DDX_Control(pDX, IDC_EDIT_TIMELINE_DETAIL_TARGET, m_edit_detail_target);
}


BEGIN_MESSAGE_MAP(TimelineDetail, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// TimelineDetail 메시지 처리기


BOOL TimelineDetail::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void TimelineDetail::Initialize()
{
	m_edit_detail_this.MoveWindow(20, 20, 170, 150);
	m_edit_detail_target.MoveWindow(200, 20, 170, 150);

	InitFrame();
}

void TimelineDetail::SetText(ComplexString thisText, ComplexString targetText)
{
	m_edit_detail_this.SetWindowTextA(thisText.GetBuffer());
	m_edit_detail_target.SetWindowTextA(targetText.GetBuffer());
	Log_Manager->OnPutLog("타임라인 상세화면 출력 완료", LogType::LT_EVENT);
}

HBRUSH TimelineDetail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == m_edit_detail_this.GetDlgCtrlID() ||
			pWnd->GetDlgCtrlID() == m_edit_detail_target.GetDlgCtrlID())
		{
			pDC->SetTextColor(DI_BLACK_COLOR);
			pDC->SetBkColor(DI_EDIT_COLOR);
			return m_editBrush;
		}
	}
	
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void TimelineDetail::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


bool TimelineDetail::DragDown(MSG* pMsg)
{
	return false;
}

bool TimelineDetail::DragMove(MSG* pMsg)
{
	return false;
}

bool TimelineDetail::DragUp(MSG* pMsg)
{
	return false;
}