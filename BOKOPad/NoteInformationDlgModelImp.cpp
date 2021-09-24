#include "pch.h"
#include "NoteInformationDlgModelImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PrepareStatementSelectInSceSEQNoteInformation()
{
	int sceSEQ;
	DATA_INSTANCE->GetNoteInformation(NULL, &sceSEQ, NULL, NULL);
	DB_INSTANCE->SetInt(1, sceSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void ResultSetSelectInSceSEQNoteInformation()
{
	DATA_INSTANCE->SetNoteInformation(DB_INSTANCE->GetInt(0), DB_INSTANCE->GetInt(3), (bool)DB_INSTANCE->GetInt(2), DB_INSTANCE->GetString(1));
}

void PrepareStatementSelectOneNoteInformation()
{
	int notSEQ;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, NULL, NULL);
	DB_INSTANCE->SetInt(1, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void ResultSetSelectOneNoteInformation()
{
	DATA_INSTANCE->SetNoteInformation(DB_INSTANCE->GetInt(0), DB_INSTANCE->GetInt(3), (bool)DB_INSTANCE->GetInt(2), DB_INSTANCE->GetString(1));
}

void PrepareStatementInsertNoteInformation()
{
	int sceSEQ;
	bool notLOCK;
	ComplexString notCONTENT;
	DATA_INSTANCE->GetNoteInformation(NULL, &sceSEQ, &notLOCK, &notCONTENT);
	DB_INSTANCE->SetString(1, notCONTENT);
	DB_INSTANCE->SetInt(2, notLOCK);
	DB_INSTANCE->SetInt(3, sceSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void PrepareStatementUpdateNoteInformation()
{
	int notSEQ;
	bool notLOCK;
	ComplexString notCONTENT;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, &notLOCK, &notCONTENT);
	DB_INSTANCE->SetString(1, notCONTENT);
	DB_INSTANCE->SetInt(2, notLOCK);
	DB_INSTANCE->SetInt(3, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void PrepareStatementDeleteNoteInformation()
{
	int notSEQ;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, NULL, NULL);
	DB_INSTANCE->SetInt(1, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void PrepareStatementUpdateNoteInformationInNotCONTENT()
{
	int notSEQ;
	ComplexString notCONTENT;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, NULL, &notCONTENT);
	DB_INSTANCE->SetString(1, notCONTENT);
	DB_INSTANCE->SetInt(2, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void PrepareStatementUpdateNoteInformationInNotLOCK()
{
	int notSEQ;
	bool notLOCK;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, &notLOCK, NULL);
	DB_INSTANCE->SetInt(1, (int)notLOCK);
	DB_INSTANCE->SetInt(2, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void PrepareStatementSelectInSceSEQScenarioListInNotSEQ()
{
	int notSEQ;
	DATA_INSTANCE->GetNoteInformation(&notSEQ, NULL, NULL, NULL);
	DB_INSTANCE->SetInt(1, notSEQ);
	DATA_INSTANCE->ResetNoteInformation();
}

void ResultSetSelectInSceSEQScenarioListInNotSEQ()
{
	DATA_INSTANCE->SetScenarioList(DB_INSTANCE->GetInt(0), "", DB_INSTANCE->GetInt(1));
}

void PrepareStatementUpdateScenarioListInNotCNT_SUB()
{
	int sceSEQ, notCNT;
	DATA_INSTANCE->GetScenarioList(&sceSEQ, &notCNT, NULL);
	DB_INSTANCE->SetInt(1, notCNT);
	DB_INSTANCE->SetInt(2, sceSEQ);
	DATA_INSTANCE->ResetScenarioList();
}

void PrepareStatementSelectInNotCNTScenarioListInSceSEQ()
{
	int sceSEQ;
	DATA_INSTANCE->GetScenarioList(&sceSEQ, NULL, NULL);
	DB_INSTANCE->SetInt(1, sceSEQ);
	DATA_INSTANCE->ResetScenarioList();
}

void ResultSetSelectInNotCNTScenarioListInSceSEQ()
{
	DATA_INSTANCE->SetScenarioList(0, "", DB_INSTANCE->GetInt(0));
}

NoteInformationDlgModelImp::NoteInformationDlgModelImp()
{

}

NoteInformationDlgModelImp::~NoteInformationDlgModelImp()
{

}

bool NoteInformationDlgModelImp::SelectInSceSEQNoteInformation(int in_sceSEQ, ComplexVector<NoteInformationVO>* out_noteInformationContainer)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(0, in_sceSEQ, false, "");
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_IN_SEQ_NOTE_INFORMATION_TABLE], PrepareStatementSelectInSceSEQNoteInformation, ResultSetSelectInSceSEQNoteInformation, NULL);

	if (bSuccess)
		DATA_INSTANCE->GetNoteInformationContainer(out_noteInformationContainer);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out_noteInformation)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, false, "");
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_ONE_NOTE_INFORMATION_TABLE], PrepareStatementSelectOneNoteInformation, ResultSetSelectOneNoteInformation, NULL);

	if (bSuccess)
		DATA_INSTANCE->GetNoteInformation(out_noteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(0, in_sceSEQ, in_notLOCK, in_notCONTENT);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[INSERT_NOTE_INFORMATION_TABLE], PrepareStatementInsertNoteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, in_notLOCK, in_notCONTENT);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_NOTE_INFORMATION_TABLE], PrepareStatementUpdateNoteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::DeleteNoteInformation(int in_notSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, false, "");
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[DELETE_NOTE_INFORMATION_TABLE], PrepareStatementDeleteNoteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, false, in_notCONTENT);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_NOTE_INFORMATION_TABLE_IN_NOTCONTENT], PrepareStatementDeleteNoteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, in_notLOCK, "");
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_NOTE_INFORMATION_TABLE_IN_NOTLOCK], PrepareStatementDeleteNoteInformation);

	DATA_INSTANCE->ResetNoteInformation();

	return bSuccess;
}

bool NoteInformationDlgModelImp::SelectInSceSEQScenarioListInNotSEQ(int in_notSEQ, ScenarioListVO* out_scenarioList)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetNoteInformation(in_notSEQ, 0, false, "");
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_IN_SCESEQ_SCENARIO_LIST_IN_NOTSEQ], PrepareStatementSelectInSceSEQScenarioListInNotSEQ, ResultSetSelectInSceSEQScenarioListInNotSEQ, NULL);

	if (bSuccess)
		DATA_INSTANCE->GetScenarioList(out_scenarioList);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool NoteInformationDlgModelImp::UpdateScenarioListInNotCNT(int in_sceSEQ, int in_notCNT)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(in_sceSEQ, "", in_notCNT);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_SCENARIO_LIST_TABLE_IN_NOTCNT], PrepareStatementUpdateScenarioListInNotCNT_SUB);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool NoteInformationDlgModelImp::SelectInNotCNTScenarioListInSceSEQ(int in_sceSEQ, ScenarioListVO* out_scenarioList)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(in_sceSEQ, "", 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_IN_NOTCNT_SCENARIO_LIST_IN_SCESEQ], PrepareStatementSelectInNotCNTScenarioListInSceSEQ, ResultSetSelectInNotCNTScenarioListInSceSEQ, NULL);

	if (bSuccess)
		DATA_INSTANCE->GetScenarioList(out_scenarioList);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}