#pragma once


// CustomStatic

class CustomStatic : public CStatic
{
	DECLARE_DYNAMIC(CustomStatic)

public:
	CustomStatic();
	virtual ~CustomStatic();

private:

	CBrush m_brush;

public:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


