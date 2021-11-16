// BOKODragDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKODragDlg.h"
#include "afxdialogex.h"


// BOKODragDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKODragDlg, CDialogEx)

BOKODragDlg::BOKODragDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DRAG_DIALOG, pParent)
{
	
}

BOKODragDlg::~BOKODragDlg()
{
}

void BOKODragDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DRAG_EDIT, m_edit_drag);
	DDX_Control(pDX, IDC_STATIC_DRAG_STATIC, m_stt_drag);
}


BEGIN_MESSAGE_MAP(BOKODragDlg, CDialogEx)
END_MESSAGE_MAP()


// BOKODragDlg 메시지 처리기
void BOKODragDlg::SetDragData(int noteCount)
{
	CString text;
	text.Format("%d", noteCount);
	m_stt_drag.SetWindowTextA(text);
}

BOOL BOKODragDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL BOKODragDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_stt_drag.MoveWindow(0, 0, 30, 30);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
