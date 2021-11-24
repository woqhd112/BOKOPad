// BOKOScenarioAddDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOScenarioAddDlg.h"
#include "afxdialogex.h"


// BOKOScenarioAddDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOScenarioAddDlg, CDialogEx)

BOKOScenarioAddDlg::BOKOScenarioAddDlg(ComplexString* inputText, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_ADD, pParent)
	, DlgInterface(this, true)
{
	m_strInputText = inputText;
	CreateFrame();
}

BOKOScenarioAddDlg::~BOKOScenarioAddDlg()
{
}

void BOKOScenarioAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_SCENARIO_ADD, m_btn_input_scenario);
	DDX_Control(pDX, IDC_STATIC_SCENARIO, m_stt_scenario);
}


BEGIN_MESSAGE_MAP(BOKOScenarioAddDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_SCENARIO_ADD, &BOKOScenarioAddDlg::OnBnClickedMfcbuttonScenarioAdd)
END_MESSAGE_MAP()


// BOKOScenarioAddDlg 메시지 처리기


BOOL BOKOScenarioAddDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();


	GotoDlgCtrl(&m_edit_input.m_edit_custom);

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void BOKOScenarioAddDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


void BOKOScenarioAddDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


HBRUSH BOKOScenarioAddDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetSafeHwnd() == m_stt_scenario.GetSafeHwnd())
		{
			pDC->SetBkMode(TRANSPARENT);
			CBrush B;
			B.CreateStockObject(NULL_BRUSH);
			return B;
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

bool BOKOScenarioAddDlg::DragDown(MSG* pMsg)
{
	return false;
}

bool BOKOScenarioAddDlg::DragMove(MSG* pMsg)
{
	return false;
}

bool BOKOScenarioAddDlg::DragUp(MSG* pMsg)
{
	return false;
}

void BOKOScenarioAddDlg::Initialize()
{
	this->SetWindowPos(NULL, 0, 0, 300, 160, SWP_NOMOVE);

	m_edit_input.Create(CustomEditCtrl::IDD, this);
	m_edit_input.MoveWindows(20, 60, 260, 40);
	m_edit_input.Initialize();
	m_edit_input.ShowWindow(SW_SHOW);
	m_edit_input.LimitText(20);
	m_btn_input_scenario.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);
	m_stt_scenario.MoveWindow(20, 40, 260, 20);
	m_btn_input_scenario.MoveWindow(230, 110, 50, 25);

	InitFrame("시나리오 추가");
}


BOOL BOKOScenarioAddDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}
	else if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedMfcbuttonScenarioAdd();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}


void BOKOScenarioAddDlg::OnBnClickedMfcbuttonScenarioAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString getText;
	m_edit_input.GetWindowTexts(getText);
	*m_strInputText = getText.GetBuffer();

	SendMessageA(WM_CLOSE);
}
