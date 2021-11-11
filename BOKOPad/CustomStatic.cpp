// CustomStatic.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomStatic.h"


// CustomStatic

IMPLEMENT_DYNAMIC(CustomStatic, CStatic)

CustomStatic::CustomStatic()
{
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CustomStatic::~CustomStatic()
{
	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CustomStatic, CStatic)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CustomStatic 메시지 처리기




HBRUSH CustomStatic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == this->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH)m_brush;
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
