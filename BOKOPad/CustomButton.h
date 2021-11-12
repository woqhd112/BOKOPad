﻿#pragma once


// CustomButton

enum CustomButtonType
{
	CBT_DEFAULT = 0,
	CBT_CLOSE,
	CBT_MINIMIZE
};

class CustomButton : public CMFCButton
{
	DECLARE_DYNAMIC(CustomButton)

public:
	CustomButton();
	virtual ~CustomButton();

	void Initialize(COLORREF color, FlatStyle style, CString strFontName = _T("고딕"), int nFontSize = 10, int nFontFlags = FW_NORMAL, CustomButtonType buttonType = CBT_DEFAULT);

private:

	bool m_bTrackMouse;
	bool m_bUseMouseBkGroundColorEvent;
	CustomButtonType m_buttonType;

	CFont m_thisFont;
	COLORREF m_defaultColor;
	COLORREF m_hoverColor;
	COLORREF m_downColor;

public:


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


