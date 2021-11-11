#include "pch.h"
#include "DlgInterface.h"
#include "resource.h"

static int s_ctlID = 20000;

DlgInterface::DlgInterface(CWnd* activeDlg)
	: m_wnd(activeDlg)
{
	m_staticBrush.CreateSolidBrush(DI_BK_COLOR);
	m_editBrush.CreateSolidBrush(DI_SUB_BK_COLOR);

	m_staticTextColor = DI_TEXT_COLOR;
	m_buttonTextColor = DI_TEXT_COLOR;
	m_bDrawing = false;
}

DlgInterface::~DlgInterface()
{
	m_staticBrush.DeleteObject();
	m_editBrush.DeleteObject();
}

void DlgInterface::CreateFrame()
{
	TCHAR* szTitleFileName1 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\title.png";
	m_pngTitlebar.Load(szTitleFileName1);

	TCHAR* szTitleFileName2 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\left.png";
	m_pngLeftbar.Load(szTitleFileName2);

	TCHAR* szTitleFileName3 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\right.png";
	m_pngRightbar.Load(szTitleFileName3);

	TCHAR* szTitleFileName4 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\bottom.png";
	m_pngBottombar.Load(szTitleFileName4);

	TCHAR* szTitleFileName5 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\test2.png";
	m_pngBackground.Load(szTitleFileName5);

}

void DlgInterface::SetWindowTitleText(CString text)
{
	m_titleTextStatic.SetWindowTextA(text);
}

void DlgInterface::SetWindowTitleTextColor(COLORREF color)
{
	m_staticTextColor = color;
	m_wnd->Invalidate();
}

void DlgInterface::Init(CString strTitleText)
{
	m_wnd->ModifyStyle(WS_BORDER, 0);
	m_closeButton.Create("X", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);
	m_titleTextStatic.Create(strTitleText, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);
	m_iconButton.Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);

	//m_titleButton.Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);

	m_ctlFont.CreateFontA(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		"고딕");

	CRect rect;
	//m_wnd->GetWindowRect(rect);
	m_wnd->GetClientRect(rect);
	m_closeButton.MoveWindow(rect.right - 30, 5, 20, 20);
	m_titleTextStatic.MoveWindow(rect.left + 35, 5, rect.Width() - 30 - 20 - 35, 20);
	m_iconButton.MoveWindow(5, 5, 16, 16);
	//m_titleButton.MoveWindow(-20, 0, rect.Width() + 100, 32);

	m_titleTextStatic.SetFont(&m_ctlFont);
	m_closeButton.Initialize(DI_BK_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, true);
	m_closeButton.SetTextColor(m_buttonTextColor);
	m_iconButton.LoadStdImage(IDB_PNG_ICON2, "PNG");
	m_iconButton.LoadHovImage(IDB_PNG_ICON2, "PNG");
	m_iconButton.LoadAltImage(IDB_PNG_ICON2, "PNG");

	//m_titleButton.LoadStdImage(IDB_PNG_BK_TITLE, "PNG");
	//m_titleButton.LoadHovImage(IDB_PNG_BK_TITLE, "PNG");
	//m_titleButton.LoadAltImage(IDB_PNG_BK_TITLE, "PNG");

	m_closeButton.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_iconButton.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_titleTextStatic.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	//m_titleButton.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	//m_titleButton.BringWindowToTop();
	m_iconButton.BringWindowToTop();
	m_titleTextStatic.BringWindowToTop();
	m_closeButton.BringWindowToTop();

	titleRect.left = rect.left;
	titleRect.top = rect.top;
	titleRect.right = rect.right;
	titleRect.bottom = rect.top + 32;
}

void DlgInterface::TitleBarActiveMove(MSG* pMsg)
{
	CPoint pt = pMsg->pt;
	m_wnd->ScreenToClient(&pt);
	if (PtInRect(titleRect, pt))
	{
		m_wnd->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));
	}
}

HBRUSH DlgInterface::CtlColors(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == m_titleTextStatic.GetDlgCtrlID())
		{
			pDC->SetTextColor(m_staticTextColor);
			pDC->SetBkMode(TRANSPARENT);
			CBrush B;
			B.CreateStockObject(NULL_BRUSH);
			return B;
		}
	}

	return NULL;
}

void DlgInterface::DrawBackground(CPaintDC* in_pDC)
{
	CBitmap bmp;
	BITMAP bmpInfo;
	CDC memDC;

	bmp.Attach(m_pngBackground);
	bmp.GetBitmap(&bmpInfo);

	BOOL ret = memDC.CreateCompatibleDC(in_pDC);
	memDC.SelectObject(bmp);

	in_pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	bmp.Detach();
	bmp.DeleteObject();
}

void DlgInterface::DrawFrame()
{
	if (!(GetWindowLong(m_wnd->GetSafeHwnd(), GWL_STYLE) & WS_CAPTION))
		return;

	if (m_bDrawing)
		return;

	int x = 0, y = 0, x2 = 0, y2 = 0, cx = 0, cy = 0;
	CRect rc;
	m_wnd->GetWindowRect(rc);

	CWindowDC dc(m_wnd);

	CDC BufferDC, tmpDC;

	BufferDC.CreateCompatibleDC(&dc);
	tmpDC.CreateCompatibleDC(&dc);

	CBitmap bmpBuffer, *pOldBitmap1;
	HGDIOBJ hgdiobj;

	bmpBuffer.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	pOldBitmap1 = (CBitmap*)BufferDC.SelectObject(&bmpBuffer);

	// title
	cx = m_pngTitlebar.GetWidth();
	cy = m_pngTitlebar.GetHeight();

	hgdiobj = tmpDC.SelectObject(m_pngTitlebar);
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);

	// left
	x = 0;
	y = m_pngTitlebar.GetHeight();
	cx = m_pngLeftbar.GetWidth();
	cy = m_pngLeftbar.GetHeight();

	tmpDC.SelectObject(m_pngLeftbar);
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);

	// right
	x = rc.Width() - 10;
	y = m_pngTitlebar.GetHeight();
	cx = m_pngRightbar.GetWidth();
	cy = m_pngRightbar.GetHeight();

	tmpDC.SelectObject(m_pngRightbar);
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);

	// bottom
	x = 0;
	y = rc.Height() - 5;
	cx = m_pngBottombar.GetWidth();
	cy = m_pngBottombar.GetHeight();

	tmpDC.SelectObject(m_pngBottombar);
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);

	tmpDC.SelectObject(hgdiobj);
	tmpDC.DeleteDC();

	BufferDC.SetBkMode(TRANSPARENT);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &BufferDC, 0, 0, SRCCOPY);

	BufferDC.SelectObject(pOldBitmap1);
	BufferDC.DeleteDC();

	m_wnd->Invalidate(FALSE);
	m_bDrawing = true;
}

void DlgInterface::Sizing(UINT nType)
{
	//if (GetWindowLong(m_wnd->GetSafeHwnd(), GWL_STYLE) & WS_CAPTION)
	//{
		CRect rc;
		m_wnd->GetWindowRect(rc);

		int width = rc.Width() + 1;
		int height = rc.Height() + 1;

		int x = 0, y = 0;
		int round = 14;
		CRgn rgnA, rgnB;

		if (nType == SIZE_MAXIMIZED)
		{
			x = 4;
			y = 4;
			width -= 4;
			height -= 4;
		}

		if ((HRGN)m_rgnWnd != NULL)
			m_rgnWnd.DeleteObject();

		m_rgnWnd.CreateRectRgn(x, y, width, height);

		rgnA.CreateEllipticRgn(x, y, round, round);
		rgnB.CreateRectRgn(x, y, round / 2, round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		rgnA.CreateEllipticRgn(width - round, y, width, round);
		rgnB.CreateRectRgn(width - round / 2, y, width, round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		m_wnd->SetWindowRgn((HRGN)m_rgnWnd, TRUE);
	//}
}