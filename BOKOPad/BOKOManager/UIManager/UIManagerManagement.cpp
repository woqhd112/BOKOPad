#include "pch.h"
#include "UIManagerManagement.h"
#include "..\..\BOKOScenarioDetailDlg.h"
#include "..\..\BOKODragDlg.h"
//#include "NoteListCtrl.h"

ComplexMap<int, BOKOScenarioDetailDlg*> UIManagerManagement::m_scenarioDlgManager;
ComplexMap<int, int> UIManagerManagement::m_scenarioSeqMap;
DragUpState UIManagerManagement::m_dragState;
int UIManagerManagement::m_cursorEventCnt;
bool UIManagerManagement::m_bIsDragging;

UIManagerManagement::UIManagerManagement()
	: m_bAttach(false)
	, m_mainDlg(nullptr)
	, m_pPutScenarioStruct(nullptr)
	, m_pPutNoteStruct(nullptr)
	, m_pPutDragStruct(nullptr)
	, m_dragDlg(new BOKODragDlg)
{
	m_dragState = DUS_NOTHING;
	m_setFont.CreatePointFont(120, TEXT("±¼¸²"));
}

UIManagerManagement::~UIManagerManagement()
{
	if (m_dragDlg)
	{
		delete m_dragDlg;
		m_dragDlg = nullptr;
	}
	m_setFont.DeleteObject();
}

void UIManagerManagement::AttachManager(CWnd* dlgAttachPointer)
{
	m_bAttach = true;
	m_mainDlg = dlgAttachPointer;

	m_dragDlg->Create(m_dragDlg->IDD, m_mainDlg);
}

CWnd* UIManagerManagement::DetachManager()
{
	m_bAttach = false;

	return m_mainDlg;
}

void UIManagerManagement::InputScenarioStruct(ScenarioManagerStruct* scenarioDataStruct)
{
	m_pPutScenarioStruct = scenarioDataStruct;
}

void UIManagerManagement::InputNoteStruct(NoteManagerStruct* noteDataStruct)
{
	m_pPutNoteStruct = noteDataStruct;
}

void UIManagerManagement::InputDragStruct(DragDataStruct* dragDataStruct)
{
	m_pPutDragStruct = dragDataStruct;
}

ScenarioManagerStruct* UIManagerManagement::BringScenarioStruct() const
{
	return m_pPutScenarioStruct;
}

void UIManagerManagement::ReleaseScenarioStruct()
{
	m_pPutScenarioStruct = nullptr;
}

NoteManagerStruct* UIManagerManagement::BringNoteStruct() const
{
	return m_pPutNoteStruct;
}

void UIManagerManagement::ReleaseNoteStruct()
{
	m_pPutNoteStruct = nullptr;
}

DragDataStruct* UIManagerManagement::BringDragStruct() const
{
	return m_pPutDragStruct;
}

void UIManagerManagement::ReleaseDragStruct()
{
	m_pPutDragStruct = nullptr;
}

DragUpState UIManagerManagement::GetDragState() const
{
	return m_dragState;
}

void UIManagerManagement::SortScenarioManagerKey()
{
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.begin();
	ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.begin();

	ComplexMap<int, BOKOScenarioDetailDlg*> sortManagerMap;
	ComplexMap<int, int> sortSeqMap;
	int i = 0;
	while (iter1 != m_scenarioDlgManager.end())
	{
		sortManagerMap.insert(i, iter1->value.value);
		iter1++;
		i++;
	}

	i = 0;
	while (iter2 != m_scenarioSeqMap.end())
	{
		sortSeqMap.insert(i, iter2->value.value);
		iter2++;
		i++;
	}

	m_scenarioDlgManager.clear();
	m_noteSeqMap.clear();

	m_scenarioDlgManager = sortManagerMap;
	m_scenarioSeqMap = sortSeqMap;
}

void UIManagerManagement::SortNoteManagerKey()
{
	ComplexMap<int, NotePadStruct>::iterator iter1 = m_notePadManager.begin();
	ComplexMap<int, int>::iterator iter2 = m_noteSeqMap.begin();

	ComplexMap<int, NotePadStruct> sortManagerMap;
	ComplexMap<int, int> sortSeqMap;
	int i = 0;
	while (iter1 != m_notePadManager.end())
	{
		sortManagerMap.insert(i, iter1->value.value);
		iter1++;
		i++;
	}

	i = 0;
	while (iter2 != m_noteSeqMap.end())
	{
		sortSeqMap.insert(i, iter2->value.value);
		iter2++;
		i++;
	}

	m_notePadManager.clear();
	m_noteSeqMap.clear();

	m_notePadManager = sortManagerMap;
	m_noteSeqMap = sortSeqMap;
}

void UIManagerManagement::CursorCountRestore(int setCursorCnt)
{
	while (m_cursorEventCnt != setCursorCnt)
	{
		if (m_cursorEventCnt < setCursorCnt)
		{
			m_cursorEventCnt = ShowCursor(TRUE);
		}
		else
		{
			m_cursorEventCnt = ShowCursor(FALSE);
		}
	}

	m_cursorEventCnt = setCursorCnt;
}