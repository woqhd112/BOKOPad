#pragma once
#include "resource.h"

// BOKOScenarioDetailDlg 대화 상자

class BOKOScenarioDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BOKOScenarioDetailDlg)

public:
	BOKOScenarioDetailDlg(int thisIndex, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOScenarioDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_TIMELINE };

private:

	int m_scenarioIndex;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
