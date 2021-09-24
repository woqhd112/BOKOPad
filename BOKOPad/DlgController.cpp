#include "pch.h"
#include "DlgController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DlgController::DlgController()
	: m_mainDlgService(new MainDlgService)
	, m_scenarioListDlgService(new ScenarioListDlgService)
	, m_noteInformationDlgService(new NoteInformationDlgService)
{

}

DlgController::~DlgController()
{
	if (m_mainDlgService)
	{
		delete m_mainDlgService;
		m_mainDlgService = nullptr;
	}

	if (m_scenarioListDlgService)
	{
		delete m_scenarioListDlgService;
		m_scenarioListDlgService = nullptr;
	}

	if (m_noteInformationDlgService)
	{
		delete m_noteInformationDlgService;
		m_noteInformationDlgService = nullptr;
	}
}

bool DlgController::SelectAllPadOption()
{
	return m_mainDlgService->SelectAllPadOption();
}

bool DlgController::UpdatePadOption(int in_option1, int in_option2)
{
	return m_mainDlgService->UpdatePadOption(in_option1, in_option2);
}

bool DlgController::InsertScenarioList(ComplexString in_title)
{
	return m_scenarioListDlgService->InsertScenarioList(in_title);
}

bool DlgController::SelectAllScenarioList()
{
	return m_scenarioListDlgService->SelectAllScenarioList();
}

bool DlgController::UpdateScenarioList(ComplexString in_title, int in_cnt, int update_seq)
{
	return m_scenarioListDlgService->UpdateScenarioList(in_title, in_cnt, update_seq);
}

bool DlgController::DeleteScenarioList(int delete_seq)
{
	return m_scenarioListDlgService->DeleteScenarioList(delete_seq);
}

bool DlgController::UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ)
{
	return m_scenarioListDlgService->UpdateScenarioListInSceTITLE(in_sceTITLE, in_sceSEQ);
}

bool DlgController::UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ)
{
	return m_scenarioListDlgService->UpdateScenarioListInNotCNT(in_notCNT, in_sceSEQ);
}

bool DlgController::SelectInSceSEQNoteInformation(int in_sceSEQ)
{
	return m_noteInformationDlgService->SelectInSceSEQNoteInformation(in_sceSEQ);
}

bool DlgController::SelectOneNoteInformation(int in_notSEQ)
{
	return m_noteInformationDlgService->SelectOneNoteInformation(in_notSEQ);
}

bool DlgController::InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ)
{
	return m_noteInformationDlgService->InsertNoteInformation(in_notCONTENT, in_notLOCK, in_sceSEQ);
}

bool DlgController::UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ)
{
	return m_noteInformationDlgService->UpdateNoteInformation(in_notCONTENT, in_notLOCK, in_notSEQ);
}

bool DlgController::DeleteNoteInformation(int in_notSEQ)
{
	return m_noteInformationDlgService->DeleteNoteInformation(in_notSEQ);
}

bool DlgController::UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ)
{
	return m_noteInformationDlgService->UpdateNoteInformationInNotContent(in_notCONTENT, in_notSEQ);
}

bool DlgController::UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ)
{
	return m_noteInformationDlgService->UpdateNoteInformationInNotLOCK(in_notLOCK, in_notSEQ);
}