#include "pch.h"
#include "DlgController.h"


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

bool DlgController::UpdatePadOption()
{
	PadOptionVO option;
	RequestScope->GetRequestAttributes(&option);
	return m_mainDlgService->UpdatePadOption(option.GetTest1(), option.GetTest2());
}

bool DlgController::UpdateScenarioListAutoIncrementSeq()
{
	return m_mainDlgService->UpdateScenarioListAutoIncrementSeq();
}

bool DlgController::UpdateNoteInformationAutoIncrementSeq()
{
	return m_mainDlgService->UpdateNoteInformationAutoIncrementSeq();
}

bool DlgController::InsertScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->InsertScenarioList(scenario.GetSceTITLE());
}

bool DlgController::SelectAllScenarioList()
{
	return m_scenarioListDlgService->SelectAllScenarioList();
}

bool DlgController::UpdateScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioList(scenario.GetSceTITLE(), scenario.GetNotCNT(), scenario.GetSceSEQ());
}

bool DlgController::DeleteScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->DeleteScenarioList(scenario.GetSceSEQ());
}

bool DlgController::UpdateScenarioListInSceTITLE()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioListInSceTITLE(scenario.GetSceTITLE(), scenario.GetSceSEQ());
}

bool DlgController::UpdateScenarioListInNotCNT()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioListInNotCNT(scenario.GetNotCNT(), scenario.GetSceSEQ());
}

bool DlgController::SelectInSceSEQScenarioListInSceTITLE()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->SelectInSceSEQScenarioListInSceTITLE(scenario.GetSceTITLE());
}

bool DlgController::SelectInSceSEQNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->SelectInSceSEQNoteInformation(note.GetSceSEQ());
}

bool DlgController::SelectOneNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->SelectOneNoteInformation(note.GetNotSEQ());
}

bool DlgController::InsertNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->InsertNoteInformation(note.GetNotCONTENT(), note.IsNotLOCK(), note.GetSceSEQ());
}

bool DlgController::UpdateNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformation(note.GetNotCONTENT(), note.IsNotLOCK(), note.GetNotSEQ());
}

bool DlgController::DeleteNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->DeleteNoteInformation(note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInNotContent()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInNotContent(note.GetNotCONTENT(), note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInNotLOCK()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInNotLOCK(note.IsNotLOCK(), note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInSceSEQ()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInSceSEQ(note.GetNotSEQ(), note.GetSceSEQ());
}