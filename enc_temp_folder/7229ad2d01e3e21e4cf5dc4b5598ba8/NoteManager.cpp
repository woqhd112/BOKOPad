#include "pch.h"
#include "NoteManager.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKODragDlg.h"
//#include "NoteListCtrl.h"


static unsigned int g_notePadID = 10000;

NoteManager::NoteManager()
	: m_variableCtrlPos_x(0)
	, m_variableCtrlPos_y(0)
	, m_bCursorAttach(false)
	, m_bCursorDetach(true)
{

}

NoteManager::~NoteManager()
{
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	while (iter != m_notePadManager.end())
	{
		NotePadStruct noteStruct = iter->value.value;
		CEdit* noteEdit = noteStruct.noteEdit;
		CButton* noteButton = noteStruct.noteButton;
		noteEdit->DestroyWindow();
		delete noteEdit;
		noteEdit = nullptr;
		noteButton->DestroyWindow();
		delete noteButton;
		noteButton = nullptr;

		iter++;
	}

	m_notePadManager.clear();

}

bool NoteManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(message);
}


bool NoteManager::HelpInvoker(PerformanceMessage message)
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
	else if (message == PM_DRAG_MOVE)
	{
		bHelpSuccess = DragMove();
	}
	else if (message == PM_DRAG_DOWN)
	{
		bHelpSuccess = DragDown();
	}
	else if (message == PM_DRAG_UP)
	{
		bHelpSuccess = DragUp();
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

	return bHelpSuccess;
}

bool NoteManager::Insert()
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

	bool bCreate1 = false, bCreate2 = false;
	bCreate1 = (bool)noteInputEdit->Create(WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER | ES_MULTILINE, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);
	bCreate2 = (bool)noteInputButton->Create("...", WS_VISIBLE  | BS_CENTER, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);

	if (bCreate1 && bCreate2)
	{
		try
		{
			NotePadStruct padStruct;
			padStruct.noteButton = noteInputButton;
			padStruct.noteEdit = noteInputEdit;
			m_notePadManager.insert(noteManagerStruct.noteIndex, padStruct);
			m_noteSeqMap.insert(noteManagerStruct.noteIndex, noteDataStruct->noteData->GetNotSEQ());
		}
		catch (ComplexDuplicateException e)
		{
			// Ű �ߺ��� �����ϰ� ����
			noteInputEdit->DestroyWindow();
			delete noteInputEdit;
			noteInputEdit = nullptr;
			noteInputButton->DestroyWindow();
			delete noteInputButton;
			noteInputButton = nullptr;
			ReleaseNoteStruct();
			return false;
		}

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

bool NoteManager::Delete()
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
		// ��ư, ������Ʈ�� �����ϱ�
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
	CEdit* deleteEdit = iter2->value.value.noteEdit;
	
	delete deleteButton;
	delete deleteEdit;
	deleteButton = nullptr;
	deleteEdit = nullptr;

	int findKey = iter2->value.key;
	m_notePadManager.erase(findKey);
	m_noteSeqMap.erase(findKey);

	SortNoteManagerKey();

	return true;
}

bool NoteManager::Clear()
{
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	while (iter != m_notePadManager.end())
	{
		NotePadStruct noteStruct = iter->value.value;
		CEdit* noteEdit = noteStruct.noteEdit;
		CButton* noteButton = noteStruct.noteButton;
		noteEdit->DestroyWindow();
		delete noteEdit;
		noteEdit = nullptr;
		noteButton->DestroyWindow();
		delete noteButton;
		noteButton = nullptr;

		iter++;
	}

	m_notePadManager.clear();
	m_noteSeqMap.clear();

	return true;
}

bool NoteManager::Show()
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
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);

	ReleaseNoteStruct();

	return true;
}

bool NoteManager::Hide()
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
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	ReleaseNoteStruct();

	return true;
}

bool NoteManager::Move()
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

	iter->value.value.noteButton->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top, noteDataStruct->noteRect->Width(), 10);
	iter->value.value.noteEdit->MoveWindow(noteDataStruct->noteRect->left, noteDataStruct->noteRect->top + 10, noteDataStruct->noteRect->Width(), noteDataStruct->noteRect->Height());

	ReleaseNoteStruct();

	return true;
}

bool NoteManager::DragDown()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
		return false;

	if (m_noteSeqMap.empty())
		return false;

	// ��ư ���̵�� ��Ʈ���� ã��
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
			return false;

		CString strEditContent;
		iter->value.value.noteEdit->GetWindowTextA(strEditContent);

		dragDataStruct->noteIndex = iter2->value.key;
		dragDataStruct->noteSEQ = iter2->value.value;
		dragDataStruct->noteCONTENT = strEditContent.GetBuffer();
	}
	else
	{
		ReleaseDragStruct();
		return false;
	}

	m_dragState = DUS_NOTHING;
	m_dragDlg->SetDragData(dragDataStruct->noteSEQ, dragDataStruct->noteIndex, dragDataStruct->noteCONTENT);
	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);
	m_dragDlg->ShowWindow(SW_SHOW);

	iter->value.value.noteButton->ShowWindow(SW_HIDE);
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	//m_cursorEventCnt = ShowCursor(FALSE);
	CursorCountRestore(-1);
	m_mainDlg->SetCapture();
	ReleaseDragStruct();

	return true;
}

bool NoteManager::DragMove()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// ���콺 Ŀ�� �̺�Ʈó�� Ÿ�Ӷ����� ��
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
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
	}


	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);

	ReleaseDragStruct();

	return true;
}

bool NoteManager::DragUp()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	
	// �ش� ��ġ�� ������� ���� ó���ؾ��ϴ� ����� �ٸ�
	// ���콺�� ����Ʈ�� ���� ���̾�αװ� �ƴҶ� ó��
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

	// ���̾�α׸� ã�� ���Ͽ��� ��
	if (iter1 == m_scenarioDlgManager.end())
	{
		m_dragState = DUS_NOTHING;
	}
	// ã�� ���̾�αװ� ���� ���̾�α��� ��
	else if (iter1->value.value == m_mainDlg->GetParent())
	{
		m_dragState = DUS_THIS;
		// Ÿ�Ӷ������� ����

		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_THIS_TIMELINE;

			// ��ư ���̵�� ��Ʈ���� ã��
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
	// ã�� ���̾�αװ� �ٸ� ���̾�α��� ��
	else
	{
		m_dragState = DUS_ANOTHER;

		// �ٸ� ���̾�α��� �� �ش� ���̾�α��� seq��ȣ�� ����ü�� ����
		ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.find(iter1->value.key);
		if (iter2 != m_scenarioSeqMap.end())
			dragDataStruct->target_sceSEQ = iter2->value.value;

		// Ÿ�Ӷ������� ���� (�̰� ó���ұ�..)
		/*CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_ANOTHER_TIMELINE;
		}*/
	}

	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	CursorCountRestore(0);
	CURSOR_ARROW;
	ReleaseCapture();
	return true;
}

bool NoteManager::DragNothing()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// ��ư ���̵�� ��Ʈ���� ã��
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

bool NoteManager::DragThisAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// ��ư ���̵�� ��Ʈ���� ã��
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

bool NoteManager::DragAnotherAttach()
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

	// �ó����� �������ʿ��� �ش� �ó����� ��������ȣ ã�� ��ȣ ������ �ó����� ����Ʈ ����.
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

	// ���� �ó��������� ��Ʈ����
	if (iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// Ÿ�� �ó��������� ��Ʈ�߰�
	if (iter3->value.value->SignalInsertNote(dragDataStruct->noteCONTENT) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return false;
}

bool NoteManager::DragThisTimelineAttach()
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

bool NoteManager::DragAnotherTimelineAttach()
{
	bool bSuccess = false;

	return bSuccess;
}

bool NoteManager::FindNoteIndex()
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

bool NoteManager::RollbackAnotherAttach()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// �ó����� �������ʿ��� �ش� �ó����� ��������ȣ ã�� ��ȣ ������ �ó����� ����Ʈ ����.
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

	// ���� �ó����� ���ε�
	if (iter2->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// Ÿ�� �ó����� ���ε�
	if (iter3->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();
	return true;
}