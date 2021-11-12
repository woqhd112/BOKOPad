// CustomButton.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomButton.h"


// CustomButton

IMPLEMENT_DYNAMIC(CustomButton, CMFCButton)

CustomButton::CustomButton()
{
	m_bTrackMouse = false;
	m_bUseMouseBkGroundColorEvent = true;
	m_buttonType = CBT_DEFAULT;
	m_defaultColor = RGB(68, 68, 68);
	m_hoverColor = RGB(58, 58, 58);
	m_downColor = RGB(48, 48, 48);
}

CustomButton::~CustomButton()
{
}


BEGIN_MESSAGE_MAP(CustomButton, CMFCButton)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CustomButton 메시지 처리기


void CustomButton::Initialize(COLORREF color, FlatStyle style, CString strFontName /*= _T("고딕")*/, int nFontSize /*= 10*/, int nFontFlags /*= FW_NORMAL*/, CustomButtonType buttonType)
{
	this->EnableWindowsTheming(FALSE);
	this->SetFaceColor(color);
	this->m_bDrawFocus = FALSE;

	m_thisFont.CreateFontA(nFontSize, 0, 0, 0, nFontFlags, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		strFontName);

	int rColor = GetRValue(color);
	int gColor = GetGValue(color);
	int bColor = GetBValue(color);

	m_defaultColor = color;
	m_hoverColor = RGB(rColor - 10, gColor - 10, bColor - 10);
	m_downColor = RGB(rColor - 20, gColor - 20, bColor - 20);

	this->SetFont(&m_thisFont);
	this->m_nFlatStyle = style;
	this->m_buttonType = buttonType;
}

void CustomButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bUseMouseBkGroundColorEvent)
	{
		this->SetFaceColor(m_hoverColor);
	}

	CMFCButton::OnMouseHover(nFlags, point);
}


void CustomButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (!m_bTrackMouse)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 1;
		if (TrackMouseEvent(&tme))
		{
			m_bTrackMouse = true;
		}
	}

	CMFCButton::OnMouseMove(nFlags, point);
}


void CustomButton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bUseMouseBkGroundColorEvent)
	{
		this->SetFaceColor(m_defaultColor);
	}

	m_bTrackMouse = false;
	CMFCButton::OnMouseLeave();
}


void CustomButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bUseMouseBkGroundColorEvent)
	{
		this->SetFaceColor(m_downColor);
	}

	CMFCButton::OnLButtonDown(nFlags, point);
}


void CustomButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bUseMouseBkGroundColorEvent)
	{
		this->SetFaceColor(m_hoverColor);
	}

	if (m_buttonType == CBT_CLOSE)
	{
		GetParent()->SendMessage(WM_CLOSE);
		return;
	}
	else if (m_buttonType == CBT_MINIMIZE)
	{
		//GetParent()->GetSystemMenu(NULL)->EnableMenuItem(SC_MINIMIZE, MF_ENABLED);
		//GetParent()->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		//GetParent()->GetSystemMenu(NULL)->EnableMenuItem(SC_MINIMIZE, MF_DISABLED);
		return;
	}
	
	CMFCButton::OnLButtonUp(nFlags, point);
}
