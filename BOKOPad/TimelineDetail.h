#pragma once
#include "DlgInterface.h"

// TimelineDetail 대화 상자

class TimelineDetail : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(TimelineDetail)

public:
	TimelineDetail(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TimelineDetail();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMELINE_DETAIL };


public:

	void SetText(ComplexString thisText, ComplexString targetText);

private:

	CEdit m_edit_detail_this;
	CEdit m_edit_detail_target;

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
