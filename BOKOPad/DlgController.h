#pragma once
#include "MainDlgService.h"
#include "ScenarioListDlgService.h"
#include "NoteInformationDlgService.h"

class DlgController
{
public:

	DlgController();
	~DlgController();

	// pad option control
	bool SelectAllPadOption();
	bool UpdatePadOption();

	// scenario list control
	bool InsertScenarioList();
	bool SelectAllScenarioList();
	bool UpdateScenarioList();
	bool DeleteScenarioList();
	bool UpdateScenarioListInSceTITLE();
	bool UpdateScenarioListInNotCNT();

	// note information control
	bool SelectInSceSEQNoteInformation();
	bool SelectOneNoteInformation();
	bool InsertNoteInformation();
	bool UpdateNoteInformation();
	bool DeleteNoteInformation();
	bool UpdateNoteInformationInNotContent();
	bool UpdateNoteInformationInNotLOCK();

private:

	MainDlgService* m_mainDlgService;
	ScenarioListDlgService* m_scenarioListDlgService;
	NoteInformationDlgService* m_noteInformationDlgService;
};

