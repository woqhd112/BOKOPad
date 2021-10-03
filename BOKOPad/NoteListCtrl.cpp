// NoteListCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "NoteListCtrl.h"
#include "NoteManager.h"
#include "afxdialogex.h"


// NoteListCtrl 대화 상자\

IMPLEMENT_DYNAMIC(NoteListCtrl, CDialogEx)

NoteListCtrl::NoteListCtrl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NOTE_LIST_CTRL, pParent)
	, m_noteInformationContainer(new ComplexVector<NoteInformationVO>)
	, m_noteManager(new NoteManager)
{
	m_bMainScrollFocus = true;
	m_noteSize = 0;
	m_bDragProcessing = false;
}

NoteListCtrl::~NoteListCtrl()
{
	if (m_noteManager)
	{
		delete m_noteManager;
		m_noteManager = nullptr;
	}

	if (m_noteInformationContainer)
	{
		delete m_noteInformationContainer;
		m_noteInformationContainer = nullptr;
	}
}

void NoteListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NoteListCtrl, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// NoteListCtrl 메시지 처리기


BOOL NoteListCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_noteManager->AttachManager(this);

	ScrollProcess::ScrollInfo info;
	info.scrollExecuteCtrl = this;
	info.wheelSize = 70;
	scroll.Init(info);

	m_calculateItemPos.left = 0;
	m_calculateItemPos.top = 0;
	m_calculateItemPos.bottom = EDIT_HEIGHT;
	m_calculateItemPos.right = EDIT_WIDTH;

	GotoDlgCtrl(this);

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NoteListCtrl::LoadNoteInformation()
{
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), false, "");
	RequestScope->SetRequestAttributes(inNote);
	MVC_Controller->SelectInSceSEQNoteInformation();
	{
		RequestScope->GetRequestAttributes(m_noteInformationContainer);
		ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

		int i = 0;
		while (iter != m_noteInformationContainer->end())
		{
			CRect* itemRect = CalcNotePosition();
			NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			m_noteManager->SendMessages(PM_NOTE_INSERT);

			SignalNoteInput();
			iter++;
			i++;
		}
	}
}

bool NoteListCtrl::InsertNote(ComplexString inpusString)
{
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), false, inpusString);
	RequestScope->SetRequestAttributes(inNote);
	if (MVC_Controller->InsertNoteInformation())
	{
		RequestScope->SetRequestAttributes(inNote);
		if (MVC_Controller->SelectInSceSEQNoteInformation())
		{
			m_noteInformationContainer->clear();
			RequestScope->GetRequestAttributes(m_noteInformationContainer);

			if (m_noteInformationContainer->empty() == false)
			{
				CRect* itemRect = CalcNotePosition();
				NoteManagerStruct noteManagerStruct(&m_noteInformationContainer->back(), itemRect, m_noteInformationContainer->size() - 1);
				m_noteManager->InputNoteStruct(&noteManagerStruct);
				m_noteManager->SendMessages(PM_NOTE_INSERT);

				SignalNoteInput(true);
			}
		}
		else
			return false;

		return true;
	}
	return false;
}

void NoteListCtrl::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;
	m_defaultDragData.sceSEQ = thisDataStruct.scenarioData.GetSceSEQ();
	m_defaultDragData.sceIndex = thisDataStruct.scenarioIndex;
}

void NoteListCtrl::SignalNoteInput(bool bPosSwitch)
{
	if (m_noteSize % 4 == 0)
	{
		scroll.ExecuteScroll();
	}

	if (bPosSwitch)
	{
		// 현재 스크롤 위치가 제일 마지막이 아니라면..
		if (scroll.GetScrollCount() != scroll.GetLineCount())
		{
			int executeScrollCount = scroll.GetLineCount() - scroll.GetScrollCount();
			for (int i = 0; i < executeScrollCount; i++)
				OnVScroll(SB_LINEDOWN, 0, GetScrollBarCtrl(SB_VERT));
		}
	}

	++m_noteSize;
}

void NoteListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	scroll.OperateScroll(nSBCode, nPos);

	//CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL NoteListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	OnVScroll(scroll.OperateWheel(zDelta), 0, GetScrollBarCtrl(SB_VERT));

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void NoteListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


CRect* NoteListCtrl::CalcNotePosition()
{
	int nCurrentScrollIndex = scroll.GetScrollCount();

	m_calculateItemPos.left = (EDIT_WIDTH + 5) * (m_noteSize % 4);
	m_calculateItemPos.top = ((EDIT_HEIGHT + 10) * ((m_noteSize / 4) - nCurrentScrollIndex));// -scroll.GetScrollSize();
	m_calculateItemPos.right = m_calculateItemPos.left + EDIT_WIDTH;
	m_calculateItemPos.bottom = m_calculateItemPos.top + EDIT_HEIGHT;

	return &m_calculateItemPos;
}


BOOL NoteListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		if (m_bMainScrollFocus)
		{
			pMsg->hwnd = this->GetSafeHwnd();
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == this->GetSafeHwnd())
			m_bMainScrollFocus = true;
		else
		{
			m_bMainScrollFocus = false;
			UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
			if (nButtonStyle == BS_PUSHBUTTON || nButtonStyle == BS_DEFPUSHBUTTON)
			{
				m_defaultDragData.mousePos_X = pMsg->pt.x;
				m_defaultDragData.mousePos_Y = pMsg->pt.y;
				m_defaultDragData.buttonID = ::GetDlgCtrlID(pMsg->hwnd);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				m_bDragProcessing = m_noteManager->SendMessages(PM_DRAG_DOWN);
				//return TRUE;
			}
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		TRACE("드래그업 %d 번째 시나리오 접근!\n", m_thisDataStruct.scenarioIndex);
		UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
		if (nButtonStyle == BS_PUSHBUTTON || nButtonStyle == BS_DEFPUSHBUTTON)
		{
			if (m_bDragProcessing)
			{
				m_defaultDragData.buttonID = ::GetDlgCtrlID(pMsg->hwnd);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				m_noteManager->SendMessages(PM_DRAG_UP);
			}
			m_bDragProcessing = false;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		TRACE("드래그 무브 %d 번째 시나리오 접근!\n", m_thisDataStruct.scenarioIndex);
		//TRACE("드래그 상태 %s\n", m_bDragProcessing ? "true" : "false");
		if (m_bDragProcessing)
		{
			m_defaultDragData.mousePos_X = pMsg->pt.x;
			m_defaultDragData.mousePos_Y = pMsg->pt.y;
			m_noteManager->InputDragStruct(&m_defaultDragData);
			m_noteManager->SendMessages(PM_DRAG_MOVE);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL NoteListCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::OnCommand(wParam, lParam);
}

