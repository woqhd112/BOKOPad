// BOKOLogViewDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOLogViewDlg.h"
#include "afxdialogex.h"


// BOKOLogViewDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOLogViewDlg, CDialogEx)

BOKOLogViewDlg::BOKOLogViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOG_VIEW, pParent)
	, DlgInterface(this, true)
	, m_logMap(nullptr)
	, m_selectComboIndex(0)
{
	Log_Manager->OnPutLog("BOKOLogViewDlg 생성자 호출", LogType::LT_PROCESS);
	CreateFrame();
}

BOKOLogViewDlg::~BOKOLogViewDlg()
{
	Log_Manager->OnPutLog("BOKOLogViewDlg 소멸자 호출", LogType::LT_PROCESS);
}

void BOKOLogViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG_COUNT, m_list_log_count);
	DDX_Control(pDX, IDC_LIST_LOG_VIEW, m_list_log_view);
	DDX_Control(pDX, IDC_COMBO_LOG_DIVIDE, m_combo_log_divide);
	DDX_Control(pDX, IDC_DATETIMEPICKER_CURRENT_DATE, m_date_start_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER_LAST_DATE, m_date_last_date);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE_LOG, m_btn_execute);
}


BEGIN_MESSAGE_MAP(BOKOLogViewDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LOG_DIVIDE, &BOKOLogViewDlg::OnCbnSelchangeComboLogDivide)
	ON_CBN_DROPDOWN(IDC_COMBO_LOG_DIVIDE, &BOKOLogViewDlg::OnCbnDropdownComboLogDivide)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE_LOG, &BOKOLogViewDlg::OnBnClickedButtonExecuteLog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// BOKOLogViewDlg 메시지 처리기


BOOL BOKOLogViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CURSOR_WAIT;
	Initialize();

	LoadLog();
	CURSOR_ARROW;

	InitFrame("로그");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOLogViewDlg::Initialize()
{
	m_list_log_count.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_log_count.InsertColumn(0, "", LVCFMT_LEFT, 0);
	m_list_log_count.InsertColumn(1, "종류", LVCFMT_CENTER, 70);
	m_list_log_count.InsertColumn(2, "개수", LVCFMT_LEFT, 370);

	InsertCountList(0, "Event", "0");
	InsertCountList(1, "Process", "0");
	InsertCountList(2, "Operate", "0");

	m_list_log_view.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_log_view.InsertColumn(0, "", LVCFMT_LEFT, 0);
	m_list_log_view.InsertColumn(1, "종류", LVCFMT_CENTER, 55);
	m_list_log_view.InsertColumn(2, "시간", LVCFMT_CENTER, 60);
	m_list_log_view.InsertColumn(3, "내용", LVCFMT_LEFT, 270);

	m_combo_log_divide.InsertString(0, "All");
	m_combo_log_divide.InsertString(1, "Event");
	m_combo_log_divide.InsertString(2, "Process");
	m_combo_log_divide.InsertString(3, "Operate");
	m_combo_log_divide.SetCurSel(0);
	m_selectComboIndex = 0;

	m_btn_execute.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);

}

void BOKOLogViewDlg::InsertCountList(int index, ComplexString type, ComplexString text)
{
	m_list_log_count.InsertItem(index, "");

	LVITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = index;
	item1.iSubItem = 1;
	item1.pszText = (LPSTR)type.GetBuffer();
	m_list_log_count.SetItem(&item1);

	LVITEM item2;
	item2.mask = LVIF_TEXT;
	item2.iItem = index;
	item2.iSubItem = 2;
	item2.pszText = (LPSTR)text.GetBuffer();
	m_list_log_count.SetItem(&item2);
}

void BOKOLogViewDlg::InsertViewList(int index, ComplexString type, ComplexString time, ComplexString content)
{
	m_list_log_view.InsertItem(index, "");

	LVITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = index;
	item1.iSubItem = 1;
	item1.pszText = (LPSTR)type.GetBuffer();
	m_list_log_view.SetItem(&item1);

	LVITEM item2;
	item2.mask = LVIF_TEXT;
	item2.iItem = index;
	item2.iSubItem = 2;
	item2.pszText = (LPSTR)time.GetBuffer();
	m_list_log_view.SetItem(&item2);

	LVITEM item3;
	item3.mask = LVIF_TEXT;
	item3.iItem = index;
	item3.iSubItem = 3;
	item3.pszText = (LPSTR)content.GetBuffer();
	m_list_log_view.SetItem(&item3);
}

void BOKOLogViewDlg::OnCbnSelchangeComboLogDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//TRACE("!\n");
}


void BOKOLogViewDlg::OnCbnDropdownComboLogDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

void BOKOLogViewDlg::LoadLog()
{
	if (Log_Manager->OnLoadLogData() == true)
		m_logMap = Log_Manager->GetLogData();
	else
	{
		OnClose();
	}
}

void BOKOLogViewDlg::AnalyzeLogData()
{
	if (m_logMap == nullptr)
		return;

	int selectCombo = m_combo_log_divide.GetCurSel();

	CTime startTime, endTime;
	CString strStartTime, strEndTime;
	m_date_start_date.GetTime(startTime);
	m_date_last_date.GetTime(endTime);

	strStartTime = startTime.Format("%Y_%m_%d");
	strEndTime = endTime.Format("%Y_%m_%d");

	if (strStartTime == m_startDate && strEndTime == m_endDate && selectCombo == m_selectComboIndex)
		return;

	// 시작 날짜가 끝 날짜보다 높을떄는 리턴한다.
	if (startTime > endTime)
		return;

	ComplexMap<ComplexString, ComplexVector<ComplexString>>::iterator iter;
	iter = m_logMap->find(strStartTime.GetBuffer());

	ComplexTimeTable timeTable;
	timeTable.year = startTime.GetYear();
	timeTable.month = startTime.GetMonth();
	timeTable.day = startTime.GetDay();

	// start 타임값을 찾는다.
	// end 까지 날짜를 세고 없으면 리턴한다.
	do
	{
		strStartTime.Format("%04d_%02d_%02d", timeTable.year, timeTable.month, timeTable.day);
		iter = m_logMap->find(strStartTime.GetBuffer());

		if (strStartTime == strEndTime)
			break;

		timeTable += 86400;	// 하루 (초)
	}
	while (iter == m_logMap->end());

	m_list_log_view.SetRedraw(FALSE);
	m_list_log_view.DeleteAllItems();


	int eventCount = 0;
	int processCount = 0;
	int operateCount = 0;

	int startTimeIndex = 0;
	int endTimeIndex = 0;
	int startLogIndex = 0;
	int endLogIndex = 0;
	ComplexString strTime, strLog;

	int insertLogViewIndex = 0;
	while (iter != m_logMap->end())
	{
		ComplexVector<ComplexString>::iterator iter1 = iter->value.value.begin();
		for (int i = 0; i < iter->value.value.size(); i++)
		{
			ComplexString logElement = iter->value.value.at(i);
			const char* type;
			if (logElement.Find("Event") != -1)
			{
				eventCount++;
				startTimeIndex = 8;
				type = "Event";
			}
			else if (logElement.Find("Process") != -1)
			{
				processCount++;
				startTimeIndex = 10;
				type = "Process";
			}
			else if (logElement.Find("Operate") != -1)
			{
				operateCount++;
				startTimeIndex = 10;
				type = "Operate";
			}
			else
				return;

			endTimeIndex = startTimeIndex + 7;
			startLogIndex = endTimeIndex + 5;
			endLogIndex = logElement.GetLength() - 1;

			strTime = logElement.GetText(startTimeIndex, endTimeIndex);
			strLog = logElement.GetText(startLogIndex, endLogIndex);

			if (selectCombo == 0)	
			{
				// All
				InsertViewList(insertLogViewIndex, type, strTime, strLog);
				insertLogViewIndex++;
			}
			else if (selectCombo == 1)
			{
				// Event
				if (type == "Event")
				{
					InsertViewList(insertLogViewIndex, type, strTime, strLog);
					insertLogViewIndex++;
				}
			}
			else if (selectCombo == 2)
			{
				// Process
				if (type == "Process")
				{
					InsertViewList(insertLogViewIndex, type, strTime, strLog);
					insertLogViewIndex++;
				}
			}
			else if (selectCombo == 3)
			{
				// Operate
				if (type == "Operate")
				{
					InsertViewList(insertLogViewIndex, type, strTime, strLog);
					insertLogViewIndex++;
				}
			}
		}

		if (iter->value.key == strEndTime.GetBuffer())
		{
			break;
		}
		iter++;
	}

	if (selectCombo == 1)
	{
		// Event
		processCount = 0;
		operateCount = 0;
	}
	else if (selectCombo == 2)
	{
		// Process
		eventCount = 0;
		operateCount = 0;
	}
	else if (selectCombo == 3)
	{
		// Operate
		eventCount = 0;
		processCount = 0;
	}

	ComplexString strEventCount, strProcessCount, strOperateCount;
	strEventCount.Format("%d", eventCount);
	strProcessCount.Format("%d", processCount);
	strOperateCount.Format("%d", operateCount);
	m_list_log_count.SetItemText(0, 2, strEventCount.GetBuffer());
	m_list_log_count.SetItemText(1, 2, strProcessCount.GetBuffer());
	m_list_log_count.SetItemText(2, 2, strOperateCount.GetBuffer());

	m_startDate = strStartTime;
	m_endDate = strEndTime;
	m_selectComboIndex = selectCombo;

	m_list_log_view.SetRedraw(TRUE);
	m_list_log_view.Invalidate();
}

void BOKOLogViewDlg::OnBnClickedButtonExecuteLog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AnalyzeLogData();
}


void BOKOLogViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.
	DrawFrame(&dc);
}


void BOKOLogViewDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


HBRUSH BOKOLogViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		return m_staticBrush;
	}


	return hbr;
}


BOOL BOKOLogViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}
	return __super::PreTranslateMessage(pMsg);
}
