// TimelineDetail.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "TimelineDetail.h"
#include "afxdialogex.h"


// TimelineDetail 대화 상자

IMPLEMENT_DYNAMIC(TimelineDetail, CDialogEx)

TimelineDetail::TimelineDetail(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE_DETAIL, pParent)
{
	Log_Manager->OnPutLog("TimelineDetail 생성자 호출", LogType::LT_PROCESS);
}

TimelineDetail::~TimelineDetail()
{
	Log_Manager->OnPutLog("TimelineDetail 소멸자 호출", LogType::LT_PROCESS);
}

void TimelineDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIMELINE_DETAIL_THIS, m_edit_detail_this);
	DDX_Control(pDX, IDC_EDIT_TIMELINE_DETAIL_TARGET, m_edit_detail_target);
}


BEGIN_MESSAGE_MAP(TimelineDetail, CDialogEx)
END_MESSAGE_MAP()


// TimelineDetail 메시지 처리기


BOOL TimelineDetail::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_edit_detail_this.MoveWindow(20, 20, 170, 150);
	m_edit_detail_target.MoveWindow(200, 20, 170, 150);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void TimelineDetail::SetText(ComplexString thisText, ComplexString targetText)
{
	m_edit_detail_this.SetWindowTextA(thisText.GetBuffer());
	m_edit_detail_target.SetWindowTextA(targetText.GetBuffer());
	Log_Manager->OnPutLog("타임라인 상세화면 출력 완료", LogType::LT_EVENT);
}