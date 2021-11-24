#pragma once
#include "DlgInterface.h"
#include "CustomEditCtrl.h"

// BOKOScenarioAddDlg 대화 상자

class BOKOScenarioAddDlg : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(BOKOScenarioAddDlg)

public:
	BOKOScenarioAddDlg(ComplexString* inputText, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOScenarioAddDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_ADD };

public:


private:

	CustomEditCtrl m_edit_input;
	CStatic m_stt_scenario;
	CustomButton m_btn_input_scenario;
	ComplexString* m_strInputText;

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);
	virtual void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedMfcbuttonScenarioAdd();
};
