// BOKOOptionScenarioExportDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOOptionScenarioExportDlg.h"
#include "afxdialogex.h"


// BOKOOptionScenarioExportDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOOptionScenarioExportDlg, CDialogEx)

BOKOOptionScenarioExportDlg::BOKOOptionScenarioExportDlg(ImportAndExport bind, bool* bSelected, ComplexString* selectScenarioTitle, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTION_EXPORT, pParent)
{
	m_bind = bind;
	m_selectScenarioTitle = selectScenarioTitle;
	m_bSelected = bSelected;
}

BOKOOptionScenarioExportDlg::~BOKOOptionScenarioExportDlg()
{
}

void BOKOOptionScenarioExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OPTION_SELECT_SCENARIO, m_list_export_scenario);
}


BEGIN_MESSAGE_MAP(BOKOOptionScenarioExportDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OPTION_SELECT_SCENARIO, &BOKOOptionScenarioExportDlg::OnNMDblclkListOptionSelectScenario)
END_MESSAGE_MAP()


// BOKOOptionScenarioExportDlg 메시지 처리기


BOOL BOKOOptionScenarioExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	CURSOR_WAIT;
	if (Scenario_DB_Manager->SelectAllScenarioList(&m_loadScenarioList))
	{
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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOOptionScenarioExportDlg::InsertScenario(ComplexString title, ComplexString index)
{
	int listSize = m_list_export_scenario.GetItemCount();
	m_list_export_scenario.InsertItem(listSize, _T(""));

	LVITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = listSize;
	item1.iSubItem = 1;
	item1.pszText = (LPSTR)index.GetBuffer();
	m_list_export_scenario.SetItem(&item1);

	LVITEM item2;
	item2.mask = LVIF_TEXT;
	item2.iItem = listSize;
	item2.iSubItem = 2;
	item2.pszText = (LPSTR)title.GetBuffer();
	m_list_export_scenario.SetItem(&item2);

}

void BOKOOptionScenarioExportDlg::Initialize()
{
	if (m_bind == SCENARIO_EXPORT)
		SetWindowTextA("내보낼 시나리오 선택");
	else
		SetWindowTextA("불러올 시나리오 선택");


	m_list_export_scenario.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_export_scenario.InsertColumn(0, "", LVCFMT_LEFT, 0);
	m_list_export_scenario.InsertColumn(1, "순번", LVCFMT_CENTER, 0);
	m_list_export_scenario.InsertColumn(2, "시나리오 명", LVCFMT_LEFT, 170);
}

void BOKOOptionScenarioExportDlg::OnNMDblclkListOptionSelectScenario(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMItemActivate->iItem != -1)
	{
		*m_selectScenarioTitle = m_list_export_scenario.GetItemText(pNMItemActivate->iItem, 2);

		*m_bSelected = true;
		SendMessage(WM_CLOSE);
	}

	*pResult = 0;
}
