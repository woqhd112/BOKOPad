#include "pch.h"
#include "ManagerManagement.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKODragDlg.h"
//#include "NoteListCtrl.h"

ComplexMap<int, BOKOScenarioDetailDlg*> ManagerManagement::m_scenarioDlgManager;
ComplexMap<int, int> ManagerManagement::m_scenarioSeqMap;

ManagerManagement::ManagerManagement()
	: m_bAttach(false)
	, m_mainDlg(nullptr)
	, m_pPutScenarioStruct(nullptr)
	, m_pPutNoteStruct(nullptr)
	, m_pPutDragStruct(nullptr)
	, m_dragDlg(new BOKODragDlg)
	, m_dragState(DUS_NOTHING)
{

}

ManagerManagement::~ManagerManagement()
{
	if (m_dragDlg)
	{
		delete m_dragDlg;
		m_dragDlg = nullptr;
	}
}

void ManagerManagement::AttachManager(CWnd* dlgAttachPointer)
{
	m_bAttach = true;
	m_mainDlg = dlgAttachPointer;

	m_dragDlg->Create(m_dragDlg->IDD, m_mainDlg);
}

CWnd* ManagerManagement::DetachManager()
{
	m_bAttach = false;

	return m_mainDlg;
}

void ManagerManagement::InputScenarioStruct(ScenarioManagerStruct* scenarioDataStruct)
{
	m_pPutScenarioStruct = scenarioDataStruct;
}

void ManagerManagement::InputNoteStruct(NoteManagerStruct* noteDataStruct)
{
	m_pPutNoteStruct = noteDataStruct;
}

void ManagerManagement::InputDragStruct(DragDataStruct* dragDataStruct)
{
	m_pPutDragStruct = dragDataStruct;
}

ScenarioManagerStruct* ManagerManagement::BringScenarioStruct() const
{
	return m_pPutScenarioStruct;
}

void ManagerManagement::ReleaseScenarioStruct()
{
	m_pPutScenarioStruct = nullptr;
}

NoteManagerStruct* ManagerManagement::BringNoteStruct() const
{
	return m_pPutNoteStruct;
}

void ManagerManagement::ReleaseNoteStruct()
{
	m_pPutNoteStruct = nullptr;
}

DragDataStruct* ManagerManagement::BringDragStruct() const
{
	return m_pPutDragStruct;
}

void ManagerManagement::ReleaseDragStruct()
{
	m_pPutDragStruct = nullptr;
}

DragUpState ManagerManagement::GetDragState() const
{
	return m_dragState;
}

void ManagerManagement::SortScenarioManagerKey()
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

void ManagerManagement::SortNoteManagerKey()
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