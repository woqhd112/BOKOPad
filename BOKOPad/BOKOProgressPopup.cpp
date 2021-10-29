// BOKOProgressPopup.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOProgressPopup.h"
#include "afxdialogex.h"


// BOKOProgressPopup 대화 상자

IMPLEMENT_DYNAMIC(BOKOProgressPopup, CDialogEx)

BOKOProgressPopup::BOKOProgressPopup(int itemCount, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROGRESS_POPUP, pParent)
{
	m_nItemCount = itemCount;
	m_posCount = 0;
}

BOKOProgressPopup::~BOKOProgressPopup()
{

}

void BOKOProgressPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_progress);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_FILE_NAME, m_stt_file_name);
}


BEGIN_MESSAGE_MAP(BOKOProgressPopup, CDialogEx)
END_MESSAGE_MAP()


// BOKOProgressPopup 메시지 처리기


BOOL BOKOProgressPopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//m_progress.SetRange(m_nStartRange, m_nEndRange);
	m_progress.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool BOKOProgressPopup::AddProgressPos()
{
	m_progress.SetPos((++m_posCount) * 100 / m_nItemCount);

	if (m_posCount == m_nItemCount)
	{
		return true;
	}

	return false;
}

void BOKOProgressPopup::Success()
{
	m_progress.SetPos(100);
}

void BOKOProgressPopup::SetAnalyzeFileName(ComplexString strFileName)
{
	m_strCurrentFileName = strFileName;
	CString strText;
	strText.Format("%s 분석 중...", strFileName.GetBuffer());
	m_stt_file_name.SetWindowTextA(strText);
}
