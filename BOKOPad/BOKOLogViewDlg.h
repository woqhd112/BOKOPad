#pragma once
#include "DlgInterface.h"

// BOKOLogViewDlg 대화 상자

class BOKOLogViewDlg : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(BOKOLogViewDlg)

public:
	BOKOLogViewDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOLogViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOG_VIEW };

private:

	CListCtrl m_list_log_count;
	CListCtrl m_list_log_view;
	CComboBox m_combo_log_divide;
	CDateTimeCtrl m_date_start_date;
	CDateTimeCtrl m_date_last_date;
	CustomButton m_btn_execute;

	CString m_startDate;
	CString m_endDate;
	int m_selectComboIndex;

	ComplexMap<ComplexString, ComplexVector<ComplexString>>* m_logMap;

	ComplexVector<ComplexString> m_allLog;
	ComplexVector<ComplexString> m_eventLog;
	ComplexVector<ComplexString> m_processLog;
	ComplexVector<ComplexString> m_operateLog;

	void Initialize();
	void LoadLog();
	void AnalyzeLogData();
	void InsertCountList(int index, ComplexString type, ComplexString text);
	void InsertViewList(int index, ComplexString type, ComplexString time, ComplexString content);

public:



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLogDivide();
	afx_msg void OnCbnDropdownComboLogDivide();
	afx_msg void OnBnClickedButtonExecuteLog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
