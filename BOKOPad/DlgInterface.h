#pragma once
#include "CustomButton.h"
#include "CustomStatic.h"
#include "DrawButton/GdipButton.h"

#define DI_TEXT_COLOR RGB(255, 255, 255)
#define DI_BK_COLOR RGB(68, 68, 68)
#define DI_LIGHT_COLOR RGB(90, 90, 90)
#define DI_BUTTON_COLOR RGB(110, 110, 110)
#define DI_SUB_BK_COLOR RGB(140, 140, 140)
#define DI_EDIT_COLOR RGB(160, 160, 160)
#define DI_BLACK_COLOR RGB(0, 0, 0)


/*
init 순서

1. 생성자에서 부모생성자 호출

2. 생성자 내부에서 CreateFrame(); 호출

3. OninitDialog 에서 Init(); 호출

4. OnSize 재정의 하고 Sizing(nType); 호출 (생략가능 다이얼로그 상단 타이틀 각진부분 둥그렇게 만드는것)

5. OnCtlColor 재정의 하고 (mainDlg 아니면 생략가능)
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;
	최상단에 입력

6. PreTranslateMessage 재정의 하고  (mainDlg 아니면 생략가능)
	if (WM_LBUTTONDOWN == pMsg->message) 조건 안에 TitleBarActiveMove(pMsg); 호출

7. OnPaint 재정의 하고 
	DrawBackground(&dc);
	호출

8. 다이얼로그 속성에서 Border none 으로 설정 (Sizing 안하면 생략가능)
*/

enum FrameDicisionType
{
	FDT_MAIN_DLG = 0,
	FDT_LIGHT_DLG,
	FDT_SUB_DLG,
	FDT_ETC_DLG,
};

class DlgInterface
{
public:

	DlgInterface(CWnd* activeDlg, bool bMain);
	~DlgInterface();

	void InitFrame(CString strTitleText = "");
	void CreateFrame(FrameDicisionType type = FDT_SUB_DLG);
	//void DrawFrame();
	void DrawFrame(CPaintDC* in_pDC);
	void Sizing(UINT nType);
	HBRUSH CtlColors(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void SetWindowTitleText(CString text);
	void SetWindowTitleTextColor(COLORREF color);

	void TitleBarActiveMove(MSG* pMsg);

protected:

	CWnd* m_wnd;

	CRgn m_rgnWnd;

	CImage m_pngBackground1;
	CImage m_pngBackground2;
	
	/*CImage m_pngTitlebar;
	CImage m_pngLeftbar;
	CImage m_pngRightbar;
	CImage m_pngBottombar;*/

	CGdipButton m_iconButton;
	CGdipButton m_titleButton;
	CustomButton m_closeButton;
	CustomButton m_minimizeButton;
	CustomStatic m_titleTextStatic;


	CFont m_ctlFont;
	CBrush m_staticBrush;
	CBrush m_staticLightBrush;
	CBrush m_subBKBrush;
	CBrush m_editBrush;

	COLORREF m_staticTextColor;
	COLORREF m_buttonTextColor;

	CRect titleRect;

	bool m_bDrawing;

	bool m_bMainDlg;

	HGLOBAL m_hBuffer;
	Gdiplus::Bitmap* m_pBitmap;

	virtual void Initialize() = 0;
	virtual bool DragDown(MSG* pMsg) = 0;
	virtual bool DragMove(MSG* pMsg) = 0;
	virtual bool DragUp(MSG* pMsg) = 0;

	bool m_bDragProcessing;

	FrameDicisionType m_eFrameType;

private:

	bool LoadPNGResource(CImage& loadObjectIamage, UINT id, LPCTSTR pType, HMODULE hInst = NULL);

};

