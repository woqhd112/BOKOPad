#include "pch.h"
#include "NoteManager.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKODragDlg.h"
//#include "NoteListCtrl.h"


static unsigned int g_notePadID = 10000;

NoteManager::NoteManager()
	: m_variableCtrlPos_x(0)
	, m_variableCtrlPos_y(0)
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
			// 키 중복은 삭제하고 리턴
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
		noteInputButton->ShowWindow(SW_SHOW);

		noteInputEdit->MoveWindow(noteManagerStruct.noteRect->left, noteManagerStruct.noteRect->top + 10, noteManagerStruct.noteRect->Width(), noteManagerStruct.noteRect->Height());
		noteInputEdit->SetWindowTextA(noteManagerStruct.noteData->GetNotCONTENT().GetBuffer());
		noteInputEdit->ShowWindow(SW_SHOW);

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

	// 버튼, 에딧컨트롤 삭제하기
	bool bFind = false;
	ComplexMap<int, int>::iterator iter1 = m_noteSeqMap.begin();
	while (iter1 != m_noteSeqMap.end())
	{
		if (iter1->value.value == noteDataStruct->noteData->GetNotSEQ())
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

	CButton* deleteButton = iter2->value.value.noteButton;
	CEdit* deleteEdit = iter2->value.value.noteEdit;
	delete deleteButton;
	delete deleteEdit;
	deleteButton = nullptr;
	deleteEdit = nullptr;

	m_notePadManager.erase(iter2->value.key);
	m_noteSeqMap.erase(iter2->value.key);

	bool bSuccess = false;

	return bSuccess;
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
			return false;

		CString strEditContent;
		iter->value.value.noteEdit->GetWindowTextA(strEditContent);

		dragDataStruct->noteIndex = iter2->value.key;
		dragDataStruct->noteSEQ = iter2->value.value;
		dragDataStruct->noteCONTENT = strEditContent.GetBuffer();
	}
	else
		return false;

	m_dragState = DUS_NOTHING;
	m_dragDlg->SetDragData(dragDataStruct->noteSEQ, dragDataStruct->noteIndex, dragDataStruct->noteCONTENT);
	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);
	m_dragDlg->ShowWindow(SW_SHOW);

	iter->value.value.noteButton->ShowWindow(SW_HIDE);
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	ShowCursor(FALSE);
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

	if (m_dragDlg->IsWindowVisible() == false)
	{
		ReleaseDragStruct();
		return false;
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

	if (m_dragDlg->IsWindowVisible() == false)
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
		// 타임라인인지 구분
	}
	// 찾은 다이얼로그가 다른 다이얼로그일 때
	else
	{
		m_dragState = DUS_ANOTHER;

		// 다른 다이얼로그일 때 해당 다이얼로그의 seq번호를 구조체에 저장
		ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.find(iter1->value.key);
		if (iter2 != m_scenarioSeqMap.end())
			dragDataStruct->target_sceSEQ = iter2->value.value;

		// 타임라인인지 구분
	}

	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	ShowCursor(TRUE);
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

	if (bFind)
	{
		ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter2 = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
		ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);

		// 현재 시나리오에서 노트삭제
		if (iter2 == m_scenarioDlgManager.end())
		{
			ReleaseDragStruct();
			return false;
		}

		iter2->value.value->SignalDeleteNote(dragDataStruct->noteSEQ);

		// 타겟 시나리오에서 노트추가
		if (iter3 == m_scenarioDlgManager.end())
		{
			ReleaseDragStruct();
			return false;
		}

		iter3->value.value->SignalInsertNote(dragDataStruct->noteCONTENT);
	}

	// 버튼, 에딧컨트롤 삭제하기
	ComplexMap<int, NotePadStruct>::iterator iter = m_notePadManager.find(dragDataStruct->noteIndex);
	if (iter == m_notePadManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	CButton* deleteButton = iter->value.value.noteButton;
	CEdit* deleteEdit = iter->value.value.noteEdit;
	delete deleteButton;
	delete deleteEdit;
	deleteButton = nullptr;
	deleteEdit = nullptr;

	m_notePadManager.erase(iter->value.key);
	m_noteSeqMap.erase(iter->value.key);

	return true;
}

bool NoteManager::DragThisTimelineAttach()
{
	bool bSuccess = false;

	return bSuccess;
}

bool NoteManager::DragAnotherTimelineAttach()
{
	bool bSuccess = false;

	return bSuccess;
}