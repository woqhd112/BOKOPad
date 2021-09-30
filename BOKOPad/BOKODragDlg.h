#pragma once


// BOKODragDlg 대화 상자

class BOKODragDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BOKODragDlg)

public:
	BOKODragDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKODragDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DRAG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
