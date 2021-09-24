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

bool DlgController::UpdatePadOption()
{
	PadOptionVO option;
	RequestScope->GetRequestAttributes(&option);
	return m_mainDlgService->UpdatePadOption(option.GetTest1(), option.GetTest2());
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