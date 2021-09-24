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
	bool UpdatePadOption(int in_option1, int in_option2);

	// scenario list control
	bool InsertScenarioList(ComplexString in_title);
	bool SelectAllScenarioList();
	bool UpdateScenarioList(ComplexString in_title, int in_cnt, int update_seq);
	bool DeleteScenarioList(int delete_seq);
	bool UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ);
	bool UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ);

	// note information control
	bool SelectInSceSEQNoteInformation(int in_sceSEQ);
	bool SelectOneNoteInformation(int in_notSEQ);
	bool InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ);
	bool UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ);
	bool DeleteNoteInformation(int in_notSEQ);
	bool UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ);
	bool UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ);

private:

	MainDlgService* m_mainDlgService;
	ScenarioListDlgService* m_scenarioListDlgService;
	NoteInformationDlgService* m_noteInformationDlgService;
};

