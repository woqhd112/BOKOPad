#include "pch.h"
#include "DlgInterface.h"
#include "DrawButton/CGdiPlusBitmap.h"
#include "resource.h"

static int s_ctlID = 20000;

DlgInterface::DlgInterface(CWnd* activeDlg, bool bMain)
	: m_wnd(activeDlg)
{
	m_staticBrush.CreateSolidBrush(DI_BK_COLOR);
	m_editBrush.CreateSolidBrush(DI_EDIT_COLOR);
	m_subBKBrush.CreateSolidBrush(DI_SUB_BK_COLOR);

	m_staticTextColor = DI_TEXT_COLOR;
	m_buttonTextColor = DI_TEXT_COLOR;
	m_bDrawing = false;
	m_bMainDlg = bMain;
	m_bDragProcessing = false;
}

DlgInterface::~DlgInterface()
{
	m_staticBrush.DeleteObject();
	m_editBrush.DeleteObject();
	m_subBKBrush.DeleteObject();

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}

}

void DlgInterface::CreateFrame(FrameDicisionType type)
{
#ifdef PATH_RESOURCE
	/*TCHAR* szTitleFileName1 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\title.png";
	m_pngTitlebar.Load(szTitleFileName1);

	TCHAR* szTitleFileName2 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\left.png";
	m_pngLeftbar.Load(szTitleFileName2);

	TCHAR* szTitleFileName3 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\right.png";
	m_pngRightbar.Load(szTitleFileName3);

	TCHAR* szTitleFileName4 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\bottom.png";
	m_pngBottombar.Load(szTitleFileName4);*/

	TCHAR* szTitleFileName5 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\test2.png";
	m_pngBackground1.Load(szTitleFileName5);

	TCHAR* szTitleFileName6 = "C:\\Users\\user\\Desktop\\워크스페이스\\work_vs2017\\BOKOPad\\BOKOPad\\res\\test.png";
	m_pngBackground2.Load(szTitleFileName6);

#else
	if (type == FDT_MAIN_DLG)
	{
		LoadPNGResource(m_pngBackground2, IDB_PNG_NOTE_TABLE, "PNG");
	}
	else if (type == FDT_SUB_DLG)
	{
		LoadPNGResource(m_pngBackground1, IDB_PNG_BK, "PNG");
		LoadPNGResource(m_pngBackground2, IDB_PNG_BK_SUB, "PNG");
	}
	else if (type == FDT_ETC_DLG)
	{
	}
#endif

	m_eFrameType = type;
}

bool DlgInterface::LoadPNGResource(CImage& loadObjectIamage, UINT id, LPCTSTR pType, HMODULE hInst)
{
	LPCTSTR pName = MAKEINTRESOURCE(id);

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return false;

	m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				if (SUCCEEDED(loadObjectIamage.Load(pStream)))
				{
					pStream->Release();
					if (m_pBitmap)
					{
						if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
							return true;

						delete m_pBitmap;
						m_pBitmap = NULL;
					}
				}
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return false;
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

void DlgInterface::InitFrame(CString strTitleText)
{
	m_wnd->ModifyStyle(WS_BORDER, 0);
	m_closeButton.Create("X", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);
	//m_minimizeButton.Create("─", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);
	m_titleTextStatic.Create(strTitleText, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);
	m_iconButton.Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), m_wnd, s_ctlID++);

	m_ctlFont.CreateFontA(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		"고딕");

	CRect rect;
	m_wnd->GetClientRect(rect);
	m_closeButton.MoveWindow(rect.right - 30, 5, 20, 20);
	//m_minimizeButton.MoveWindow(rect.right - 30 - 30, 5, 20, 20);
	m_titleTextStatic.MoveWindow(rect.left + 35, 5, rect.Width() - 30 - 20 - 35, 20);
	m_iconButton.MoveWindow(5, 5, 16, 16);

	m_titleTextStatic.SetFont(&m_ctlFont);
	m_closeButton.Initialize(DI_BK_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, CBT_CLOSE);
	//m_minimizeButton.Initialize(DI_BK_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD, CBT_MINIMIZE);
	m_closeButton.SetTextColor(m_buttonTextColor);
	//m_minimizeButton.SetTextColor(m_buttonTextColor);
	m_iconButton.LoadStdImage(IDB_PNG_ICON2, "PNG");
	m_iconButton.LoadHovImage(IDB_PNG_ICON2, "PNG");
	m_iconButton.LoadAltImage(IDB_PNG_ICON2, "PNG");

	titleRect.left = rect.left;
	titleRect.top = rect.top;
	titleRect.right = rect.right;
	titleRect.bottom = rect.top + 32;

	if (m_bMainDlg == false)
	{
		m_iconButton.ShowWindow(SW_HIDE);
		m_closeButton.ShowWindow(SW_HIDE);
		//m_minimizeButton.ShowWindow(SW_HIDE);
		m_titleTextStatic.ShowWindow(SW_HIDE);
	}
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

void DlgInterface::DrawFrame(CPaintDC* in_pDC)
{
	if ((m_eFrameType == FDT_MAIN_DLG) || (m_eFrameType == FDT_SUB_DLG))
	{
		CBitmap bmp;
		BITMAP bmpInfo;
		CDC memDC;

		bmp.Attach(m_bMainDlg ? m_pngBackground1 : m_pngBackground2);
		bmp.GetBitmap(&bmpInfo);

		BOOL ret = memDC.CreateCompatibleDC(in_pDC);
		memDC.SelectObject(bmp);

		in_pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC, 0, 0, SRCCOPY);

		memDC.DeleteDC();
		bmp.Detach();
		bmp.DeleteObject();
	}
	else if (m_eFrameType == FDT_ETC_DLG)
	{
		CRect rect;
		m_wnd->GetClientRect(rect);

		in_pDC->FillSolidRect(rect, DI_EDIT_COLOR);
	}
}

//void DlgInterface::DrawFrame()
//{
//	if (!(GetWindowLong(m_wnd->GetSafeHwnd(), GWL_STYLE) & WS_CAPTION))
//		return;
//
//	if (m_bDrawing)
//		return;
//
//	int x = 0, y = 0, x2 = 0, y2 = 0, cx = 0, cy = 0;
//	CRect rc;
//	m_wnd->GetWindowRect(rc);
//
//	CWindowDC dc(m_wnd);
//
//	CDC BufferDC, tmpDC;
//
//	BufferDC.CreateCompatibleDC(&dc);
//	tmpDC.CreateCompatibleDC(&dc);
//
//	CBitmap bmpBuffer, *pOldBitmap1;
//	HGDIOBJ hgdiobj;
//
//	bmpBuffer.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
//	pOldBitmap1 = (CBitmap*)BufferDC.SelectObject(&bmpBuffer);
//
//	// title
//	cx = m_pngTitlebar.GetWidth();
//	cy = m_pngTitlebar.GetHeight();
//
//	hgdiobj = tmpDC.SelectObject(m_pngTitlebar);
//	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);
//
//	// left
//	x = 0;
//	y = m_pngTitlebar.GetHeight();
//	cx = m_pngLeftbar.GetWidth();
//	cy = m_pngLeftbar.GetHeight();
//
//	tmpDC.SelectObject(m_pngLeftbar);
//	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);
//
//	// right
//	x = rc.Width() - 10;
//	y = m_pngTitlebar.GetHeight();
//	cx = m_pngRightbar.GetWidth();
//	cy = m_pngRightbar.GetHeight();
//
//	tmpDC.SelectObject(m_pngRightbar);
//	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);
//
//	// bottom
//	x = 0;
//	y = rc.Height() - 5;
//	cx = m_pngBottombar.GetWidth();
//	cy = m_pngBottombar.GetHeight();
//
//	tmpDC.SelectObject(m_pngBottombar);
//	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, cx, cy, SRCCOPY);
//
//	tmpDC.SelectObject(hgdiobj);
//	tmpDC.DeleteDC();
//
//	BufferDC.SetBkMode(TRANSPARENT);
//
//	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &BufferDC, 0, 0, SRCCOPY);
//
//	BufferDC.SelectObject(pOldBitmap1);
//	BufferDC.DeleteDC();
//
//	m_wnd->Invalidate(FALSE);
//	m_bDrawing = true;
//}

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

		// 좌측 상단
		rgnA.CreateEllipticRgn(x, y, round, round);
		rgnB.CreateRectRgn(x, y, round / 2, round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		// 우측 상단
		rgnA.CreateEllipticRgn(width - round, y, width, round);
		rgnB.CreateRectRgn(width - round / 2, y, width, round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		// 좌측 하단
		rgnA.CreateEllipticRgn(x, height, round, height - round);
		rgnB.CreateRectRgn(x, height, round / 2, height - round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		// 우측 하단
		rgnA.CreateEllipticRgn(width - round, height, width, height - round);
		rgnB.CreateRectRgn(width - round / 2, height, width, height - round / 2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		m_wnd->SetWindowRgn((HRGN)m_rgnWnd, TRUE);
	//}
}