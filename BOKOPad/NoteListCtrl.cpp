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
	, m_downButton(nullptr)
{
	m_bMainScrollFocus = true;
	m_viewNoteSize = 0;
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
	scroll.ExecuteScroll(SCROLL_LINE_NOTHING);

	m_calculateItemPos.left = 0;
	m_calculateItemPos.top = 0;
	m_calculateItemPos.bottom = EDIT_HEIGHT;
	m_calculateItemPos.right = EDIT_WIDTH;

	GotoDlgCtrl(this);

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool NoteListCtrl::LoadDraggingNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteManager->SendMessages(PM_LOAD_DRAGGING_MODE) == false)
				return false;
			bDetectedTimelineCnt++;
		}

		i++;
		iter++;
	}

	return true;
}

bool NoteListCtrl::UnloadDraggingNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteManager->SendMessages(PM_UNLOAD_DRAGGING_MODE) == false)
				return false;
			bDetectedTimelineCnt++;
		}

		i++;
		iter++;
	}

	return true;
}

bool NoteListCtrl::DeleteAllItems()
{
	if (m_noteManager->SendMessages(PM_NOTE_CLEAR) == false)
		return false;

	m_viewNoteSize = 0;
	scroll.ResetScroll();

	return true;
}

bool NoteListCtrl::LoadNoteInformation()
{
	CURSOR_WAIT;
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), false, false, "");
	if (UpdateScenarioList(&inNote) == false)
		return false;

	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
		NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);

		m_noteManager->InputNoteStruct(&noteManagerStruct);
		if (m_noteManager->SendMessages(PM_NOTE_INSERT) == false)
			return false;

		if (iter->value.IsSetTIMELINE() == false)
		{
			bDetectedTimelineCnt++;
			ScrollExecute(true);
		}
		i++;
		iter++;
	}
	CURSOR_ARROW;

	return true;
}

bool NoteListCtrl::UpdateScenarioList(NoteInformationVO* noteInform)
{
	RequestScope->SetRequestAttributes(*noteInform);
	if (MVC_Controller->SelectInSceSEQNoteInformation() == false)
		return false;

	m_noteInformationContainer->clear();
	RequestScope->GetRequestAttributes(m_noteInformationContainer);
	return true;
}

bool NoteListCtrl::UpdateSetTIME(int notSEQ)
{
	NoteInformationVO note;
	note.SetNotSEQ(notSEQ);
	note.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	note.SetSetTIMELINE(false);
	RequestScope->SetRequestAttributes(note);
	if (MVC_Controller->UpdateNoteInformationInSetTIMELINE() == false)
		return false;

	if (UpdateScenarioList(&note) == false)
		return false;

	NoteManagerStruct noteManagerStruct(&note, NULL, notSEQ);

	m_noteManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteManager->SendMessages(PM_FIND_NOTE_INDEX) == false)
		return false;

	m_noteManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteManager->SendMessages(PM_NOTE_SHOW) == false)
		return false;
		
	// 타임라인 보이기 false로 업데이트 완료했으니 노트들 무빙시켜야함
	if (MoveNote() == false)
		return false;

	ScrollExecute(true, true);
	return true;
}

bool NoteListCtrl::InsertNote(ComplexString inpusString, bool bNoteShow)
{
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), !bNoteShow, false, inpusString);
	RequestScope->SetRequestAttributes(inNote);
	if (MVC_Controller->InsertNoteInformation() == false)
		return false;

	if (UpdateScenarioList(&inNote) == false)
		return false;

	CRect* itemRect = CalcNotePosition(m_viewNoteSize);
	NoteManagerStruct noteManagerStruct(&m_noteInformationContainer->back(), itemRect, m_noteInformationContainer->size() - 1);

	m_noteManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteManager->SendMessages(PM_NOTE_INSERT) == false)
		return false;

	if (bNoteShow)
		ScrollExecute(true, true);

	return true;
}

bool NoteListCtrl::CheckDeleteNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	NoteManagerStruct noteManagerStruct(NULL, NULL, -1);
	int deleteCount = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			noteManagerStruct.noteData = &iter->value;
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			// ui삭제안된건 냅두고 된거만 db삭제한다.
			if (m_noteManager->SendMessages(PM_NOTE_CHECK_DELETE))
			{
				RequestScope->SetRequestAttributes(*noteManagerStruct.noteData);
				if (MVC_Controller->DeleteNoteInformation() == false)
					return false;

				deleteCount++;
			}
		}
		iter++;
	}

	if (deleteCount > 0)
	{
		if (DeleteAllItems() == false)
			return false;

		if (LoadNoteInformation() == false)
			return false;

	}


	return true;
}

bool NoteListCtrl::DeleteNote(int notSEQ)
{
	NoteInformationVO inNote(notSEQ, m_thisDataStruct.scenarioData.GetSceSEQ(), false, false, "");
	RequestScope->SetRequestAttributes(inNote);
	if (MVC_Controller->DeleteNoteInformation() == false)
		return false;

	if (UpdateScenarioList(&inNote) == false)
		return false;

	NoteManagerStruct noteManagerStruct(&inNote, NULL, -1);
	m_noteManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteManager->SendMessages(PM_NOTE_DELETE) == false)
		return false;

	// 삭제한 노트인덱스 부터 노트리스트의 마지막 인덱스까지 정렬
	if (MoveNote() == false)
		return false;

	ScrollExecute(false, true);


	return true;
}

bool NoteListCtrl::MoveNote()
{
	int noteSize = m_noteInformationContainer->size();

	int bDetectedTimelineCnt = 0;
	for (int i = 0; i < noteSize; i++)
	{
		NoteInformationVO moveVO = m_noteInformationContainer->at(i);
		if (moveVO.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(i - bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&moveVO, itemRect, i);
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteManager->SendMessages(PM_NOTE_MOVE) == false)
				return false;
		}
		else
			bDetectedTimelineCnt++;
	}

	return true;
}

void NoteListCtrl::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;
	m_defaultDragData.sceSEQ = thisDataStruct.scenarioData.GetSceSEQ();
	m_defaultDragData.sceIndex = thisDataStruct.scenarioIndex;
}

void NoteListCtrl::ScrollExecute(bool bAdd, bool bPosSwitch)
{
	if (bAdd)
	{
		if (m_viewNoteSize % 4 == 0)
		{
			scroll.ExecuteScroll(SCROLL_LINE_ADD);
		}
		++m_viewNoteSize;
	}
	else
	{
		if ((m_viewNoteSize - 1) % 4 == 0)
		{
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
		}
		--m_viewNoteSize;
	}

	if (bPosSwitch)
	{
		// 현재 스크롤 위치가 제일 마지막이 아니라면..
		if (scroll.GetScrollCount() != scroll.GetLineCount())
		{
			int executeScrollCount = scroll.GetLineCount() - scroll.GetScrollCount();
			// 노트를 추가했을 때만 스크롤 다운 이벤트
			if (bAdd)
			{
				for (int i = 0; i < executeScrollCount; i++)
					OnVScroll(SB_LINEDOWN, 0, GetScrollBarCtrl(SB_VERT));
			}
		}
		// 현재 스크롤이 마지막일 때
		else
		{
			// 노트가 삭제되면 페이지 업..
			if (m_viewNoteSize % 4 == 0)
			{
				if (!bAdd)
					OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
			}
		}
	}
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


CRect* NoteListCtrl::CalcNotePosition(int itemIndex)
{
	// 이부분이 조금 이상한듯? 그리고 타임라인에서 노트쪽으로 드래그할때 마우스포인트도 처리할것
	int nCurrentScrollIndex = scroll.GetScrollCount();

	m_calculateItemPos.left = (EDIT_WIDTH + 5) * (itemIndex % 4);
	m_calculateItemPos.top = ((EDIT_HEIGHT + 10) * ((itemIndex / 4) - nCurrentScrollIndex));// -scroll.GetScrollSize();
	m_calculateItemPos.right = m_calculateItemPos.left + EDIT_WIDTH;
	m_calculateItemPos.bottom = m_calculateItemPos.top + EDIT_HEIGHT;

	return &m_calculateItemPos;
}

bool NoteListCtrl::DragDown(MSG* pMsg)
{
	Scenario_Manager->InputScenarioStruct(&m_thisDataStruct);
	if (Scenario_Manager->SendMessages(PM_IS_DRAGGING_MODE) == false)
		return false;

	UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
	if (nButtonStyle == BS_PUSHBUTTON || nButtonStyle == BS_DEFPUSHBUTTON)
	{
		m_downButton = (CButton*)FromHandle(pMsg->hwnd);
		m_defaultDragData.mousePos_X = pMsg->pt.x;
		m_defaultDragData.mousePos_Y = pMsg->pt.y;
		m_defaultDragData.buttonID = ::GetDlgCtrlID(pMsg->hwnd);
		m_noteManager->InputDragStruct(&m_defaultDragData);
		m_bDragProcessing = m_noteManager->SendMessages(PM_DRAG_DOWN);
		if (!m_bDragProcessing)
		{
			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_OFF) == false)
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

			return false;
		}

		return true;
	}
	return false;
}

bool NoteListCtrl::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	m_defaultDragData.mousePos_X = pMsg->pt.x;
	m_defaultDragData.mousePos_Y = pMsg->pt.y;
	m_noteManager->InputDragStruct(&m_defaultDragData);
	m_noteManager->SendMessages(PM_DRAG_MOVE);
	return true;
}

bool NoteListCtrl::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	if (m_downButton == nullptr)
	{
		m_bDragProcessing = false;
		return false;
	}

	m_defaultDragData.buttonID = m_downButton->GetDlgCtrlID();
	m_noteManager->InputDragStruct(&m_defaultDragData);
	if (m_noteManager->SendMessages(PM_DRAG_UP))
	{
		CURSOR_WAIT;
		DragUpState dus = m_noteManager->GetDragState();
		if (dus == DUS_NOTHING)
		{
			// 버그날일이 없을듯하다..
			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_NOTHING) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				return false;
			}
		}
		else if (dus == DUS_THIS)
		{
			// 버그날일이 없을듯하다..
			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_THIS_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				return false;
			}
		}
		else if (dus == DUS_ANOTHER)
		{
			TransactionInstance->RequestSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
			// db처리는 다이얼로그에서.. 매니저는 ui처리만 할것
			
			NoteInformationVO note;
			note.SetNotSEQ(m_defaultDragData.noteSEQ);
			RequestScope->SetRequestAttributes(note);
			if (MVC_Controller->SelectOneNoteInformation() == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
				return false;
			}

			RequestScope->GetRequestAttributes(&note);
			m_defaultDragData.noteCONTENT = note.GetNotCONTENT();
			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_ANOTHER_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_ANOTHER_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
			TransactionInstance->Commit();
		}
		else if (dus == DUS_THIS_TIMELINE)
		{
			TransactionInstance->RequestSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_THIS_TIMELINE_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			// 노트의 settimeline값을 true로 주고 노트 ui를 hide 시키고 감춘 노트인덱스까지 모두 이동시킨다.
			NoteInformationVO inNote(m_defaultDragData.noteSEQ, m_thisDataStruct.scenarioData.GetSceSEQ(), true, false, "");
			RequestScope->SetRequestAttributes(inNote);
			if (MVC_Controller->UpdateNoteInformationInSetTIMELINE() == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			NoteManagerStruct noteManagerStruct(&inNote, NULL, m_defaultDragData.noteIndex);
			m_noteManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteManager->SendMessages(PM_NOTE_HIDE) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			if (UpdateScenarioList(&inNote) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			// 스크롤 처리
			ScrollExecute(false, true);
			// 노트없앤 인덱스부터 에딧 이동
			if (MoveNote() == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
			TransactionInstance->Commit();
		}
		else if (dus == DUS_ANOTHER_TIMELINE)
		{
			TransactionInstance->RequestSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL]);

			m_noteManager->InputDragStruct(&m_defaultDragData);
			if (m_noteManager->SendMessages(PM_DRAG_ANOTHER_TIMELINE_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				TransactionInstance->Rollback(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL]);
				TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL]);
				m_noteManager->InputDragStruct(&m_defaultDragData);
				if (m_noteManager->SendMessages(PM_ROLLBACK_THIS_ANOTHER_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			TransactionInstance->ReleaseSavePoint(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL]);
			TransactionInstance->Commit();
		}
		CURSOR_ARROW;
	}
	else
	{
		m_bDragProcessing = false;
		m_downButton = nullptr;
		return false;
	}

	m_bDragProcessing = false;
	m_downButton = nullptr;

	return true;
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
			if (DragDown(pMsg))
				return TRUE;
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (DragUp(pMsg))
			return TRUE;
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (DragMove(pMsg))
			return FALSE;
	}
	else if (WM_KEYUP == pMsg->message)
	{
		if (pMsg->wParam == VK_CONTROL)
		{
			MSG msg;
			msg.message = WM_KEYUP;
			msg.wParam = VK_CONTROL;
			GetParent()->PreTranslateMessage(&msg);
			GetParent()->SetFocus();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL NoteListCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (HIWORD(wParam) == BN_CLICKED)
	{
		// 음.. 시나리오디테일에서 컨트롤 누른상태에서 클릭이벤트 처리되면 다시 키업으로 안돌아감..
		// 노트매니저에 체크박스 클릭 이벤트 날리기
		m_defaultDragData.buttonID = LOWORD(wParam);
		m_noteManager->InputDragStruct(&m_defaultDragData);
		m_noteManager->SendMessages(PM_NOTE_CLICK);
		return 1;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
