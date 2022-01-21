
// BOKOPadDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BOKOPad.h"
#include "BOKOPadDlg.h"
#include "File/FileProcess.h"
#include "BOKOOptionScenarioExportDlg.h"
#include "BOKOLogViewDlg.h"
#include "afxdialogex.h"
#include "Application/ApplicationProcess.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBOKOPadDlg 대화 상자



CBOKOPadDlg::CBOKOPadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOKOPAD_DIALOG, pParent)
	, DlgInterface(this, true)
{
	CreateFrame();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Log_Manager->OnPutLog("BOKOPadDlg 생성자 호출", LogType::LT_PROCESS);
}

CBOKOPadDlg::~CBOKOPadDlg()
{
	Scenario_UI_Manager->SendMessages(PM_SCENARIO_CLEAR);
	Log_Manager->OnPutLog("BOKOPadDlg 소멸자 호출", LogType::LT_PROCESS);

	m_scenario.DestroyWindow();
}

void CBOKOPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBOKOPadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
#ifndef NOT_USED
	ON_COMMAND(ID_SCENARIO_EXPORT, &CBOKOPadDlg::OnScenarioExport)
	ON_COMMAND(ID_SCENARIO_IMPORT, &CBOKOPadDlg::OnScenarioImport)
#endif
	ON_COMMAND(ID_PROGRAM_CLOSE, &CBOKOPadDlg::OnProgramClose)
	ON_COMMAND(ID_LOG_VIEW, &CBOKOPadDlg::OnLogView)
	ON_COMMAND(ID_EXPLANATION_VIEW, &CBOKOPadDlg::OnExplanationView)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CBOKOPadDlg 메시지 처리기

BOOL CBOKOPadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetMenu(NULL);
	
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//SetBackgroundColor(RGB(68, 68, 68));

	Initialize();
	Log_Manager->OnPutLog("메인 화면 초기화", LogType::LT_PROCESS);
	
	m_scenario.LoadScenarioList();

	Scenario_UI_Manager->AttachManager(this);
	Log_Manager->OnPutLog("시나리오 UI 매니저 연결", LogType::LT_PROCESS);

	CRect rect;
	GetWindowRect(rect);

	return FALSE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBOKOPadDlg::Initialize()
{
	this->SetWindowPos(NULL, 0, 0, MAIN_DLG_SIZE_WIDTH, MAIN_DLG_SIZE_HEIGHT, SWP_NOMOVE);


	InitFrame("BOKOPad");
	m_menu.Create(CustomMenu::IDD, this);
	m_menu.ShowWindow(SW_HIDE);
	m_menu.AddMenu("설명 확인", OnExplanationViewCallBackFunc);
#ifdef ADMIN_CONFIRM_VIEW
	m_menu.AddMenu("로그 확인", OnLogViewCallBackFunc);
	m_menu.AddMenu("DB 콘솔", OnDBConsoleCallBackFunc);
#endif

	m_menu.Init(true);

	SetWindowTextA("BOKOPad");

	m_scenario.Create(ScenarioListCtrl::IDD, this);
	m_scenario.ShowWindow(SW_SHOW);
	m_scenario.MoveWindow(50, 50, TABLE_SIZE_WIDTH, TABLE_SIZE_HEIGHT);

}

void CBOKOPadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBOKOPadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CPaintDC dc(this);
		DrawFrame(&dc);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBOKOPadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CBOKOPadDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

#ifndef NOT_USED
void CBOKOPadDlg::OnScenarioExport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);

	ComplexString path;
	path = dir_;
	path.Remove(DB_MODULE_NANE);

	CFolderPickerDialog fd(path.GetBuffer(), OFN_FILEMUSTEXIST, NULL, 0);
	if (fd.DoModal() != IDOK)
		return;

	// 등록할 시나리오선택.. 
	bool bSelected = false;
	ComplexString selectScenarioTitle;
	BOKOOptionScenarioExportDlg exportDlg(BOKOOptionScenarioExportDlg::SCENARIO_EXPORT, &bSelected, &selectScenarioTitle);
	exportDlg.DoModal();

	if (!bSelected)
		return;

	ScenarioListVO outputScenario;

	if (Scenario_DB_Manager->SelectInSceSEQScenarioListInSceTITLE(selectScenarioTitle, &outputScenario) == false)
		return;

	Log_Manager->OnPutLog("내보낼 시나리오 정보 로드", LogType::LT_PROCESS);

	CURSOR_WAIT;

	CString strFullPath = fd.GetPathName();
	Log_Manager->OnPutLog(ComplexString(strFullPath.GetBuffer()) + " 폴더 선택", LogType::LT_EVENT);

	ComplexVector<NoteInformationVO> loadNoteInformationContainer;
	if (Scenario_DB_Manager->SelectInSceSEQNoteInformation(outputScenario.GetSceSEQ(), &loadNoteInformationContainer))
	{
		Log_Manager->OnPutLog("해당 시나리오의 노트정보 로드", LogType::LT_PROCESS);
		ComplexVector<NoteInformationVO>::iterator iter = loadNoteInformationContainer.begin();
		while (iter != loadNoteInformationContainer.end())
		{
			NoteInformationVO noteInform = iter->value;

			ComplexString strWriteAnsiContent, strConvertUTF8Content;
			strWriteAnsiContent = noteInform.GetNotCONTENT();
			//ComplexUtilProcess::ANSIToUTF8(strConvertUTF8Content, strWriteAnsiContent);
			//ComplexUtilProcess::ExportFile(strConvertUTF8Content, strFullPath.GetBuffer());
			ComplexUtilProcess::ExportFile(strWriteAnsiContent, strFullPath.GetBuffer(), "txt");
			Log_Manager->OnPutLog("노트 내보내기 성공", LogType::LT_PROCESS);
			iter++;
		}
	}
	else
	{
		CURSOR_ARROW;
		MessageBox("데이터 불러오기에 실패하였습니다.");
		Log_Manager->OnPutLog("시나리오 내보내기 실패", LogType::LT_PROCESS);
		return;
	}

	CURSOR_ARROW;
	MessageBox("파일 내보내기에 성공하였습니다.");
}


void CBOKOPadDlg::OnScenarioImport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog fd(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, "All Files(*.*) | *.*||", this);
	if (fd.DoModal() != IDOK)
		return;

	// 등록할 시나리오선택.. 
	bool bSelected = false;
	ComplexString selectScenarioTitle;
	BOKOOptionScenarioExportDlg exportDlg(BOKOOptionScenarioExportDlg::SCENARIO_IMPORT, &bSelected, &selectScenarioTitle);
	exportDlg.DoModal();

	if (!bSelected)
		return;

	ScenarioListVO outputScenario;

	if (Scenario_DB_Manager->SelectInSceSEQScenarioListInSceTITLE(selectScenarioTitle, &outputScenario) == false)
		return;

	Log_Manager->OnPutLog("불러올 시나리오 정보 로드", LogType::LT_PROCESS);

	CURSOR_WAIT;
	POSITION pos = fd.GetStartPosition();

	while (pos != NULL)
	{
		ComplexString strPathName = fd.GetNextPathName(pos).GetBuffer();
		Log_Manager->OnPutLog(ComplexString("불러온 파일 명 : ") + strPathName, LogType::LT_PROCESS);
		ComplexString strReadUTF8Content, strConvertAnsiContent;
		ComplexUtilProcess::ImportFile(strReadUTF8Content, strPathName);
		Log_Manager->OnPutLog("파일 불러오기 성공", LogType::LT_PROCESS);
		//ComplexUtilProcess::UTF8ToANSI(strConvertAnsiContent, strReadUTF8Content);

		NoteInformationVO insertNote(0, outputScenario.GetSceSEQ(), false, false, strReadUTF8Content);
		if (Scenario_DB_Manager->InsertNoteInformation(insertNote) == false)
		{
			CURSOR_ARROW;
			MessageBox("데이터 저장에 실패하였습니다.");
			Log_Manager->OnPutLog("노트 정보 DB 저장 실패", LogType::LT_PROCESS);
			return;
		}
		Log_Manager->OnPutLog("노트 정보 DB 저장 성공", LogType::LT_PROCESS);
	}

	CURSOR_ARROW;
	MessageBox("파일 불러오기에 성공하였습니다.");
}
#endif

void CBOKOPadDlg::OnProgramClose()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Scenario_UI_Manager->SendMessages(PM_SCENARIO_CLEAR);
	Log_Manager->OnPutLog("BOKOPad.exe 종료", LogType::LT_OPERATE);
	this->PostMessageA(WM_CLOSE);
}

void OnLogViewCallBackFunc()
{
	CBOKOPadDlg* dlg = (CBOKOPadDlg*)theApp.GetMainWnd();
	dlg->OnLogView();
}

void OnExplanationViewCallBackFunc()
{
	CBOKOPadDlg* dlg = (CBOKOPadDlg*)theApp.GetMainWnd();
	dlg->OnExplanationView();
}

void OnDBConsoleCallBackFunc()
{
	CBOKOPadDlg* dlg = (CBOKOPadDlg*)theApp.GetMainWnd();
	if (ComplexUtilProcess::FindProcess("BOKODBConsole.exe") == 0 && ComplexUtilProcess::FindProcess("BOKODBConsole_dbg.exe") == 0)
	{
#ifdef DEBUG
		ComplexUtilProcess::ExecuteProcess("BOKODBConsole_dbg.exe");
#elif NDEBUG
		ComplexUtilProcess::ExecuteProcess("BOKODBConsole.exe");
#endif
	}
	else
	{
		dlg->MessageBox("BOKOConsole이 이미 실행 중 입니다.");
	}
}

void CBOKOPadDlg::OnLogView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BOKOLogViewDlg logView;
	logView.DoModal();
}


void CBOKOPadDlg::OnExplanationView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

}

void CBOKOPadDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


HBRUSH CBOKOPadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CBOKOPadDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_menu.Open(point);

	__super::OnRButtonDown(nFlags, point);
}


BOOL CBOKOPadDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	return __super::OnCommand(wParam, lParam);
}

bool CBOKOPadDlg::DragDown(MSG* pMsg)
{
	return false;
}

bool CBOKOPadDlg::DragMove(MSG* pMsg)
{
	return false;
}

bool CBOKOPadDlg::DragUp(MSG* pMsg)
{
	return false;
}