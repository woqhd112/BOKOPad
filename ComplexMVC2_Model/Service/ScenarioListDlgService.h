#pragma once
#include "../Model/DAO/ScenarioListDlgModelImp.h"

class ScenarioListDlgService
{
public:

	ScenarioListDlgService();
	~ScenarioListDlgService();

	bool InsertScenarioList(ComplexString in_sceTITLE);
	bool SelectAllScenarioList();
	bool UpdateScenarioList(ComplexString in_sceTITLE, int in_notCNT, int in_sceSEQ);
	bool DeleteScenarioList(int delete_seq);
	bool UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ);
	bool UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ);
	bool SelectInSceSEQScenarioListInSceTITLE(ComplexString in_sceTITLE);
	bool DeleteAllScenarioList();

private:

	ScenarioListDlgModelImp* m_scenarioDlgModel;

};

