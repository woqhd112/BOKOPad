
// BOKOPadDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BOKOPad.h"
#include "BOKOPadDlg.h"
#include "BOKOOptionDlg.h"
#include "afxdialogex.h"

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 컨트롤러 생성
	//m_controller = new DlgController;
}

CBOKOPadDlg::~CBOKOPadDlg()
{
	/*if (m_controller)
	{
		delete m_controller;
		m_controller = nullptr;
	}*/
}

void CBOKOPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SCENARIO_LIST, m_list_scenario_list);
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_btn_option);
	DDX_Control(pDX, IDC_EDIT_INPUT_SCENARIO, m_edit_input_scenario);
	DDX_Control(pDX, IDC_BUTTON_INPUT_SCENARIO, m_btn_input_scenario);
	DDX_Control(pDX, IDC_BUTTON_SCENARIO_TITLE_MODIFY, m_btn_scenario_title_modify);
	DDX_Control(pDX, IDC_BUTTON_SCENARIO_DELETE, m_btn_scenario_delete);
}

BEGIN_MESSAGE_MAP(CBOKOPadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CBOKOPadDlg::OnBnClickedButtonOption)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCENARIO_LIST, &CBOKOPadDlg::OnLvnItemchangedListScenarioList)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_SCENARIO, &CBOKOPadDlg::OnBnClickedButtonInputScenario)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_TITLE_MODIFY, &CBOKOPadDlg::OnBnClickedButtonScenarioTitleModify)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_DELETE, &CBOKOPadDlg::OnBnClickedButtonScenarioDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SCENARIO_LIST, &CBOKOPadDlg::OnNMDblclkListScenarioList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCENARIO_LIST, &CBOKOPadDlg::OnNMClickListScenarioList)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	CURSOR_WAIT;
	// 옵션 로드
	if (MVC_Controller->SelectAllPadOption())
	{
		RequestScope->GetRequestAttributes(&m_mainOptionData);
	}

	// 시나리오 리스트 로드
	if (MVC_Controller->SelectAllScenarioList())
	{
		RequestScope->GetRequestAttributes(&m_loadScenarioList);

		ComplexVector<ScenarioListVO>::iterator iter = m_loadScenarioList.begin();

		while (iter != m_loadScenarioList.end())
		{
			ComplexString index = ComplexConvert::IntToString(iter->value.GetSceSEQ());
			ComplexString title = iter->value.GetSceTITLE();
			InsertScenario(title, index);
			iter++;
		}
	}
	CURSOR_ARROW;

	GotoDlgCtrl(&m_edit_input_scenario);
	m_edit_input_scenario.LimitText(20);

	Scenario_Manager->AttachManager(this);

	return FALSE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBOKOPadDlg::InsertScenario(ComplexString title, ComplexString index)
{
	int listSize = m_list_scenario_list.GetItemCount();
	m_list_scenario_list.InsertItem(listSize, _T(""));
	
	LVITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = listSize;
	item1.iSubItem = 1;
	item1.pszText = (LPSTR)index.GetBuffer();
	m_list_scenario_list.SetItem(&item1);

	LVITEM item2;
	item2.mask = LVIF_TEXT;
	item2.iItem = listSize;
	item2.iSubItem = 2;
	item2.pszText = (LPSTR)title.GetBuffer();
	m_list_scenario_list.SetItem(&item2);

}

void CBOKOPadDlg::Initialize()
{
	SetWindowTextA("BOKOPad");

	m_list_scenario_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_scenario_list.InsertColumn(0, "", LVCFMT_LEFT, 0);
	m_list_scenario_list.InsertColumn(1, "순번", LVCFMT_CENTER, 0);
	m_list_scenario_list.InsertColumn(2, "시나리오 명", LVCFMT_LEFT, 350);
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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBOKOPadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBOKOPadDlg::OnLvnItemchangedListScenarioList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	pNMLV->iItem;
	pNMLV->iSubItem;
	*pResult = 0;
}


void CBOKOPadDlg::OnBnClickedButtonInputScenario()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString getText;
	m_edit_input_scenario.GetWindowTextA(getText);

	if (getText.IsEmpty())
		return;

	ComplexString inputScenarioTitle = getText.GetBuffer();

	CURSOR_WAIT;
	RequestScope->SetRequestAttributes(ScenarioListVO(0, 0, inputScenarioTitle));
	if (MVC_Controller->InsertScenarioList())
	{
		ComplexString index = ComplexConvert::IntToString(m_list_scenario_list.GetItemCount() + 1);
		InsertScenario(inputScenarioTitle, index);
		m_edit_input_scenario.SetWindowTextA("");

		if (MVC_Controller->SelectAllScenarioList())
		{
			m_loadScenarioList.clear();
			RequestScope->GetRequestAttributes(&m_loadScenarioList);
		}
	}
	CURSOR_ARROW;
}


BOOL CBOKOPadDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonInputScenario();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		if (pMsg->hwnd == m_list_scenario_list) {}
		else if (pMsg->hwnd == m_btn_scenario_delete) {}
		else if (pMsg->hwnd == m_btn_scenario_title_modify) {}
		else
		{
			m_list_scenario_list.SetSelectionMark(-1);
			m_list_scenario_list.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBOKOPadDlg::OnBnClickedButtonOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOKOOptionDlg optionDlg;
	optionDlg.DoModal();
}


void CBOKOPadDlg::OnBnClickedButtonScenarioTitleModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_loadScenarioList.empty())
		return;

	int mark = m_list_scenario_list.GetSelectionMark();

	mark;
}


void CBOKOPadDlg::OnBnClickedButtonScenarioDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_loadScenarioList.empty())
		return;

	CURSOR_WAIT;
	int selectedCount = m_list_scenario_list.GetSelectedCount();
	POSITION selectPos = m_list_scenario_list.GetFirstSelectedItemPosition();

	m_list_scenario_list.SetRedraw(FALSE);
	ComplexVector<int> selectVector;
	while (selectPos)
	{
		int selectedIndex = m_list_scenario_list.GetNextSelectedItem(selectPos);
		selectVector.push_back(selectedIndex);
	}

	for (int i = selectVector.size() - 1; i >= 0; i--)
	{
		ScenarioListVO selectedScenario = m_loadScenarioList.at(i);
		RequestScope->SetRequestAttributes(selectedScenario);

		if (MVC_Controller->DeleteScenarioList() == true)
		{
			m_list_scenario_list.DeleteItem(i);
			m_loadScenarioList.erase(i);
		}
	}

	m_list_scenario_list.SetRedraw(TRUE);
	m_list_scenario_list.Invalidate();

	if (m_loadScenarioList.empty())
	{
		MVC_Controller->UpdateScenarioListAutoIncrementSeq();
	}

	CURSOR_ARROW;
}


void CBOKOPadDlg::OnNMDblclkListScenarioList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_loadScenarioList.empty())
		return;

	int mark = m_list_scenario_list.GetSelectionMark();
	if (mark < 0)
		return;

	// 시나리오가 이미 존재할 시
	ScenarioManagerStruct scenarioStruct(m_loadScenarioList.at(mark), mark);
	Scenario_Manager->InputScenarioStruct(&scenarioStruct);

	if (Scenario_Manager->SendMessages(PM_EXIST) == true)
		return;

	CURSOR_WAIT;
	Scenario_Manager->SendMessages(PM_CREATE);
	CURSOR_ARROW;
	*pResult = 0;
}


void CBOKOPadDlg::OnNMClickListScenarioList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMItemActivate->iItem == -1)
	{
		m_list_scenario_list.SetSelectionMark(-1);
		m_list_scenario_list.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}
