// BOKOProgressPopup.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOProgressPopup.h"
#include "afxdialogex.h"


// BOKOProgressPopup 대화 상자

IMPLEMENT_DYNAMIC(BOKOProgressPopup, CDialogEx)

BOKOProgressPopup::BOKOProgressPopup(bool* processing, int itemCount, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROGRESS_POPUP, pParent)
	, DlgInterface(this, true)
{
	m_nItemCount = itemCount;
	m_posCount = 0;
	m_bProcessing = processing;
	*m_bProcessing = false;
	CreateFrame();
}

BOKOProgressPopup::~BOKOProgressPopup()
{

}

void BOKOProgressPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_progress);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_FILE_NAME, m_stt_file_name);
}


BEGIN_MESSAGE_MAP(BOKOProgressPopup, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// BOKOProgressPopup 메시지 처리기


BOOL BOKOProgressPopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//m_progress.SetRange(m_nStartRange, m_nEndRange);
	
	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOProgressPopup::Initialize()
{
	m_progress.SetPos(0);
	*m_bProcessing = true;
	InitFrame("진행 중");
}

bool BOKOProgressPopup::AddProgressPos()
{
	m_progress.SetPos((++m_posCount) * 100 / m_nItemCount);

	if (m_posCount == m_nItemCount)
	{
		return true;
	}

	return false;
}

void BOKOProgressPopup::Success()
{
	m_progress.SetPos(100);
}

void BOKOProgressPopup::SetAnalyzeFileName(ComplexString strFileName)
{
	m_strCurrentFileName = strFileName;
	CString strText;
	strText.Format("%s...", strFileName.GetBuffer());
	m_stt_file_name.SetWindowTextA(strText);
}


void BOKOProgressPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.
	DrawFrame(&dc);
}


HBRUSH BOKOProgressPopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(DI_BK_COLOR);
		pDC->SetTextColor(DI_TEXT_COLOR);
		return m_staticBrush;
	}

	return hbr;
}


void BOKOProgressPopup::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


BOOL BOKOProgressPopup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}
	else if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_ESCAPE)
			OnClose();
	}

	return __super::PreTranslateMessage(pMsg);
}

void BOKOProgressPopup::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	*m_bProcessing = false;
	__super::OnClose();
}

bool BOKOProgressPopup::DragDown(MSG* pMsg)
{
	return false;
}

bool BOKOProgressPopup::DragMove(MSG* pMsg)
{
	return false;
}

bool BOKOProgressPopup::DragUp(MSG* pMsg)
{
	return false;
}