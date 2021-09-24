#pragma once
#include "DlgModel.h"

void PrepareStatementInsertScenarioList();
void ResultSetSelectAllScenarioList();
void PrepareStatementUpdateScenarioList();
void PrepareStatementDeleteScenarioList();
void PrepareStatementUpdateScenarioListInSceTITLE();
void PrepareStatementUpdateScenarioListInNotCNT();

class ScenarioListDlgModelImp : public DlgModel
{
public:

	ScenarioListDlgModelImp();
	virtual ~ScenarioListDlgModelImp();

	bool InsertScenarioList(ComplexString in_title);
	bool SelectAllScenarioList(ComplexVector<ScenarioListVO>* scenarioListContainer);
	bool UpdateScenarioList(ComplexString in_title, int in_cnt, int update_seq);
	bool DeleteScenarioList(int delete_seq);
	bool UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ);
	bool UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ);

private:


};

