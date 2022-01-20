
// BOKOPadDlg.h: 헤더 파일
//

#pragma once
#include "DlgInterface.h"
#include "CustomMenu.h"
#include "ScenarioListCtrl.h"

void OnLogViewCallBackFunc();
void OnExplanationViewCallBackFunc();
void OnDBConsoleCallBackFunc();


// CBOKOPadDlg 대화 상자
class CBOKOPadDlg : public CDialogEx, public DlgInterface
{
// 생성입니다.
public:

	CBOKOPadDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CBOKOPadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BOKOPAD_DIALOG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual void Initialize();

public:


private:


	PadOptionVO m_mainOptionData;
	ComplexVector<ScenarioListVO> m_loadScenarioList;

	CustomMenu m_menu;
	ScenarioListCtrl m_scenario;
// 구현입니다.

private:


protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnScenarioExport();
	afx_msg void OnScenarioImport();
	afx_msg void OnProgramClose();
	afx_msg void OnLogView();
	afx_msg void OnExplanationView();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

