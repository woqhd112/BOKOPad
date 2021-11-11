#pragma once
#include "CustomButton.h"
#include "CustomStatic.h"
#include "DrawButton/GdipButton.h"

#define DI_TEXT_COLOR RGB(255, 255, 255)
#define DI_BK_COLOR RGB(68, 68, 68)
#define DI_BUTTON_COLOR RGB(110, 110, 110)
#define DI_SUB_BK_COLOR RGB(140, 140, 140)


/*
init 순서

1. 생성자에서 부모생성자 호출

2. 생성자 내부에서 CreateFrame(); 호출

3. OninitDialog 에서 Init(); 호출

4. OnSize 재정의 하고 Sizing(nType); 호출

5. OnCtlColor 재정의 하고 
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		hbr = returnHBR;
	입력

6. PreTranslateMessage 재정의 하고 
	if (WM_LBUTTONDOWN == pMsg->message) 조건 안에 TitleBarActiveMove(pMsg); 호출

7. OnPaint 재정의 하고 
	DrawBackground(&dc);
	호출

8. 다이얼로그 속성에서 Border none 으로 설정
*/

class DlgInterface
{
public:

	DlgInterface(CWnd* activeDlg);
	~DlgInterface();

	void Init(CString strTitleText);
	void CreateFrame();
	void DrawFrame();
	void DrawBackground(CPaintDC* in_pDC);
	void Sizing(UINT nType);
	HBRUSH CtlColors(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void SetWindowTitleText(CString text);
	void SetWindowTitleTextColor(COLORREF color);

	void TitleBarActiveMove(MSG* pMsg);

protected:

	CWnd* m_wnd;

	CRgn m_rgnWnd;

	CImage m_pngBackground;
	
	CImage m_pngTitlebar;
	CImage m_pngLeftbar;
	CImage m_pngRightbar;
	CImage m_pngBottombar;

	CGdipButton m_iconButton;
	CGdipButton m_titleButton;
	CustomButton m_closeButton;
	CustomStatic m_titleTextStatic;


	CFont m_ctlFont;
	CBrush m_staticBrush;
	CBrush m_editBrush;

	COLORREF m_staticTextColor;
	COLORREF m_buttonTextColor;

	CRect titleRect;

	bool m_bDrawing;
};

