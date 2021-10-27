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
	, m_logMap(nullptr)
	, m_selectComboIndex(0)
{
	Log_Manager->OnPutLog("BOKOLogViewDlg 생성자 호출", LogType::LT_PROCESS);
}

BOKOLogViewDlg::~BOKOLogViewDlg()
{
	Log_Manager->OnPutLog("BOKOLogViewDlg 생성자 호출", LogType::LT_PROCESS);
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
END_MESSAGE_MAP()


// BOKOLogViewDlg 메시지 처리기


BOOL BOKOLogViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	LoadLog();

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
	m_list_log_view.InsertColumn(1, "종류", LVCFMT_CENTER, 70);
	m_list_log_view.InsertColumn(2, "시간", LVCFMT_CENTER, 100);
	m_list_log_view.InsertColumn(3, "내용", LVCFMT_LEFT, 270);

	m_combo_log_divide.InsertString(0, "All");
	m_combo_log_divide.InsertString(1, "Event");
	m_combo_log_divide.InsertString(2, "Process");
	m_combo_log_divide.InsertString(3, "Operate");
	m_combo_log_divide.SetCurSel(0);
	m_selectComboIndex = 0;
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

void BOKOLogViewDlg::OnCbnSelchangeComboLogDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TRACE("!\n");
}


void BOKOLogViewDlg::OnCbnDropdownComboLogDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

void BOKOLogViewDlg::LoadLog()
{
	Log_Manager->OnLoadLogData();
	m_logMap = Log_Manager->GetLogData();
	AnalyzeLogData();
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

	m_list_log_view.SetRedraw(FALSE);
	m_list_log_view.DeleteAllItems();

	ComplexMap<ComplexString, ComplexVector<ComplexString>>::iterator iter = m_logMap->find(strStartTime.GetBuffer());

	int eventCount = 0;
	int processCount = 0;
	int operateCount = 0;

	int startTimeIndex = 0;
	int endTimeIndex = 0;
	int startLogIndex = 0;
	int endLogIndex = 0;
	ComplexString strTime, strLog;
	while (iter != m_logMap->end())
	{
		ComplexVector<ComplexString>::iterator iter1 = iter->value.value.begin();
		for (int i = 0; i < iter->value.value.size(); i++)
		{
			ComplexString logElement = iter->value.value.at(i);

			if (logElement.Find("Event") != -1)
			{
				eventCount++;
				startTimeIndex = 8;
			}
			else if (logElement.Find("Process") != -1)
			{
				processCount++;
				startTimeIndex = 10;
			}
			else if (logElement.Find("Operate") != -1)
			{
				operateCount++;
				startTimeIndex = 10;
			}
			else
				return;

			endTimeIndex = startTimeIndex + 7;
			startLogIndex = endTimeIndex + 5;
			endLogIndex = logElement.GetLength() - 1;

			strTime = logElement.GetText(startTimeIndex, endTimeIndex);
			strLog = logElement.GetText(startLogIndex, endLogIndex);

			// 리스트(카운트, 뷰) 등록 (콤보박스 구분)
		}

		if (iter->value.key == strEndTime.GetBuffer())
		{
			break;
		}
		iter++;
	}

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
