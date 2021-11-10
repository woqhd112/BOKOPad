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
void BOKODragDlg::SetDragData(int& notSEQ, int& notIndex, ComplexString& notCONTENT)
{
	m_notSEQ = notSEQ;
	m_notIndex = notIndex;
	m_notCONTENT = notCONTENT;
}

BOOL BOKODragDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}
