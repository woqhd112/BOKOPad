#pragma once
#include "DlgInterface.h"

// BOKOSelectExtentionDlg 대화 상자

class BOKOSelectExtentionDlg : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(BOKOSelectExtentionDlg)

public:
	BOKOSelectExtentionDlg(int* selectExtentionType, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOSelectExtentionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SELECT_EXPORT_TIMELINE_EXTENTION };


private:

	CGdipButton m_btn_notepad;
	CGdipButton m_btn_word;
	CGdipButton m_btn_excel;

	CMFCButton m_btn_notepad2;
	CMFCButton m_btn_excel2;
	CMFCButton m_btn_word2;

	int* m_nSelectExtentionType;

public:




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
