#include "pch.h"
#include "NoteManager.h"
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

	return bHelpSuccess;
}

bool NoteManager::Insert()
{
	ComplexScopedLock lock(&m_processLock);

	NoteManagerStruct* noteDataStruct = BringNoteStruct();

	if (noteDataStruct == nullptr)
		return false;

	if (noteDataStruct->noteIndex < 0)
		return false;

	bool bSuccess = false;

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

	return bSuccess;
}

bool NoteManager::DragDown()
{
	ComplexScopedLock lock(&m_processLock);

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

		dragDataStruct->noteIndex = iter2->value.key;
		dragDataStruct->noteSEQ = iter2->value.value;
	}
	else
		return false;

	m_dragDlg->SetDragData(dragDataStruct->noteSEQ, dragDataStruct->noteIndex, dragDataStruct->noteCONTENT);
	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);
	m_dragDlg->ShowWindow(SW_SHOW);

	iter->value.value.noteButton->ShowWindow(SW_HIDE);
	iter->value.value.noteEdit->ShowWindow(SW_HIDE);

	// 같은거 다시 들었을때 안먹히네..

	ShowCursor(FALSE);
	//m_mainDlg->SetCapture();
	ReleaseDragStruct();

	return true;
}

bool NoteManager::DragMove()
{
	ComplexScopedLock lock(&m_processLock);

	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
		return false;

	if (m_dragDlg->IsWindowVisible() == false)
		return false;

	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);


	ReleaseDragStruct();

	return true;
}

bool NoteManager::DragUp()
{
	ComplexScopedLock lock(&m_processLock);

	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
		return false;

	if (m_dragDlg->IsWindowVisible() == false)
		return false;

	// 해당 위치가 어딘지에 따라 처리해야하는 방법이 다름
	CRect rect;
	m_mainDlg->GetWindowRect(rect);
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	if(PtInRect(rect, pt) == false)
	{
		// 마우스의 포인트가 현재 다이얼로그가 아닐때 처리
		// 시나리오 매니저에서 다이얼로그 리스트를 가져와야하는데..
	}


	// 공용 처리구역
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
		iter->value.value.noteButton->ShowWindow(SW_SHOW);
		iter->value.value.noteEdit->ShowWindow(SW_SHOW);
	}
	else
		return false;

	//m_dragDlg->DestroyWindow();
	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	ShowCursor(TRUE);
	//ReleaseCapture();

	return true;
}