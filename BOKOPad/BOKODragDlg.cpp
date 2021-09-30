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
}


BEGIN_MESSAGE_MAP(BOKODragDlg, CDialogEx)
END_MESSAGE_MAP()


// BOKODragDlg 메시지 처리기
