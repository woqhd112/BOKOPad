#pragma once
#include "ComplexUniqueQueue.h"

// CustomEdit

class CustomEdit : public CEdit
{
	DECLARE_DYNAMIC(CustomEdit)

public:
	CustomEdit(int notSEQ);
	virtual ~CustomEdit();

private:

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
};


