// BOKOOptionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOOptionDlg.h"
#include "File/FileProcess.h"
#include "BOKOOptionScenarioExportDlg.h"
#include "afxdialogex.h"


// BOKOOptionDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOOptionDlg, CDialogEx)

BOKOOptionDlg::BOKOOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTION, pParent)
{
	
}

BOKOOptionDlg::~BOKOOptionDlg()
{
}

void BOKOOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_EXPORT_FILE, m_btn_export_file);
	DDX_Control(pDX, IDC_RADIO_TEST1, m_radio_test1);
	DDX_Control(pDX, IDC_RADIO_TEST2, m_radio_test2);
	DDX_Control(pDX, IDC_BUTTON_IMPORT_FILE, m_btn_import_file);
}


BEGIN_MESSAGE_MAP(BOKOOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_FILE, &BOKOOptionDlg::OnBnClickedButtonExportFile)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_FILE, &BOKOOptionDlg::OnBnClickedButtonImportFile)
END_MESSAGE_MAP()


// BOKOOptionDlg 메시지 처리기


BOOL BOKOOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	bool bInit = false;

	CURSOR_WAIT;
	if (Scenario_DB_Manager->SelectAllPadOption(&m_mainOptionData))
		bInit = true;

	if (bInit)
	{
		int test1 = m_mainOptionData.GetTest1();
		int test2 = m_mainOptionData.GetTest2();

		if (test1)
			m_radio_test1.SetCheck(TRUE);

		if (test2)
			m_radio_test2.SetCheck(TRUE);

		char dir_[2048] = { 0 };
		::GetModuleFileNameA(NULL, dir_, 2048);

		m_workPath = dir_;
		m_workPath.Remove(DB_MODULE_NANE);
	}

	CURSOR_ARROW;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOOptionDlg::OnBnClickedButtonExportFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFolderPickerDialog fd(m_workPath.GetBuffer(), OFN_FILEMUSTEXIST, NULL, 0);
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

	CURSOR_WAIT;

	CString strFullPath = fd.GetPathName();

	ComplexVector<NoteInformationVO> loadNoteInformationContainer;
	if (Scenario_DB_Manager->SelectInSceSEQNoteInformation(outputScenario.GetSceSEQ(), &loadNoteInformationContainer))
	{
		ComplexVector<NoteInformationVO>::iterator iter = loadNoteInformationContainer.begin();
		while (iter != loadNoteInformationContainer.end())
		{
			NoteInformationVO noteInform = iter->value;

			ComplexString strWriteAnsiContent, strConvertUTF8Content;
			strWriteAnsiContent = noteInform.GetNotCONTENT();
			//ComplexUtilProcess::ANSIToUTF8(strConvertUTF8Content, strWriteAnsiContent);
			//ComplexUtilProcess::ExportFile(strConvertUTF8Content, strFullPath.GetBuffer());
			ComplexUtilProcess::ExportFile(strWriteAnsiContent, strFullPath.GetBuffer());

			iter++;
		}
	}
	else
	{
		CURSOR_ARROW;
		MessageBox("데이터 불러오기에 실패하였습니다.");
		return;
	}

	CURSOR_ARROW;
	MessageBox("파일 내보내기에 성공하였습니다.");
}


void BOKOOptionDlg::OnBnClickedButtonImportFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	CURSOR_WAIT;
	POSITION pos = fd.GetStartPosition();

	while (pos != NULL)
	{
		ComplexString strPathName = fd.GetNextPathName(pos).GetBuffer();
		ComplexString strReadUTF8Content, strConvertAnsiContent;
		ComplexUtilProcess::ImportFile(strReadUTF8Content, strPathName);
		//ComplexUtilProcess::UTF8ToANSI(strConvertAnsiContent, strReadUTF8Content);

		NoteInformationVO insertNote(0, outputScenario.GetSceSEQ(), false, false, strReadUTF8Content);
		if (Scenario_DB_Manager->InsertNoteInformation(insertNote) == false)
		{
			CURSOR_ARROW;
			MessageBox("데이터 저장에 실패하였습니다.");
			return;
		}
	}

	CURSOR_ARROW;
	MessageBox("파일 불러오기에 성공하였습니다.");
}
