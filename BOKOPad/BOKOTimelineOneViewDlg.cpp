// BOKOTimelineOneViewDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOTimelineOneViewDlg.h"
#include "BOKOManager/UIManager/TimelineUIManager.h"
#include "BOKOManager/DBManager/TimelineDBManager.h"
#include "BOKOSelectExtentionDlg.h"
#include "afxdialogex.h"
#include "File/FileProcess.h"
#include "OfficeAutomation/Manager/ExcelAutomationManager.h"
#include "OfficeAutomation/Manager/WordAutomationManager.h"


// BOKOTimelineOneViewDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOTimelineOneViewDlg, CDialogEx)

BOKOTimelineOneViewDlg::BOKOTimelineOneViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE_ONE_VIEW, pParent)
	, m_bExpandedProcess(false)
	, m_bPreviewProcess(false)
{
	Log_Manager->OnPutLog("BOKOTimelineOneViewDlg 생성자 호출", LogType::LT_PROCESS);
}

BOKOTimelineOneViewDlg::~BOKOTimelineOneViewDlg()
{
	Log_Manager->OnPutLog("BOKOTimelineOneViewDlg 소멸자 호출", LogType::LT_PROCESS);
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

void BOKOTimelineOneViewDlg::AttachManager(NoteDBManager* dbmanager)
{
	m_list_one_view.AttachManager(dbmanager);
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

void BOKOTimelineOneViewDlg::SetTimelineText(ComplexString& strText, int noteSEQ)
{
	//m_edit_one_view.SetWindowTextA(strText.GetBuffer());
	m_list_one_view.InsertItem(strText, noteSEQ);
}

void BOKOTimelineOneViewDlg::OnBnClickedButtonTimelineExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CURSOR_WAIT;

	Log_Manager->OnPutLog("타임라인 내보내기 버튼 클릭", LogType::LT_EVENT);
	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);

	ComplexString path;
	path = dir_;
	path.Remove(DB_MODULE_NANE);

	int selectExtentionType = -1;
	// 확장자 선택
	Log_Manager->OnPutLog("확장자 선택 화면 생성 완료", LogType::LT_OPERATE);
	BOKOSelectExtentionDlg extentionSelect(&selectExtentionType);
	extentionSelect.DoModal();

	if (selectExtentionType == -1)
		return;

	
	ComplexString strWriteAnsiContent, strConvertUTF8Content;
	m_list_one_view.GetFullItemText(&strWriteAnsiContent);

	// 엑셀
	if (selectExtentionType == 1)
	{
		ExcelAutomationManager eam;

		CString name, path;
		if (eam.OnReadyProcess(&name, &path) == false)
			return;

		if (eam.StartExcel("시나리오") == false)
			return;

		ComplexStringTokenizer tokens;

		strWriteAnsiContent.ReplaceAll("\r\n", "+");
		tokens.ApplyStringTokenize(strWriteAnsiContent, '+');

		while (tokens.HasNextToken())
		{
			if (eam.SetRange(1, eam.GetDepth(), 1, 1))
			{
				eam.SetItem(tokens.NextToken().GetBuffer());
				eam.SaveDepth();
			}
		}

		eam.CloseExcel(path, false);
	}
	// 워드
	else if (selectExtentionType == 2)
	{
		WordAutomationManager wam;

		CString name, path;
		if (wam.OnReadyProcess(&name, &path) == false)
			return;

		if (wam.StartWord("시나리오") == false)
			return;

		wam.AppendText(strWriteAnsiContent.GetBuffer());

		wam.CloseWord(path, false);
	}
	// 메모장
	else if (selectExtentionType == 3)
	{
		CFolderPickerDialog fd(path.GetBuffer(), OFN_FILEMUSTEXIST, NULL, 0);
		if (fd.DoModal() != IDOK)
			return;

		CString strFullPath = fd.GetPathName();

		// 이거 pc별로 포맷팅이 다른데 구분하는법있나.. BOKOOptionDlg도 처리해야함..
		//ComplexUtilProcess::ANSIToUTF8(strConvertUTF8Content, strWriteAnsiContent);
		ComplexUtilProcess::ExportFile(strWriteAnsiContent, strFullPath.GetBuffer());
	}

	CURSOR_ARROW;

	Log_Manager->OnPutLog("타임라인 내보내기 성공", LogType::LT_PROCESS);
	MessageBox("타임라인 내보내기에 성공하였습니다.");
}


BOOL BOKOTimelineOneViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_editFont.CreatePointFont(150, TEXT("굴림"));

	m_edit_one_view.SetFont(&m_editFont);

	m_edit_one_view.ShowWindow(SW_HIDE);

	m_list_one_view.Create(OneViewList::IDD, this);
	Log_Manager->OnPutLog("한눈에 보기 리스트 화면 생성 완료", LogType::LT_OPERATE);

	CRect rect;
	GetClientRect(rect);

	m_edit_one_view.MoveWindow(rect.left + 20, 20, rect.right - 40, 310);
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
		Log_Manager->OnPutLog("모두 펼치기 버튼 클릭", LogType::LT_EVENT);
		m_btn_expand_all.SetWindowTextA("모두 접기");
		m_list_one_view.ExpandAll(true);
		m_bExpandedProcess = true;
	}
	else
	{
		Log_Manager->OnPutLog("모두 접기 버튼 클릭", LogType::LT_EVENT);
		SetExpandCloseEvent();
	}
}

void BOKOTimelineOneViewDlg::SetExpandCloseEvent()
{
	m_btn_expand_all.SetWindowTextA("모두 펼치기");
	m_list_one_view.ExpandAll(false);
	m_bExpandedProcess = false;
}


void BOKOTimelineOneViewDlg::OnBnClickedButtonOneView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_bPreviewProcess)
	{
		Log_Manager->OnPutLog("목록보기 버튼 클릭", LogType::LT_EVENT);
		m_edit_one_view.SetWindowTextA("");
		m_edit_one_view.ShowWindow(SW_HIDE);
		m_list_one_view.ShowWindow(SW_SHOW);
		m_bPreviewProcess = false;

		m_btn_preview.SetWindowTextA("미리보기");
	}
	else
	{
		Log_Manager->OnPutLog("미리보기 버튼 클릭", LogType::LT_EVENT);
		ComplexString strFullText;
		m_list_one_view.GetFullItemText(&strFullText);

		m_edit_one_view.SetWindowTextA(strFullText.GetBuffer());
		m_edit_one_view.ShowWindow(SW_SHOW);
		m_list_one_view.ShowWindow(SW_HIDE);
		m_bPreviewProcess = true;

		m_btn_preview.SetWindowTextA("목록보기");
	}
}


BOOL BOKOTimelineOneViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	return CDialogEx::PreTranslateMessage(pMsg);
}
