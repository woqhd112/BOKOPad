#pragma once
#include "CustomButton.h"
#include "DlgInterface.h"
#include "ScrollProcess.h"
#include "resource.h"

// CustomScroll 대화 상자

enum ScrollConnect
{
	SC_DISCONNECT = 0,
	SC_CONNECT
};

class CustomScroll : public CDialogEx, public DlgInterface, public ScrollProcess
{
	DECLARE_DYNAMIC(CustomScroll)

public:
	CustomScroll(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CustomScroll();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCROLL_BAR };

	bool OperateScroll(int nSBCode, int nPos);
	void ExecuteScroll(int scrollLineFlag);
	UINT OperateWheel(short zDelta);
	void SetScrollInfo(ScrollInfo info, ScrollConnect con = SC_CONNECT);
	void SetExactlyHide(bool hide);

	void ShowScroll();

private:

	CustomButton m_btn_top;
	CustomButton m_btn_bottom;
	CustomButton m_btn_middle;
	ScrollConnect m_con;

	const int SCROLL_SIZE_WIDTH = 20;
	const int BUTTON_SIZE = 16;
	const int SCROLL_AND_BUTTON_MARGIN = 2;
	const int SCROLL_AND_STICK_MARGIN = 4;
	const int BUTTON_AND_STICK_MARGIN = 1;
	const int STICK_SIZE_WIDTH = 12;
	const int STICK_DEFAULT_START_POS = SCROLL_AND_BUTTON_MARGIN + BUTTON_SIZE + BUTTON_AND_STICK_MARGIN;
	int m_nStickEndPos;

	int m_nStickMaxHeight;
	int m_nStartScrollPos;

	int m_nMoveMiddleLength;

	int m_nMiddlePointError;

	int m_nPrevMousePosY;

	int m_nPos;

	void ThisMoveWindow(int x, int y);
	void EmptySpaceClickEvent(MSG* pMsg);

	bool m_bMovingMouse;
	int m_nHideScrollCount;

	bool m_bScrollExactlyHideEvent;

public:



protected:

	virtual void Initialize();
	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedMfcbuttonTop();
	afx_msg void OnBnClickedMfcbuttonBottom();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
