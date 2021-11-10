#include "pch.h"
#include "NoteUIManager.h"
#include "..\..\BOKOScenarioDetailDlg.h"
#include "..\..\BOKODragDlg.h"
//#include "NoteListCtrl.h"


static unsigned int g_notePadID = 10000;

NoteUIManager::NoteUIManager()
	: m_variableCtrlPos_x(0)
	, m_variableCtrlPos_y(0)
	, m_bCursorAttach(false)
	, m_bCursorDetach(true)
{
	
}

NoteUIManager::~NoteUIManager()
{
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	while (iter != m_notePadManager.end())
	{
		NotePadStruct noteStruct = iter->value.value;
		CEdit* noteEdit = noteStruct.noteEdit;
		CButton* noteButton = noteStruct.noteButton;
		CButton* noteCheckBox = noteStruct.noteCheckBox;
		noteEdit->DestroyWindow();
		delete noteEdit;
		noteEdit = nullptr;
		noteButton->DestroyWindow();
		delete noteButton;
		noteButton = nullptr;
		noteCheckBox->DestroyWindow();
		delete noteCheckBox;
		noteCheckBox = nullptr;

		iter++;
	}

	m_notePadManager.clear();
}

bool NoteUIManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(message);
}


bool NoteUIManager::HelpInvoker(PerformanceMessage message)
{
	bool bHelpSuccess = false;

	if (message == PM_NOTE_INSERT)
	{
		bHelpSuccess = Insert();
	}
	else if (message == PM_NOTE_DELETE)
	{
		bHelpSuccess = Delete();
	}
	else if (message == PM_NOTE_SHOW)
	{
		bHelpSuccess = Show();
	}
	else if (message == PM_NOTE_HIDE)
	{
		bHelpSuccess = Hide();
	}
	else if (message == PM_NOTE_MOVE)
	{
		bHelpSuccess = Move();
	}
	else if (message == PM_NOTE_CHECK_DELETE)
	{
		bHelpSuccess = NoteCheckDelete();
	}
	else if (message == PM_NOTE_CLICK)
	{
		bHelpSuccess = NoteClick();
	}
	else if (message == PM_DRAG_MOVE)
	{
		bHelpSuccess = DragMove();
	}
	else if (message == PM_LOAD_DRAGGING_MODE)
	{
		bHelpSuccess = LoadDraggingMode();
	}
	else if (message == PM_UNLOAD_DRAGGING_MODE)
	{
		bHelpSuccess = UnloadDraggingMode();
	}
	else if (message == PM_DRAG_DOWN)
	{
		bHelpSuccess = DragDown();
	}
	else if (message == PM_DRAG_UP)
	{
		bHelpSuccess = DragUp();
	}
	else if (message == PM_DRAG_OFF)
	{
		bHelpSuccess = DragOff();
	}
	else if (message == PM_DRAG_NOTHING)
	{
		bHelpSuccess = DragNothing();
	}
	else if (message == PM_DRAG_THIS_ATTACH)
	{
		bHelpSuccess = DragThisAttach();
	}
	else if (message == PM_DRAG_ANOTHER_ATTACH)
	{
		bHelpSuccess = DragAnotherAttach();
	}
	else if (message == PM_DRAG_THIS_TIMELINE_ATTACH)
	{
		bHelpSuccess = DragThisTimelineAttach();
	}
	else if (message == PM_DRAG_ANOTHER_TIMELINE_ATTACH)
	{
		bHelpSuccess = DragAnotherTimelineAttach();
	}
	else if (message == PM_FIND_NOTE_INDEX)
	{
		bHelpSuccess = FindNoteIndex();
	}
	else if (message == PM_ROLLBACK_ANOTHER_ATTACH)
	{
		bHelpSuccess = RollbackAnotherAttach();
	}
	else if (message == PM_NOTE_CLEAR)
	{
		bHelpSuccess = Clear();
	}
	else if (message == PM_ROLLBACK_THIS_TIMELINE_ATTACH)
	{
		bHelpSuccess = RollbackThisTimelineAttach();
	}
	else if (message == PM_ROLLBACK_THIS_ANOTHER_TIMELINE_ATTACH)
	{
		bHelpSuccess = RollbackThisAnotherTimelineAttach();
	}

	return bHelpSuccess;
}

bool NoteUIManager::Insert()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (noteDataStruct->noteIndex < 0)
		return false;

	NoteManagerStruct noteManagerStruct;
	noteManagerStruct.noteData = noteDataStruct->noteData;
	noteManagerStruct.noteIndex = noteDataStruct->noteIndex;
	noteManagerStruct.noteRect = noteDataStruct->noteRect;

	CEdit* noteInputEdit = new CEdit;
	CButton* noteInputButton = new CButton;
	CButton* noteInputCheckBox = new CButton;
	
	bool bCreate1 = false, bCreate2 = false, bCreate3 = false;
	bCreate1 = (bool)noteInputEdit->Create(WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER | ES_MULTILINE, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);
	bCreate2 = (bool)noteInputButton->Create("...", WS_VISIBLE | BS_CENTER | BS_PUSHBUTTON, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);
	bCreate3 = (bool)noteInputCheckBox->Create("", WS_VISIBLE | BS_AUTOCHECKBOX, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);


	if (bCreate1 && bCreate2)
	{
		try
		{
			noteInputEdit->SetFont(&m_setFont);

			NotePadStruct padStruct;
			padStruct.noteButton = noteInputButton;
			padStruct.noteEdit = noteInputEdit;
			padStruct.noteCheckBox = noteInputCheckBox;
			m_notePadManager.insert(noteManagerStruct.noteIndex, padStruct);
			m_noteSeqMap.insert(noteManagerStruct.noteIndex, noteDataStruct->noteData->GetNotSEQ());
		}
		catch (ComplexDuplicateException e)
		{
			// 키 중복은 삭제하고 리턴
			noteInputEdit->DestroyWindow();
			delete noteInputEdit;
			noteInputEdit = nullptr;
			noteInputButton->DestroyWindow();
			delete noteInputButton;
			noteInputButton = nullptr;
			noteInputCheckBox->DestroyWindow();
			delete noteInputCheckBox;
			noteInputCheckBox = nullptr;
			ReleaseNoteStruct();
			return false;
		}

		noteInputCheckBox->MoveWindow(noteManagerStruct.noteRect->left, noteManagerStruct.noteRect->top, 10, 10);
		noteInputCheckBox->ShowWindow(SW_HIDE);

		noteInputButton->MoveWindow(noteManagerStruct.noteRect->left, noteManagerStruct.noteRect->top, noteManagerStruct.noteRect->Width(), 10);
		noteInputButton->SetWindowTextA("...");
		noteInputButton->ShowWindow(noteManagerStruct.noteData->IsSetTIMELINE() ? SW_HIDE : SW_SHOW);

		noteInputEdit->MoveWindow(noteManagerStruct.noteRect->left, noteManagerStruct.noteRect->top + 10, noteManagerStruct.noteRect->Width(), noteManagerStruct.noteRect->Height());
		noteInputEdit->SetWindowTextA(noteManagerStruct.noteData->GetNotCONTENT().GetBuffer());
		noteInputEdit->ShowWindow(noteManagerStruct.noteData->IsSetTIMELINE() ? SW_HIDE : SW_SHOW);
	}
	else if (!bCreate1 && bCreate2)
	{
		delete noteInputEdit;
		noteInputEdit = nullptr;
		noteInputButton->DestroyWindow();
		delete noteInputButton;
		noteInputButton = nullptr;
	}
	else if (bCreate1 && !bCreate2)
	{
		noteInputEdit->DestroyWindow();
		delete noteInputEdit;
		noteInputEdit = nullptr;
		delete noteInputButton;
		noteInputButton = nullptr;
	}
	else
	{
		noteInputEdit->DestroyWindow();
		delete noteInputEdit;
		noteInputEdit = nullptr;
		noteInputButton->DestroyWindow();
		delete noteInputButton;
		noteInputButton = nullptr;
	}

	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::Delete()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (m_noteSeqMap.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	int notIndex = noteDataStruct->noteIndex;
	if (notIndex < 0)
	{
		// 버튼, 에딧컨트롤 삭제하기
		bool bFind = false;
		ComplexMap<int, int>::iterator iter1 = m_noteSeqMap.begin();
		while (iter1 != m_noteSeqMap.end())
		{
			if (iter1->value.value == noteDataStruct->noteData->GetNotSEQ())
			{
				notIndex = iter1->value.key;
				noteDataStruct->noteIndex = notIndex;
				bFind = true;
				break;
			}
			iter1++;
		}

		if (!bFind)
		{
			ReleaseNoteStruct();
			return false;
		}
	}

	ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.find(notIndex);
	if (iter2 == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	CButton* deleteButton = iter2->value.value.noteButton;
	CButton* deleteCheckBox = iter2->value.value.noteCheckBox;
	CEdit* deleteEdit = iter2->value.value.noteEdit;
	
	delete deleteButton;
	delete deleteCheckBox;
	delete deleteEdit;
	deleteButton = nullptr;
	deleteCheckBox = nullptr;
	deleteEdit = nullptr;

	int findKey = iter2->value.key;
	m_notePadManager.erase(findKey);
	m_noteSeqMap.erase(findKey);

	SortNoteManagerKey();

	return true;
}

bool NoteUIManager::Clear()
{
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	while (iter != m_notePadManager.end())
	{
		NotePadStruct noteStruct = iter->value.value;
		CEdit* noteEdit = noteStruct.noteEdit;
		CButton* noteButton = noteStruct.noteButton;
		CButton* noteCheckBox = noteStruct.noteCheckBox;
		noteEdit->DestroyWindow();
		delete noteEdit;
		noteEdit = nullptr;
		noteButton->DestroyWindow();
		delete noteButton;
		noteButton = nullptr;
		noteCheckBox->DestroyWindow();
		delete noteCheckBox;
		noteCheckBox = nullptr;

		iter++;
	}

	m_notePadManager.clear();
	m_noteSeqMap.clear();

	return true;
}

bool NoteUIManager::Show()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (noteDataStruct->noteIndex < 0)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(noteDataStruct->noteIndex);
	if (iter == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	iter->value.value.noteButton->ShowWindow(SW_SHOW);
	// 드래그상태에서만 처리되는 함수이니 주석
	//iter->value.value.noteCheckBox->ShowWindow(SW_SHOW);
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);

	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::Hide()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (noteDataStruct->noteIndex < 0)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(noteDataStruct->noteIndex);
	if (iter == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	iter->value.value.noteButton->ShowWindow(SW_HIDE);
	// 드래그 상태에서만 처리되는 함수이니 주석
	//iter->value.value.noteCheckBox->ShowWindow(SW_HIDE);
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::Move()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (noteDataStruct->noteIndex < 0)
	{
		ReleaseNoteStruct();
		return false;
	}

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(noteDataStruct->noteIndex);
	if (iter == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	int sceSEQ = noteDataStruct->noteData->GetSceSEQ();
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(iter1->value.key);
	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (iter2->value.value->m_bDragModeCheck)
	{
		iter->value.value.noteButton->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top, noteDataStruct->noteRect->Width(), 10);
		iter->value.value.noteEdit->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top + 10, noteDataStruct->noteRect->Width(), noteDataStruct->noteRect->Height());
	}
	else
	{
		iter->value.value.noteButton->MoveWindow(noteDataStruct->noteRect->left + 11, noteDataStruct->noteRect->top, noteDataStruct->noteRect->Width() - 11, 10);
		iter->value.value.noteCheckBox->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top, 10, 10);
		iter->value.value.noteEdit->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top + 10, noteDataStruct->noteRect->Width(), noteDataStruct->noteRect->Height());
	}

	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::NoteCheckDelete()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	int notSEQ = noteDataStruct->noteData->GetNotSEQ();

	ComplexMap<int, int>::iterator iter1 = m_noteSeqMap.begin();

	bool bFind = false;
	while (iter1 != m_noteSeqMap.end())
	{
		if (iter1->value.value == notSEQ)
		{
			bFind = true;
			break;
		}

		iter1++;
	}

	if (!bFind)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.find(iter1->value.key);
	if (iter2 == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (iter2->value.value.noteCheckBox->GetCheck() == FALSE)
	{
		ReleaseNoteStruct();
		return false;
	}

	CButton* deleteButton = iter2->value.value.noteButton;
	CButton* deleteCheckBox = iter2->value.value.noteCheckBox;
	CEdit* deleteEdit = iter2->value.value.noteEdit;

	delete deleteButton;
	delete deleteCheckBox;
	delete deleteEdit;
	deleteButton = nullptr;
	deleteCheckBox = nullptr;
	deleteEdit = nullptr;

	int findKey = iter2->value.key;
	m_notePadManager.erase(findKey);
	m_noteSeqMap.erase(findKey);

	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::NoteClick()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_noteSeqMap.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	// 버튼 아이디로 노트정보 찾기
	ComplexMap<int, NotePadStruct>::iterator iter1 = m_notePadManager.begin();

	bool bFind = false;
	while (iter1 != m_notePadManager.end())
	{
		if (iter1->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (iter1 == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (iter1->value.value.noteCheckBox->GetCheck() == TRUE)
		iter1->value.value.noteCheckBox->SetCheck(FALSE);
	else
		iter1->value.value.noteCheckBox->SetCheck(TRUE);

	ReleaseDragStruct();

	return true;
}

bool NoteUIManager::DragDown()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_noteSeqMap.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	// 버튼 아이디로 노트정보 찾기
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	bool bFind = false;
	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
		{
			bFind = true;
			break;
		}
		iter++;
	}

	if (bFind)
	{
		ComplexMap<int, int>::iterator iter2 = m_noteSeqMap.find(iter->value.key);
		if (iter2 == m_noteSeqMap.end())
		{
			ReleaseDragStruct();
			return false;
		}

		/*CString strEditContent;
		iter->value.value.noteEdit->GetWindowTextA(strEditContent);*/

		dragDataStruct->noteIndex = iter2->value.key;
		dragDataStruct->noteSEQ = iter2->value.value;
		//dragDataStruct->noteCONTENT = strEditContent.GetBuffer();
	}
	else
	{
		ReleaseDragStruct();
		return false;
	}

	m_dragState = DUS_NOTHING;
	//m_dragDlg->SetDragData(dragDataStruct->noteSEQ, dragDataStruct->noteIndex, dragDataStruct->noteCONTENT);
	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);
	m_dragDlg->ShowWindow(SW_SHOW);

	iter->value.value.noteButton->ShowWindow(SW_HIDE);
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	//m_cursorEventCnt = ShowCursor(FALSE);
	CursorCountRestore(-1);
	m_mainDlg->SetCapture();
	ReleaseDragStruct();

	m_bIsDragging = true;

	return true;
}

bool NoteUIManager::DragMove()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}


	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.begin();

	bool bFind = false;
	while (iter1 != m_scenarioDlgManager.end())
	{
		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			//TRACE("%d번째 시나리오 접근\n", iter1->value.key);
			if (m_bCursorDetach)
			{
				m_bCursorAttach = true;
				CursorCountRestore(0);
				m_dragDlg->ShowWindow(SW_HIDE);
				CURSOR_CROSS;
				m_bCursorDetach = false;

			}
			iter1->value.value->m_timeline.ThickEventTimeline(dragDataStruct->noteSEQ, pt, Timeline::TTA_NOTE_BY_TIMELINE_DRAG_EVENT_APPROCH);
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		if (m_bCursorAttach)
		{
			//TRACE("!!\n");
			m_bCursorDetach = true;
			CursorCountRestore(-1);
			m_dragDlg->ShowWindow(SW_SHOW);
			m_bCursorAttach = false;
		}
	}

	// 마우스 커서 이벤트처리 타임라인일 때
	/*CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	if (iter1 != m_scenarioDlgManager.end())
	{
		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			if (m_bCursorDetach)
			{
				m_bCursorAttach = true;
				CursorCountRestore(0);
				m_dragDlg->ShowWindow(SW_HIDE);
				CURSOR_CROSS;
				m_bCursorDetach = false;

			}
			iter1->value.value->m_timeline.ThickEventTimeline(dragDataStruct->noteSEQ, pt, Timeline::TTA_NOTE_BY_TIMELINE_DRAG_EVENT_APPROCH);
		}
		else
		{
			if (m_bCursorAttach)
			{
				m_bCursorDetach = true;
				CursorCountRestore(-1);
				m_dragDlg->ShowWindow(SW_SHOW);
				m_bCursorAttach = false;
			}
		}
	}*/


	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);

	ReleaseDragStruct();

	return true;
}

bool NoteUIManager::DragUp()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	
	// 해당 위치가 어딘지에 따라 처리해야하는 방법이 다름
	// 마우스의 포인트가 현재 다이얼로그가 아닐때 처리
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.begin();
	while (iter1 != m_scenarioDlgManager.end())
	{
		CRect rect;
		iter1->value.value->GetWindowRect(rect);
		if(PtInRect(rect, pt))
			break;

		iter1++;
	}

	// 다이얼로그를 찾지 못하였을 때
	if (iter1 == m_scenarioDlgManager.end())
	{
		m_dragState = DUS_NOTHING;
	}
	// 찾은 다이얼로그가 현재 다이얼로그일 때
	else if (iter1->value.value == m_mainDlg->GetParent())
	{
		m_dragState = DUS_THIS;
		dragDataStruct->target_sceSEQ = -1;
		// 타임라인인지 구분

		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_THIS_TIMELINE;

			// 버튼 아이디로 노트정보 찾기
			ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.begin();

			bool bFind = false;
			while (iter2 != m_notePadManager.end())
			{
				if (iter2->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
				{
					bFind = true;
					break;
				}
				iter2++;
			}

			if (bFind)
			{
				iter2->value.value.noteButton->ShowWindow(SW_SHOW);
				iter2->value.value.noteEdit->ShowWindow(SW_SHOW);
			}
		}
	}
	// 찾은 다이얼로그가 다른 다이얼로그일 때
	else
	{
		m_dragState = DUS_ANOTHER;

		// 다른 다이얼로그일 때 해당 다이얼로그의 seq번호를 구조체에 저장
		ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.find(iter1->value.key);
		if (iter2 != m_scenarioSeqMap.end())
			dragDataStruct->target_sceSEQ = iter2->value.value;

		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_ANOTHER_TIMELINE;
		}
	}

	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	CursorCountRestore(0);
	CURSOR_ARROW;
	ReleaseCapture();
	m_bIsDragging = false;

	return true;
}

bool NoteUIManager::DragOff()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	if (dragDataStruct->noteIndex < 0)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(dragDataStruct->noteIndex);
	if (iter == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	iter->value.value.noteButton->ShowWindow(SW_SHOW);
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);
	m_dragDlg->ShowWindow(SW_HIDE);

	ReleaseNoteStruct();

	CursorCountRestore(0);
	CURSOR_ARROW;
	ReleaseCapture();
	m_bIsDragging = false;
	return true;
}

bool NoteUIManager::DragNothing()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 버튼 아이디로 노트정보 찾기
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	bool bFind = false;
	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
		{
			bFind = true;
			break;
		}
		iter++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	iter->value.value.noteButton->ShowWindow(SW_SHOW);
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);

	ReleaseDragStruct();

	return true;
}

bool NoteUIManager::DragThisAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 버튼 아이디로 노트정보 찾기
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	bool bFind = false;
	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
		{
			bFind = true;
			break;
		}
		iter++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	iter->value.value.noteButton->ShowWindow(SW_SHOW);
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::DragAnotherAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_scenarioSeqMap.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	// 시나리오 시퀀스맵에서 해당 시나리오 시퀀스번호 찾고 신호 보내서 시나리오 리스트 갱신.
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오에서 노트삭제
	if (iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오에서 노트추가
	if (iter3->value.value->SignalInsertNote(dragDataStruct->noteCONTENT, true) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::DragThisTimelineAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_scenarioSeqMap.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	if (iter1 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}
	
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	
	if (iter1->value.value->SignalInsertTimeline(dragDataStruct->noteSEQ, pt) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();

	return true;
}

bool NoteUIManager::DragAnotherTimelineAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_scenarioSeqMap.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	// 시나리오 시퀀스맵에서 해당 시나리오 시퀀스번호 찾고 신호 보내서 시나리오 리스트 갱신.
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오에서 노트삭제
	if (iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오에서 노트추가
	if (iter3->value.value->SignalInsertNote(dragDataStruct->noteCONTENT, false) == false)
	{
		ReleaseDragStruct();
		return false;
	}
	int targetNoteContainerSize = iter3->value.value->m_list_notePad.m_noteInformationContainer->size();

	if (targetNoteContainerSize <= 0)
	{
		ReleaseDragStruct();
		return false;
	}

	int lastInsertNoteSEQ = iter3->value.value->m_list_notePad.m_noteInformationContainer->at(targetNoteContainerSize - 1).GetNotSEQ();
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	// 타겟 타임라인 추가
	if (iter3->value.value->SignalInsertTimeline(lastInsertNoteSEQ, pt) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::FindNoteIndex()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	ComplexMap<int, int>::iterator iter = m_noteSeqMap.begin();

	bool bFind = false;
	while (iter != m_noteSeqMap.end())
	{
		if (iter->value.value == noteDataStruct->noteData->GetNotSEQ())
		{
			bFind = true;
			break;
		}
		iter++;
	}

	if (!bFind)
	{
		ReleaseNoteStruct();
		return false;
	}

	noteDataStruct->noteIndex = iter->value.key;
	ReleaseNoteStruct();

	return true;
}

bool NoteUIManager::RollbackAnotherAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 시나리오 시퀀스맵에서 해당 시나리오 시퀀스번호 찾고 신호 보내서 시나리오 리스트 갱신.
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오 리로드
	if (iter2->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오 리로드
	if (iter3->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::RollbackThisTimelineAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(dragDataStruct->sceIndex);

	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오 리로드
	if (iter->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 타임라인 리로드
	if (iter->value.value->SignalReloadTimeline() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::RollbackTimelineAnotherAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 시나리오 시퀀스맵에서 해당 시나리오 시퀀스번호 찾고 신호 보내서 시나리오 리스트 갱신.
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 타임라인 리로드
	if (iter2->value.value->SignalReloadTimeline() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오 리로드
	if (iter3->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::RollbackThisAnotherTimelineAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 시나리오 시퀀스맵에서 해당 시나리오 시퀀스번호 찾고 신호 보내서 시나리오 리스트 갱신.
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

	if (iter2 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오 리로드
	if (iter2->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 타임라인 리로드
	if (iter2->value.value->SignalReloadTimeline() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오 리로드
	if (iter3->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 타임라인 리로드
	if (iter3->value.value->SignalReloadTimeline() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}

bool NoteUIManager::LoadDraggingMode()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (noteDataStruct->noteIndex < 0)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(noteDataStruct->noteIndex);

	iter->value.value.noteCheckBox->ShowWindow(SW_HIDE);
	iter->value.value.noteCheckBox->SetCheck(FALSE);	// 드래그모드 설정하면 체크했던 체크박스들 전부 해제
	iter->value.value.noteButton->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top, noteDataStruct->noteRect->Width(), 10);

	ReleaseNoteStruct();
	return true;
}

bool NoteUIManager::UnloadDraggingMode()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (noteDataStruct->noteIndex < 0)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(noteDataStruct->noteIndex);

	iter->value.value.noteCheckBox->ShowWindow(SW_SHOW);
	iter->value.value.noteButton->MoveWindow(noteDataStruct->noteRect->left + 11, noteDataStruct->noteRect->top, noteDataStruct->noteRect->Width() - 11, 10);
	iter->value.value.noteCheckBox->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top, 10, 10);

	ReleaseNoteStruct();
	return true;
}