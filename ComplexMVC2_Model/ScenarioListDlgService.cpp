#include "pch.h"
#include "ScenarioListDlgService.h"

ScenarioListDlgService::ScenarioListDlgService()
	: m_scenarioDlgModel(new ScenarioListDlgModelImp)
{

}

ScenarioListDlgService::~ScenarioListDlgService()
{
	if (m_scenarioDlgModel)
	{
		delete m_scenarioDlgModel;
		m_scenarioDlgModel = nullptr;
	}
}

bool ScenarioListDlgService::InsertScenarioList(ComplexString in_sceTITLE)
{
	bool bSuccess = false;
	bSuccess = m_scenarioDlgModel->InsertScenarioList(in_sceTITLE);

	if (bSuccess)
		m_scenarioDlgModel->Commit();
	else
		m_scenarioDlgModel->Rollback();

	return bSuccess;
}

bool ScenarioListDlgService::SelectAllScenarioList()
{
	bool bSuccess = false;

	ComplexVector<ScenarioListVO> scenarioListContainer;
	bSuccess = m_scenarioDlgModel->SelectAllScenarioList(&scenarioListContainer);

	if (bSuccess)
		RequestScope->SetRequestAttributes(scenarioListContainer);

	return bSuccess;
}

bool ScenarioListDlgService::UpdateScenarioList(ComplexString in_sceTITLE, int in_notCNT, int in_sceSEQ)
{
	bool bSuccess = false;

	bSuccess = m_scenarioDlgModel->UpdateScenarioList(in_sceTITLE, in_notCNT, in_sceSEQ);

	if (bSuccess)
		m_scenarioDlgModel->Commit();
	else
		m_scenarioDlgModel->Rollback();

	return bSuccess;
}

bool ScenarioListDlgService::DeleteScenarioList(int delete_seq)
{
	bool bSuccess = false;

	bSuccess = m_scenarioDlgModel->DeleteScenarioList(delete_seq);

	if (bSuccess)
		m_scenarioDlgModel->Commit();
	else
		m_scenarioDlgModel->Rollback();

	return bSuccess;
}

bool ScenarioListDlgService::UpdateScenarioListInSceTITLE(ComplexString in_sceTITLE, int in_sceSEQ)
{
	bool bSuccess = false;

	bSuccess = m_scenarioDlgModel->UpdateScenarioListInSceTITLE(in_sceTITLE, in_sceSEQ);

	if (bSuccess)
		m_scenarioDlgModel->Commit();
	else
		m_scenarioDlgModel->Rollback();

	return bSuccess;
}

bool ScenarioListDlgService::UpdateScenarioListInNotCNT(int in_notCNT, int in_sceSEQ)
{
	bool bSuccess = false;

	bSuccess = m_scenarioDlgModel->UpdateScenarioListInNotCNT(in_notCNT, in_sceSEQ);

	if (bSuccess)
		m_scenarioDlgModel->Commit();
	else
		m_scenarioDlgModel->Rollback();

	return bSuccess;
}

bool ScenarioListDlgService::SelectInSceSEQScenarioListInSceTITLE(ComplexString in_sceTITLE)
{
	bool bSuccess = false;

	int out_sceSEQ;
	bSuccess = m_scenarioDlgModel->SelectInSceSEQScenarioListInSceTITLE(in_sceTITLE, &out_sceSEQ);

	if (bSuccess)
	{
		ScenarioListVO scenario(out_sceSEQ, 0, "");
		RequestScope->SetRequestAttributes(scenario);
	}

	return bSuccess;
}