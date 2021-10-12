// BOKOTimelineOneViewDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOTimelineOneViewDlg.h"
#include "TimelineUIManager.h"
#include "TimelineDBManager.h"
#include "afxdialogex.h"
#include "FileProcess.h"


// BOKOTimelineOneViewDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOTimelineOneViewDlg, CDialogEx)

BOKOTimelineOneViewDlg::BOKOTimelineOneViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE_ONE_VIEW, pParent)
	, m_bExpandedProcess(false)
{

}

BOKOTimelineOneViewDlg::~BOKOTimelineOneViewDlg()
{
}

void BOKOTimelineOneViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIMELINE_ONE_VIEW_EDIT, m_edit_one_view);
	DDX_Control(pDX, IDC_BUTTON_TIMELINE_EXPORT, m_btn_timeline_export);
	DDX_Control(pDX, IDC_BUTTON_EXPAND_ALL, m_btn_expand_all);
	DDX_Control(pDX, IDC_BUTTON_ONE_VIEW, m_btn_preview);
}


BEGIN_MESSAGE_MAP(BOKOTimelineOneViewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TIMELINE_EXPORT, &BOKOTimelineOneViewDlg::OnBnClickedButtonTimelineExport)
	ON_BN_CLICKED(IDC_BUTTON_EXPAND_ALL, &BOKOTimelineOneViewDlg::OnBnClickedButtonExpandAll)
	ON_BN_CLICKED(IDC_BUTTON_ONE_VIEW, &BOKOTimelineOneViewDlg::OnBnClickedButtonOneView)
END_MESSAGE_MAP()


// BOKOTimelineOneViewDlg 메시지 처리기

void BOKOTimelineOneViewDlg::AttachManager(TimelineUIManager* manager, TimelineDBManager* dbmanager)
{
	m_list_one_view.AttachManager(manager, dbmanager);
}

void BOKOTimelineOneViewDlg::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_list_one_view.SetScenarioManagerStruct(thisDataStruct);
}

void BOKOTimelineOneViewDlg::Clear()
{
	m_list_one_view.DeleteAllItems();
	m_bExpandedProcess = false;
}

void BOKOTimelineOneViewDlg::SetTimelineText(ComplexString& strText)
{
	//m_edit_one_view.SetWindowTextA(strText.GetBuffer());
	m_list_one_view.InsertItem(strText);
}

void BOKOTimelineOneViewDlg::OnBnClickedButtonTimelineExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);

	ComplexString path;
	path = dir_;
	path.Remove(DB_MODULE_NANE);

	CFolderPickerDialog fd(path.GetBuffer(), OFN_FILEMUSTEXIST, NULL, 0);
	if (fd.DoModal() != IDOK)
		return;

	CString strFullPath = fd.GetPathName();

	CString strText;
	m_edit_one_view.GetWindowTextA(strText);
	ComplexString strWriteAnsiContent, strConvertUTF8Content;
	strWriteAnsiContent = strText.GetBuffer();

	// 이거 pc별로 포맷팅이 다른데 구분하는법있나.. BOKOOptionDlg도 처리해야함..
	//ComplexUtilProcess::ANSIToUTF8(strConvertUTF8Content, strWriteAnsiContent);
	ComplexUtilProcess::ExportFile(strWriteAnsiContent, strFullPath.GetBuffer());
}


BOOL BOKOTimelineOneViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	m_editFont.CreatePointFont(150, TEXT("굴림"));

	m_edit_one_view.SetFont(&m_editFont);

	m_edit_one_view.ShowWindow(SW_HIDE);

	m_list_one_view.Create(OneViewList::IDD, this);

	CRect rect;
	GetClientRect(rect);

	m_list_one_view.MoveWindow(rect.left + 20, 20, rect.right - 40, 310);
	m_list_one_view.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void BOKOTimelineOneViewDlg::OnBnClickedButtonExpandAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bExpandedProcess)
	{
		m_list_one_view.ExpandAll(true);
		m_bExpandedProcess = true;
	}
	else
	{
		m_list_one_view.ExpandAll(false);
		m_bExpandedProcess = false;
	}
}


void BOKOTimelineOneViewDlg::OnBnClickedButtonOneView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
