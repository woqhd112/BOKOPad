#pragma once
#include "resource.h"
#include "DlgInterface.h"

// BOKOProgressPopup 대화 상자
#define ONE_DOT "."
#define TWO_DOT ".."
#define THREE_DOT "..."


class BOKOProgressPopup : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(BOKOProgressPopup)

public:
	BOKOProgressPopup(bool* processing, int itemCount, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOProgressPopup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PROGRESS_POPUP };

private:

	CProgressCtrl m_progress;
	CStatic m_stt_file_name;

	ComplexString m_strCurrentDot;
	ComplexString m_strCurrentFileName;

	int m_nItemCount;

	int m_posCount;

	bool* m_bProcessing;

public:

	bool AddProgressPos();
	void SetAnalyzeFileName(ComplexString strFileName);
	void Success();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
};
