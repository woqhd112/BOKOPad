// BOKOSelectExtentionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOSelectExtentionDlg.h"
#include "afxdialogex.h"


// BOKOSelectExtentionDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOSelectExtentionDlg, CDialogEx)

BOKOSelectExtentionDlg::BOKOSelectExtentionDlg(int* selectExtentionType, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_EXPORT_TIMELINE_EXTENTION, pParent)
	, DlgInterface(this, true)
{
	m_nSelectExtentionType = selectExtentionType;
	Log_Manager->OnPutLog("BOKOSelectExtentionDlg 생성자 호출", LogType::LT_PROCESS);
	CreateFrame();
}

BOKOSelectExtentionDlg::~BOKOSelectExtentionDlg()
{
	Log_Manager->OnPutLog("BOKOSelectExtentionDlg 소멸자 호출", LogType::LT_PROCESS);
}

void BOKOSelectExtentionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SELECT_NOTEPAD, m_btn_notepad);
	DDX_Control(pDX, IDC_BUTTON_SELECT_WORD, m_btn_word);
	DDX_Control(pDX, IDC_BUTTON_SELECT_EXCEL, m_btn_excel);
	DDX_Control(pDX, IDC_MFCBUTTON_NOTEPAD, m_btn_notepad2);
	DDX_Control(pDX, IDC_MFCBUTTON_EXCEL, m_btn_excel2);
	DDX_Control(pDX, IDC_MFCBUTTON_WORD, m_btn_word2);
}


BEGIN_MESSAGE_MAP(BOKOSelectExtentionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BOKOSelectExtentionDlg 메시지 처리기


BOOL BOKOSelectExtentionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	this->SetBackgroundColor(RGB(255, 255, 255));

	m_btn_notepad.LoadStdImage(IDB_PNG_NOTEPAD, "PNG");
	m_btn_notepad.LoadHovImage(IDB_PNG_NOTEPAD, "PNG");
	m_btn_notepad.LoadAltImage(IDB_PNG_NOTEPAD, "PNG");

	m_btn_excel.LoadStdImage(IDB_PNG_EXCEL, "PNG");
	m_btn_excel.LoadHovImage(IDB_PNG_EXCEL, "PNG");
	m_btn_excel.LoadAltImage(IDB_PNG_EXCEL, "PNG");

	m_btn_word.LoadStdImage(IDB_PNG_WORD, "PNG");
	m_btn_word.LoadHovImage(IDB_PNG_WORD, "PNG");
	m_btn_word.LoadAltImage(IDB_PNG_WORD, "PNG");

	m_btn_excel.MoveWindow(20, 40, 64, 64);
	m_btn_word.MoveWindow(140, 40, 64, 64);
	m_btn_notepad.MoveWindow(260, 40, 64, 64);

	m_btn_notepad2.ShowWindow(SW_HIDE);
	m_btn_excel2.ShowWindow(SW_HIDE);
	m_btn_word2.ShowWindow(SW_HIDE);

	m_btn_excel2.SetFocus();

	InitFrame("확장자 선택");

	//SetClassLongA(m_btn_excel.GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));
	//SetClassLongA(m_btn_word.GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));
	//SetClassLongA(m_btn_notepad.GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));

	/*m_btn_notepad.ShowWindow(SW_HIDE);
	m_btn_excel.ShowWindow(SW_HIDE);
	m_btn_word.ShowWindow(SW_HIDE);

	CImage cbuttonImg1, cbuttonImg2, cbuttonImg3;

	HRESULT result = cbuttonImg1.Load("res/Excel.png");

	if (result != S_OK)
		return NULL;

	result = cbuttonImg2.Load("res/Word.png");

	if (result != S_OK)
		return NULL;

	result = cbuttonImg3.Load("res/Notepad.png");

	if (result != S_OK)
		return NULL;

	HBITMAP hBit1, hBit2, hBit3;
	hBit1 = cbuttonImg1.Detach();
	hBit2 = cbuttonImg2.Detach();
	hBit3 = cbuttonImg3.Detach();

	m_btn_excel2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_btn_excel2.m_bTransparent = TRUE;
	m_btn_excel2.m_bDrawFocus = FALSE;

	m_btn_word2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_btn_word2.m_bTransparent = TRUE;
	m_btn_word2.m_bDrawFocus = FALSE;

	m_btn_notepad2.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_btn_notepad2.m_bTransparent = TRUE;
	m_btn_notepad2.m_bDrawFocus = FALSE;

	m_btn_excel2.SetImage(hBit1, TRUE, hBit1);
	m_btn_word2.SetImage(hBit2, TRUE, hBit1);
	m_btn_notepad2.SetImage(hBit3, TRUE, hBit1);

	m_btn_excel2.MoveWindow(20, 20, 75, 75);
	m_btn_word2.MoveWindow(140, 20, 75, 75);
	m_btn_notepad2.MoveWindow(260, 20, 75, 75);

	m_btn_excel2.SetMouseCursorHand();
	m_btn_word2.SetMouseCursorHand();
	m_btn_notepad2.SetMouseCursorHand();

	m_btn_excel.SetFocus();
	*/

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL BOKOSelectExtentionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		if (pMsg->hwnd == m_btn_excel)
		{
			*m_nSelectExtentionType = 1;
			Log_Manager->OnPutLog("엑셀 내보내기 선택", LogType::LT_EVENT);
			SendMessage(WM_CLOSE);
		}
		else if (pMsg->hwnd == m_btn_word)
		{
			*m_nSelectExtentionType = 2;
			Log_Manager->OnPutLog("워드 내보내기 선택", LogType::LT_EVENT);
			SendMessage(WM_CLOSE);
		}
		else if (pMsg->hwnd == m_btn_notepad)
		{
			*m_nSelectExtentionType = 3;
			Log_Manager->OnPutLog("메모장 내보내기 선택", LogType::LT_EVENT);
			SendMessage(WM_CLOSE);
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (pMsg->hwnd == m_btn_excel)
		{
			CURSOR_HAND;
		}
		else if (pMsg->hwnd == m_btn_word)
		{
			CURSOR_HAND;
		}
		else if (pMsg->hwnd == m_btn_notepad)
		{
			CURSOR_HAND;
		}
		else
			CURSOR_ARROW;
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void BOKOSelectExtentionDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
}


HBRUSH BOKOSelectExtentionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void BOKOSelectExtentionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


void BOKOSelectExtentionDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}
