#include "pch.h"
#include "ScenarioListDlgModelImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PrepareStatementInsertScenarioList()
{
	int seq, cnt;
	ComplexString title;
	DATA_INSTANCE->GetScenarioList(&seq, &cnt, &title);
	DB_INSTANCE->SetString(1, title);
	DATA_INSTANCE->ResetScenarioList();
}

void ResultSetSelectAllScenarioList()
{
	DATA_INSTANCE->SetScenarioList(DB_INSTANCE->GetInt(0), DB_INSTANCE->GetString(1), DB_INSTANCE->GetInt(2));
}

void PrepareStatementUpdateScenarioList()
{
	int seq, cnt;
	ComplexString title;
	DATA_INSTANCE->GetScenarioList(&seq, &cnt, &title);
	DB_INSTANCE->SetString(1, title);
	DB_INSTANCE->SetInt(2, cnt);
	DB_INSTANCE->SetInt(3, seq);
	DATA_INSTANCE->ResetScenarioList();
}

void PrepareStatementDeleteScenarioList()
{
	int seq;
	DATA_INSTANCE->GetScenarioList(&seq, NULL, NULL);
	DB_INSTANCE->SetInt(1, seq);
	DATA_INSTANCE->ResetScenarioList();
}

void PrepareStatementUpdateScenarioListInSceTITLE()
{
	int seq;
	ComplexString title;
	DATA_INSTANCE->GetScenarioList(&seq, NULL, &title);
	DB_INSTANCE->SetString(1, title);
	DB_INSTANCE->SetInt(2, seq);
	DATA_INSTANCE->ResetScenarioList();
}

void PrepareStatementUpdateScenarioListInNotCNT()
{
	int seq, cnt;
	DATA_INSTANCE->GetScenarioList(&seq, &cnt, NULL);
	DB_INSTANCE->SetInt(1, cnt);
	DB_INSTANCE->SetInt(2, seq);
	DATA_INSTANCE->ResetScenarioList();
}

void PrepareStatementSelectInSceSEQScenarioListInSceTITLE()
{
	ComplexString title;
	DATA_INSTANCE->GetScenarioList(NULL, NULL, &title);
	DB_INSTANCE->SetString(1, title);
	DATA_INSTANCE->ResetScenarioList();
}

void ResultSetSelectInSceSEQScenarioListInSceTITLE()
{
	DATA_INSTANCE->SetScenarioList(DB_INSTANCE->GetInt(0), "", 0);
}

ScenarioListDlgModelImp::ScenarioListDlgModelImp()
{

}

ScenarioListDlgModelImp::~ScenarioListDlgModelImp()
{

}

bool ScenarioListDlgModelImp::InsertScenarioList(ComplexString in_title)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(0, in_title, 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[INSERT_SCENARIO_LIST_TABLE], PrepareStatementInsertScenarioList);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::SelectAllScenarioList(ComplexVector<ScenarioListVO>* scenarioListContainer)
{
	bool bSuccess = false;
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_ALL_SCENARIO_LIST_TABLE], NULL, ResultSetSelectAllScenarioList);

	if (bSuccess)
		DATA_INSTANCE->GetScenarioListContainer(scenarioListContainer);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::UpdateScenarioList(ComplexString in_title, int in_cnt, int update_seq)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(update_seq, in_title, in_cnt);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_SCENARIO_LIST_TABLE], PrepareStatementUpdateScenarioList);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::DeleteScenarioList(int delete_seq)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(delete_seq, "", 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[DELETE_SCENARIO_LIST_TABLE], PrepareStatementDeleteScenarioList);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(in_sceSEQ, in_sceTITLE, 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_SCENARIO_LIST_TABLE_IN_SCETITLE], PrepareStatementUpdateScenarioListInSceTITLE);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(in_sceSEQ, "", in_notCNT);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_SCENARIO_LIST_TABLE_IN_NOTCNT], PrepareStatementUpdateScenarioListInNotCNT);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}

bool ScenarioListDlgModelImp::SelectInSceSEQScenarioListInSceTITLE(ComplexString in_sceTITLE, int* out_sceSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetScenarioList(0, in_sceTITLE, 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_IN_SCETITLE_SCENARIO_LIST_IN_SCESEQ], PrepareStatementSelectInSceSEQScenarioListInSceTITLE, ResultSetSelectInSceSEQScenarioListInSceTITLE);

	if (bSuccess)
		DATA_INSTANCE->GetScenarioList(out_sceSEQ, NULL, NULL);

	DATA_INSTANCE->ResetScenarioList();

	return bSuccess;
}