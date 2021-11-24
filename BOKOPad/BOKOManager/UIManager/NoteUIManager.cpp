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
		CustomEditCtrl* noteEdit = noteStruct.noteEdit;
		CustomButton* noteButton = noteStruct.noteButton;
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

	return InvokeHelper(message);
}


bool NoteUIManager::InvokeHelper(PerformanceMessage message)
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
	else if (message == PM_INSERT_NOTE_VIEW_UPDATE)
	{
		bHelpSuccess = InsertNoteViewUpdate();
	}
	else if (message == PM_SEQ_NOTE_VIEW_UPDATE)
	{
		bHelpSuccess = SeqNoteViewUpdate();
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

	CustomEditCtrl* noteInputEdit = new CustomEditCtrl(noteManagerStruct.noteData->GetNotSEQ());
	CustomButton* noteInputButton = new CustomButton;
	CButton* noteInputCheckBox = new CButton;
	
	bool bCreate1 = false, bCreate2 = false, bCreate3 = false;
	bCreate1 = (bool)noteInputEdit->Create(CustomEditCtrl::IDD, m_mainDlg);
	bCreate2 = (bool)noteInputButton->Create("...", WS_VISIBLE | BS_CENTER | BS_OWNERDRAW, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);
	bCreate3 = (bool)noteInputCheckBox->Create("", WS_VISIBLE | BS_AUTOCHECKBOX, CRect(0, 0, 0, 0), m_mainDlg, g_notePadID++);


	if (bCreate1 && bCreate2)
	{
		try
		{
			noteInputEdit->MoveWindows(noteManagerStruct.noteRect->left, noteManagerStruct.noteRect->top + 10, noteManagerStruct.noteRect->Width(), noteManagerStruct.noteRect->Height());
			noteInputEdit->Initialize();
			noteInputEdit->SetFont(&m_setFont);
			noteInputEdit->ExecuteTimer();
			noteInputButton->Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("���"), 16, FW_BOLD, CBT_DEFAULT, CBE_TOGGLE);

			NotePadStruct padStruct;
			padStruct.bOpenView = noteDataStruct->noteView;
			padStruct.noteButton = noteInputButton;
			padStruct.noteEdit = noteInputEdit;
			padStruct.noteCheckBox = noteInputCheckBox;
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

		
		noteInputEdit->SetWindowTexts(noteManagerStruct.noteData->GetNotCONTENT().GetBuffer());
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

	CustomButton* deleteButton = iter2->value.value.noteButton;
	CButton* deleteCheckBox = iter2->value.value.noteCheckBox;
	CustomEditCtrl* deleteEdit = iter2->value.value.noteEdit;
	
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
		CustomEditCtrl* noteEdit = noteStruct.noteEdit;
		CustomButton* noteButton = noteStruct.noteButton;
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
	iter->value.value.noteCheckBox->SetCheck(FALSE);
	// �巡�׻��¿����� ó���Ǵ� �Լ��̴� �ּ�
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
	// �巡�� ���¿����� ó���Ǵ� �Լ��̴� �ּ�
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

	CustomButton* deleteButton = iter2->value.value.noteButton;
	CButton* deleteCheckBox = iter2->value.value.noteCheckBox;
	CustomEditCtrl* deleteEdit = iter2->value.value.noteEdit;

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

	// ��ư ���̵�� ��Ʈ���� ã��
	ComplexMap<int, NotePadStruct>::iterator iter1 = m_notePadManager.begin();

	int findKey = -1;

	if (dragDataStruct->pushShiftButton)
	{
		int beforeCheckKey = -1;
		while (iter1 != m_notePadManager.end())
		{
			if (iter1->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
			{
				findKey = iter1->value.key;
				break;
			}
			iter1++;
		}

		iter1 = m_notePadManager.begin();
		while (iter1 != m_notePadManager.end())
		{
			if (iter1->value.value.noteButton->GetDlgCtrlID() != dragDataStruct->buttonID && iter1->value.value.noteCheckBox->GetCheck())
			{
				beforeCheckKey = iter1->value.key;
				break;
			}
			iter1++;
		}

		int startKey = -1, endKey = -1;

		if (beforeCheckKey < findKey)
		{
			startKey = beforeCheckKey;
			endKey = findKey;
		}
		else
		{
			startKey = findKey;
			endKey = beforeCheckKey;
		}

		if (startKey == -1)
		{
			ReleaseDragStruct();
			return false;
		}

		if (endKey == -1)
		{
			ReleaseDragStruct();
			return false;
		}

		for (int i = startKey; i <= endKey; i++)
		{
			ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.find(i);
			if (iter2 != m_notePadManager.end())
			{
				if (iter2->value.value.bOpenView)
				{
					iter2->value.value.noteCheckBox->SetCheck(TRUE);
					iter2->value.value.noteButton->ExecuteClickButtonColor();
				}
			}
		}

		ReleaseDragStruct();

		return true;
	}
	else
	{
		if (dragDataStruct->pushCtrlButton == false)
		{
			while (iter1 != m_notePadManager.end())
			{
				if (iter1->value.value.noteButton->GetDlgCtrlID() != dragDataStruct->buttonID)
				{
					if (iter1->value.value.bOpenView)
					{
						iter1->value.value.noteCheckBox->SetCheck(FALSE);
						iter1->value.value.noteButton->ResetClickButtonColor();
					}
				}
				else
				{
					findKey = iter1->value.key;
				}
				iter1++;
			}
		}
		else
		{
			while (iter1 != m_notePadManager.end())
			{
				if (iter1->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
				{
					findKey = iter1->value.key;
					break;
				}
				iter1++;
			}
		}
	}

	/*iter1 = m_notePadManager.begin();
	while (iter1 != m_notePadManager.end())
	{
		if (iter1->value.value.noteButton->GetDlgCtrlID() == dragDataStruct->buttonID)
		{
			break;
		}
		iter1++;
	}

	if (iter1 == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}*/
	if (findKey == -1)
	{
		ReleaseNoteStruct();
		return false;
	}

	iter1 = m_notePadManager.find(findKey);
	if (iter1 == m_notePadManager.end())
	{
		ReleaseNoteStruct();
		return false;
	}

	if (iter1->value.value.bOpenView)
	{
		if (iter1->value.value.noteCheckBox->GetCheck() == TRUE)
		{
			iter1->value.value.noteCheckBox->SetCheck(FALSE);
			iter1->value.value.noteButton->ResetClickButtonColor();
		}
		else
		{
			iter1->value.value.noteCheckBox->SetCheck(TRUE);
			iter1->value.value.noteButton->ExecuteClickButtonColor();
		}
	}

	ReleaseDragStruct();

	return true;
}

bool NoteUIManager::SeqNoteViewUpdate()
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
	int notIndex = -1;

	ComplexMap<int, int>::iterator iter1 = m_noteSeqMap.begin();

	while (iter1 != m_noteSeqMap.end())
	{
		if (iter1->value.value == notSEQ)
		{
			notIndex = iter1->value.key;
			break;
		}
		iter1++;
	}

	if (notIndex <= -1)
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.find(notIndex);
	if (iter2 != m_notePadManager.end())
	{
		iter2->value.value.bOpenView = noteDataStruct->noteView;
	}

	ReleaseNoteStruct();
	return true;
}

bool NoteUIManager::InsertNoteViewUpdate()
{
	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (m_notePadManager.empty())
	{
		ReleaseNoteStruct();
		return false;
	}

	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();
	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteCheckBox->GetCheck() == TRUE)
		{
			iter->value.value.bOpenView = noteDataStruct->noteView;
		}
		iter++;
	}

	ReleaseNoteStruct();
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

	ComplexMap<int, NotePadStruct>::iterator iter1 = m_notePadManager.begin();
	int checkCount = 0;
	while (iter1 != m_notePadManager.end())
	{
		if (iter1->value.value.bOpenView)
		{
			if (iter1->value.value.noteCheckBox->GetCheck() == TRUE)
			{
				checkCount++;
				iter1->value.value.noteButton->ShowWindow(SW_HIDE);
				iter1->value.value.noteEdit->ShowWindow(SW_HIDE);
			}
		}

		iter1++;
	}

	if (checkCount < 1)
	{
		ReleaseDragStruct();
		return false;
	}

	m_dragState = DUS_NOTHING;
	//m_dragDlg->SetDragData(dragDataStruct->noteSEQ, dragDataStruct->noteIndex, dragDataStruct->noteCONTENT);
	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);
	m_dragDlg->SetDragData(checkCount);
	m_dragDlg->ShowWindow(SW_SHOW);

	//iter->value.value.noteButton->ShowWindow(SW_HIDE);
	//iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	//m_cursorEventCnt = ShowCursor(FALSE);
	CursorCountRestore(-1);
	//m_mainDlg->SetCapture();
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
			//TRACE("%d��° �ó����� ����\n", iter1->value.key);
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

	// ���콺 Ŀ�� �̺�Ʈó�� Ÿ�Ӷ����� ��
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

	// �ٿ��̺�Ʈ�� ������� ���� �ݹ��Լ��� �ȸ��� ���꿡�� �ѹ��� Ŀ�� ���̵� ��Ŵ 
	// �켱 �ӽ�ó����
	if (m_cursorEventCnt == -1)
	{
		CursorCountRestore(-2);
	}

	m_mainDlg->SetCapture();
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
		dragDataStruct->target_sceSEQ = -1;
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

	// ��ư ���̵�� ��Ʈ���� ã��
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	/*bool bFind = false;
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
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);*/

	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteCheckBox->GetCheck() == TRUE)
		{
			iter->value.value.noteButton->ShowWindow(SW_SHOW);
			iter->value.value.noteEdit->ShowWindow(SW_SHOW);
		}
		iter++;
	}

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

	// ��ư ���̵�� ��Ʈ���� ã��
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.begin();

	/*bool bFind = false;
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
	iter->value.value.noteEdit->ShowWindow(SW_SHOW);*/

	while (iter != m_notePadManager.end())
	{
		if (iter->value.value.noteCheckBox->GetCheck() == TRUE)
		{
			iter->value.value.noteButton->ShowWindow(SW_SHOW);
			iter->value.value.noteEdit->ShowWindow(SW_SHOW);
		}
		iter++;
	}

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

	ComplexMap<int, NotePadStruct>::iterator iter4 = m_notePadManager.begin();

	ComplexVector<int> seqContainer;

	// �켱 ��Ϻ����ϰ� ������ �����������̳ʿ� ��� ���ʷ� �����Ѵ�.
	while (iter4 != m_notePadManager.end())
	{
		if (iter4->value.value.noteCheckBox->GetCheck() == TRUE)
		{
			ComplexMap<int, int>::iterator iter5 = m_noteSeqMap.find(iter4->value.key);
			if (iter5 != m_noteSeqMap.end())
			{
				// notSEQ
				seqContainer.push_back(iter5->value.value);
				CString strContent;
				iter4->value.value.noteEdit->GetWindowTexts(strContent);

				// Ÿ�� �ó��������� ��Ʈ�߰�
				if (iter3->value.value->SignalInsertNote(strContent.GetBuffer(), true) == false)
				{
					ReleaseDragStruct();
					return false;
				}
			}
		}
		iter4++;
	}

	for (int i = 0; i < seqContainer.size(); i++)
	{
		int notSEQ = seqContainer.at(i);
		// ���� �ó��������� ��Ʈ����
		if (iter2->value.value->SignalDeleteNote(notSEQ) == false)
		{
			ReleaseDragStruct();
			return false;
		}
	}

	//// ���� �ó��������� ��Ʈ����
	//if (iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ) == false)
	//{
	//	ReleaseDragStruct();
	//	return false;
	//}

	//// Ÿ�� �ó��������� ��Ʈ�߰�
	//if (iter3->value.value->SignalInsertNote(dragDataStruct->noteCONTENT, true) == false)
	//{
	//	ReleaseDragStruct();
	//	return false;
	//}

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

	ComplexMap<int, NotePadStruct>::iterator iter2 = m_notePadManager.begin();

	int checkCount = 0;
	while (iter2 != m_notePadManager.end())
	{
		if (iter2->value.value.noteCheckBox->GetCheck() == TRUE)
			checkCount++;

		iter2++;
	}

	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	if (checkCount > 1)
	{
		if (iter1->value.value->SignalGetTimelineCount() > 0)
			iter2 = m_notePadManager.rbegin();
		else
			iter2 = m_notePadManager.begin();

		while (iter2 != m_notePadManager.end())
		{
			if (iter2->value.value.noteCheckBox->GetCheck() == TRUE)
			{
				ComplexMap<int, int>::iterator iter3 = m_noteSeqMap.find(iter2->value.key);
				if (iter3 != m_noteSeqMap.end())
				{
					if (iter1->value.value->SignalInsertTimeline(iter3->value.value, pt) == false)
					{
						ReleaseDragStruct();
						return false;
					}

					if (iter1->value.value->SignalUpdateSetTIME(iter3->value.value, true) == false)
					{
						ReleaseDragStruct();
						return false;
					}
				}
			}

			iter2++;
		}

		if (iter1->value.value->SignalReloadNoteList() == false)
		{
			ReleaseDragStruct();
			return false;
		}
	}
	else
	{
		if (iter1->value.value->SignalInsertTimeline(dragDataStruct->noteSEQ, pt) == false)
		{
			ReleaseDragStruct();
			return false;
		}

		if (iter1->value.value->SignalUpdateSetTIME(dragDataStruct->noteSEQ, true) == false)
		{
			ReleaseDragStruct();
			return false;
		}

		if (iter1->value.value->SignalReloadNoteList() == false)
		{
			ReleaseDragStruct();
			return false;
		}
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

	ComplexMap<int, NotePadStruct>::iterator iter4 = m_notePadManager.begin();
	int checkCount = 0;
	while (iter4 != m_notePadManager.end())
	{
		if (iter4->value.value.noteCheckBox->GetCheck() == TRUE)
			checkCount++;

		iter4++;
	}


	if (checkCount > 1)
	{
		ComplexVector<int> seqContainer;
		if (iter3->value.value->SignalGetTimelineCount() > 0)
			iter4 = m_notePadManager.rbegin();
		else
			iter4 = m_notePadManager.begin();

		while (iter4 != m_notePadManager.end())
		{
			if (iter4->value.value.noteCheckBox->GetCheck() == TRUE)
			{
				ComplexMap<int, int>::iterator iter5 = m_noteSeqMap.find(iter4->value.key);
				CString strText;
				iter4->value.value.noteEdit->GetWindowTexts(strText);
				seqContainer.push_back(iter5->value.value);
				// Ÿ�� �ó��������� ��Ʈ�߰�
				if (iter3->value.value->SignalInsertNote(strText.GetBuffer(), false) == false)
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
				// Ÿ�� Ÿ�Ӷ��� �߰�
				// pt �� �������Ѿ���.. ����ұ�
				if (iter3->value.value->SignalInsertTimeline(lastInsertNoteSEQ, pt) == false)
				{
					ReleaseDragStruct();
					return false;
				}
			}
			iter4++;
		}

		for (int i = 0; i < seqContainer.size(); i++)
		{
			int notSEQ = seqContainer.at(i);
			// ���� �ó��������� ��Ʈ����
			if (iter2->value.value->SignalDeleteNote(notSEQ) == false)
			{
				ReleaseDragStruct();
				return false;
			}
		}

		if (iter2->value.value->SignalReloadNoteList() == false)
		{
			ReleaseDragStruct();
			return false;
		}
	}
	else
	{
		// ���� �ó��������� ��Ʈ����
		if (iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ) == false)
		{
			ReleaseDragStruct();
			return false;
		}

		// Ÿ�� �ó��������� ��Ʈ�߰�
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
		// Ÿ�� Ÿ�Ӷ��� �߰�
		if (iter3->value.value->SignalInsertTimeline(lastInsertNoteSEQ, pt) == false)
		{
			ReleaseDragStruct();
			return false;
		}
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

	// ���� �ó����� ���ε�
	if (iter->value.value->SignalReloadNoteList() == false)
	{
		ReleaseDragStruct();
		return false;
	}

	// ���� Ÿ�Ӷ��� ���ε�
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

	// ���� Ÿ�Ӷ��� ���ε�
	if (iter2->value.value->SignalReloadTimeline() == false)
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

	// ���� Ÿ�Ӷ��� ���ε�
	if (iter2->value.value->SignalReloadTimeline() == false)
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

	// Ÿ�� Ÿ�Ӷ��� ���ε�
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
	iter->value.value.noteCheckBox->SetCheck(FALSE);	// �巡�׸�� �����ϸ� üũ�ߴ� üũ�ڽ��� ���� ����
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