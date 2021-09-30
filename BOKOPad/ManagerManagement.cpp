#include "pch.h"
#include "ManagerManagement.h"


ManagerManagement::ManagerManagement()
	: m_bAttach(false)
	, m_mainDlg(nullptr)
	, m_pPutScenarioStruct(nullptr)
{
	m_pStageManager = new StageManager;
}

ManagerManagement::~ManagerManagement()
{
	if (m_pStageManager)
	{
		delete m_pStageManager;
		m_pStageManager = nullptr;
	}
}

void ManagerManagement::AttachManager(CWnd* dlgAttachPointer)
{
	m_bAttach = true;
	m_mainDlg = dlgAttachPointer;
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