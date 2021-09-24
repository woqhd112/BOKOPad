
// BOKOPadDlg.h: 헤더 파일
//

#pragma once
#include "DlgController.h"

// CBOKOPadDlg 대화 상자
class CBOKOPadDlg : public CDialogEx
{
// 생성입니다.
public:
	CBOKOPadDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CBOKOPadDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOKOPAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


public:


private:

	DlgController* m_controller;
	PadOptionVO m_mainOptionData;
	ComplexVector<ScenarioListVO> m_loadScenarioList;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
