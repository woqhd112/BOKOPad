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
init ����

1. �����ڿ��� �θ������ ȣ��

2. ������ ���ο��� CreateFrame(); ȣ��

3. OninitDialog ���� Init(); ȣ��

4. OnSize ������ �ϰ� Sizing(nType); ȣ�� (�������� ���̾�α� ��� Ÿ��Ʋ �����κ� �ձ׷��� ����°�)

5. OnCtlColor ������ �ϰ� (mainDlg �ƴϸ� ��������)
	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;
	�ֻ�ܿ� �Է�

6. PreTranslateMessage ������ �ϰ�  (mainDlg �ƴϸ� ��������)
	if (WM_LBUTTONDOWN == pMsg->message) ���� �ȿ� TitleBarActiveMove(pMsg); ȣ��

7. OnPaint ������ �ϰ� 
	DrawBackground(&dc);
	ȣ��

8. ���̾�α� �Ӽ����� Border none ���� ���� (Sizing ���ϸ� ��������)
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

