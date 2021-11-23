#pragma once
#include "CustomEdit.h"
#include "DlgInterface.h"

// CustomEditCtrl 대화 상자

class CustomEditCtrl : public CDialogEx, public DlgInterface
{
	// 스크롤 접근
	friend class CustomEdit;
	DECLARE_DYNAMIC(CustomEditCtrl)

public:
	CustomEditCtrl(int id, int notSEQ, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CustomEditCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT };


public:

	void SetFont(CFont* pFont, BOOL bRedraw = 1);

	void ExecuteTimer();
	void SetWindowTexts(LPCTSTR lpszString);
	void GetWindowTexts(CString& lpszString);
	void MoveWindows(int x, int y, int cx, int cy);

	virtual void Initialize();


private:

	CustomEdit m_edit_custom;
	CustomScroll m_scroll;

	int m_nEditID;
	int m_nNotSEQ;

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
