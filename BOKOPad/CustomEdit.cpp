// CustomEdit.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomEdit.h"
#include "CustomEditCtrl.h"
#include "NoteListCtrl.h"

// CustomEdit

IMPLEMENT_DYNAMIC(CustomEdit, CEdit)

CustomEdit::CustomEdit()
{
	m_notSEQ = 0;
	m_bSwapping = false;
	m_nPrevLineCount = 0;
}

CustomEdit::CustomEdit(int notSEQ)
{
	m_notSEQ = notSEQ;
	m_bSwapping = false;
}

CustomEdit::~CustomEdit()
{
	while (m_editQueue.emptyOutputQueue() == false && m_bSwapping == false)
	{
	}
}


BEGIN_MESSAGE_MAP(CustomEdit, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CustomEdit::OnEnChange)
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


void CustomEdit::SetNotSEQ(int notSEQ)
{
	m_notSEQ = notSEQ;
}

// CustomEdit 메시지 처리기

void CustomEdit::ExecuteTimer()
{
	SetTimer(NULL, 500, NULL);
}

void CustomEdit::ScrollSync()
{
	CustomEditCtrl* parent = (CustomEditCtrl*)GetParent();
	parent->m_scroll.ResetScroll();

	int nLineCount = GetLineCount();
	if (nLineCount > 6)
	{
		int error = nLineCount - 6;
		for (int i = 0; i < error; i++)
			parent->m_scroll.ExecuteScroll(SCROLL_LINE_ADD);
	}
}

void CustomEdit::SetScrollEditPos(int scrollUpAndDown)
{
	if (scrollUpAndDown == SB_PAGEDOWN || scrollUpAndDown == SB_LINEDOWN)
	{
		LineScroll(1);
	}
	else if (scrollUpAndDown == SB_PAGEUP || scrollUpAndDown == SB_LINEUP)
	{
		LineScroll(-1);
	}
}

void CustomEdit::OnEnChange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CEdit::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nLineCount = GetLineCount();
	if (nLineCount > 6)
	{
		// 현재 크기의 6줄 이상이면 스크롤 처리함
		if (m_nPrevLineCount > nLineCount)
		{
			// 텍스트 지웠을때 라인이 감소하면..

			int error = m_nPrevLineCount - nLineCount;
			CustomEditCtrl* parent = (CustomEditCtrl*)GetParent();
			for (int i = 0; i < error; i++)
				parent->m_scroll.ExecuteScroll(SCROLL_LINE_ADD);
		}
		else if (m_nPrevLineCount < nLineCount)
		{
			// 텍스트 입력했을때 라인이 증가하면..

			int error = nLineCount - m_nPrevLineCount;
			CustomEditCtrl* parent = (CustomEditCtrl*)GetParent();
			for (int i = 0; i < error; i++)
				parent->m_scroll.ExecuteScroll(SCROLL_LINE_DELETE);
		}
	}

	CString strText;
	GetWindowTextA(strText);
	// 텍스트가 비어있지 않을 때만 푸시
	if (strText.IsEmpty() == false)
	{
		m_editQueue.lock();
		m_editQueue.push(strText);
		m_editQueue.unlock();
		m_bSwapping = false;
	}

	m_nPrevLineCount = GetLineCount();
}


void CustomEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_editQueue.swapQueue();
	m_bSwapping = true;
	if (m_editQueue.emptyOutputQueue() == false)
	{
		CString lastText = m_editQueue.backOutputQueue();

		Scenario_DB_Manager->UpdateNoteInformationInNotContent(m_notSEQ, lastText.GetBuffer());
		ComplexString log;
		log.Format("%d SEQ 노트 변경 DB 갱신 완료", m_notSEQ);
		Log_Manager->OnPutLog(log, LogType::LT_PROCESS);
		m_editQueue.clearOutputQueue();
	}

	CEdit::OnTimer(nIDEvent);
}



BOOL CustomEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		if (pMsg->hwnd == this->GetSafeHwnd())
		{
			CustomEditCtrl* parent = (CustomEditCtrl*)GetParent();
			NoteListCtrl* gParent = (NoteListCtrl*)parent->GetParent();
			if (gParent->bMainScrollFocus == false)
			{
				parent->SendMessageA(pMsg->message, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == this->GetSafeHwnd())
		{
			NoteListCtrl* gParent = (NoteListCtrl*)GetParent()->GetParent();
			gParent->bMainScrollFocus = false;
			this->SetFocus();
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}


BOOL CustomEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}


void CustomEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}
