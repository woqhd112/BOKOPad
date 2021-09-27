#pragma once
#include "DlgModel.h"

void PrepareStatementSelectInSceSEQNoteInformation();
void ResultSetSelectInSceSEQNoteInformation();
void PrepareStatementSelectOneNoteInformation();
void ResultSetSelectOneNoteInformation();
void PrepareStatementInsertNoteInformation();
void PrepareStatementUpdateNoteInformation();
void PrepareStatementDeleteNoteInformation();
void PrepareStatementUpdateNoteInformationInNotCONTENT();
void PrepareStatementUpdateNoteInformationInNotLOCK();
void PrepareStatementSelectInSceSEQScenarioListInNotSEQ();
void ResultSetSelectInSceSEQScenarioListInNotSEQ();
void PrepareStatementUpdateScenarioListInNotCNT_SUB();
void PrepareStatementSelectInNotCNTScenarioListInSceSEQ();
void ResultSetSelectInNotCNTScenarioListInSceSEQ();

class NoteInformationDlgModelImp : public DlgModel
{
public:

	NoteInformationDlgModelImp();
	virtual ~NoteInformationDlgModelImp();

	bool SelectInSceSEQNoteInformation(int in_sceSEQ, ComplexVector<NoteInformationVO>* out_noteInformationContainer);
	bool SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out_noteInformation);
	bool InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ);
	bool UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ);
	bool DeleteNoteInformation(int in_notSEQ);
	bool UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ);
	bool UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ);
	bool SelectInSceSEQScenarioListInNotSEQ(int in_notSEQ, ScenarioListVO* out_scenarioList);
	bool UpdateScenarioListInNotCNT(int in_sceSEQ, int in_notCNT);
	bool SelectInNotCNTScenarioListInSceSEQ(int in_sceSEQ, ScenarioListVO* out_scenarioList);

private:


};

