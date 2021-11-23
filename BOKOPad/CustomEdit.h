#pragma once
#include "ComplexUniqueQueue.h"
#include "CustomScroll.h"

// CustomEdit

class CustomEdit : public CEdit
{
	DECLARE_DYNAMIC(CustomEdit)

public:

	CustomEdit();
	CustomEdit(int notSEQ);
	virtual ~CustomEdit();

	void SetNotSEQ(int notSEQ);
	void ScrollSync();

	void SetScrollEditPos(int scrollUpAndDown);


private:

	int m_nPrevLineCount;

	int m_notSEQ;
	ComplexUniqueSwapQueue<CString> m_editQueue;
	bool m_bSwapping;

public:

	void ExecuteTimer();

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
