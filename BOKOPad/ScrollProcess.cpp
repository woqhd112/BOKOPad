#include "pch.h"
#include "ScrollProcess.h"

ScrollProcess::ScrollProcess()
{
	m_nPageCount = 0;
	m_nWheelSize = 60;
	m_nAllPageSize = 0;
	m_nScrollPos = 0;
	m_nScrollProcessCount = 0;
	//m_nOnePageSize = 0;
	m_bInit = false;
	m_prePos = 0;
}

ScrollProcess::~ScrollProcess()
{

}

void ScrollProcess::Init(ScrollInfo& info)
{
	m_pProcessDlg = info.scrollExecuteCtrl;
	m_nWheelSize = info.wheelSize;
	m_bInit = true;
}

void ScrollProcess::ExecuteScroll(int scrollLineFlag)
{
	if (!m_bInit)
		return;

	if (scrollLineFlag == SCROLL_LINE_ADD)
	{
		m_nAllPageSize += m_nWheelSize;
		m_nPageCount++;
	}
	else if (scrollLineFlag == SCROLL_LINE_DELETE)
	{
		m_nAllPageSize -= m_nWheelSize;
		m_nPageCount--;
		if (m_nPageCount < 0)
		{
			m_nPageCount = 0;
			m_nAllPageSize = 0;
		}
		if (m_nScrollProcessCount > m_nPageCount)
			m_nScrollProcessCount = m_nPageCount;
	}

	int nScrollMax = 0;
	if (m_nWheelSize < m_nAllPageSize)
	{
		nScrollMax = m_nAllPageSize - 1;
		m_nScrollPos = min(m_nScrollPos, m_nAllPageSize - m_nWheelSize - 1);
	}


	/*SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = nScrollMax;
	si.nPos = m_nScrollPos;
	si.nPage = m_nWheelSize;
	m_pProcessDlg->SetScrollInfo(SB_VERT, &si, TRUE);*/
}

void ScrollProcess::ProcessScrollCount(int nSBCode)
{
	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nScrollProcessCount--;
		break;
	case SB_PAGEUP:
		m_nScrollProcessCount--;
		break;
	case SB_PAGEDOWN:
		m_nScrollProcessCount++;
		break;
	case SB_LINEDOWN:
		m_nScrollProcessCount++;
		break;
	default:
		break;
	}
}
//
//bool ScrollProcess::OperateScroll(int nSBCode, int nPos)
//{
//	if (!m_bInit)
//		return false;
//
//	int delta = 0;
//	if (nSBCode == SB_LINEUP || nSBCode == SB_PAGEUP)
//	{
//		delta = -m_nWheelSize;
//	}
//	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_PAGEDOWN)
//	{
//		delta = m_nWheelSize;
//	}
//	/*else if (nSBCode == SB_THUMBTRACK)
//	{
//		if (m_prePos < nPos && nPos % m_nWheelSize == 0)
//		{
//			delta = m_nWheelSize;
//			nSBCode = SB_PAGEDOWN;
//		}
//		else if (m_prePos > nPos && nPos % m_nWheelSize == 0)
//		{
//			delta = -m_nWheelSize;
//			nSBCode = SB_PAGEUP;
//		}
//	}
//
//	m_prePos = nPos;*/
//
//	if (m_nScrollProcessCount < 0)
//		m_nScrollProcessCount = 0;
//
//	int scrollpos = m_nScrollPos + delta;
//	int nMaxPos = m_nAllPageSize - m_nWheelSize;
//
//	if (scrollpos < 0)
//	{
//		delta = -m_nScrollPos;
//	}
//	else
//	{
//		if (scrollpos > nMaxPos)
//		{
//			delta = nMaxPos - m_nScrollPos;
//		}
//	}
//
//	if (delta != 0)
//	{
//		ProcessScrollCount(nSBCode);
//		m_nScrollPos += delta;
//		m_pProcessDlg->SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
//		m_pProcessDlg->ScrollWindow(0, -delta);
//
//		return true;
//	}
//
//	return false;
//}

//UINT ScrollProcess::OperateWheel(short zDelta)
//{
//
//	UINT nFlag;
//	if (zDelta > 0)	// 위스크롤
//	{
//		nFlag = SB_LINEUP;
//	}
//	else // 아래스크롤
//	{
//		nFlag = SB_LINEDOWN;
//	}
//
//	return nFlag;
//}

int ScrollProcess::GetLineCount() const
{
	return m_nPageCount;
}

int ScrollProcess::GetScrollCount() const
{
	return m_nScrollProcessCount;
}

int ScrollProcess::GetScrollSize() const
{
	return m_nScrollPos;
}

int ScrollProcess::GetWheelSize() const
{
	return m_nWheelSize;
}

void ScrollProcess::ResetScroll()
{
	m_nAllPageSize = 0;
	m_nScrollPos = 0;
	m_nPageCount = 0;
	m_nScrollProcessCount = 0;
}