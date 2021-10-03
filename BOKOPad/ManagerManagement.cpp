#include "pch.h"
#include "ManagerManagement.h"
#include "BOKODragDlg.h"
//#include "NoteListCtrl.h"

std::shared_ptr<DragProcess> ManagerManagement::m_dragProc;

ManagerManagement::ManagerManagement()
	: m_bAttach(false)
	, m_mainDlg(nullptr)
	, m_pPutScenarioStruct(nullptr)
	, m_pPutNoteStruct(nullptr)
	, m_pPutDragStruct(nullptr)
	, m_dragDlg(new BOKODragDlg)
{
	if (m_dragProc.get() == NULL)
	{
		m_dragProc.reset(new DragProcess);
		//m_dragProc->Start();
	}
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
	//m_dragProc->Stop();
	//m_dragProc->Join();

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