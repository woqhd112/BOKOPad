#include "pch.h"
#include "MainDlgService.h"

MainDlgService::MainDlgService()
	: m_mainDlgModel(new MainDlgModelImp)
{

}

MainDlgService::~MainDlgService()
{
	if (m_mainDlgModel)
	{
		delete m_mainDlgModel;
		m_mainDlgModel = nullptr;
	}
}

bool MainDlgService::SelectAllPadOption()
{
	bool bSuccess = false;
	
	PadOptionVO padOption;
	bSuccess = m_mainDlgModel->SelectAllPadOption(&padOption);

	if (bSuccess)
		RequestScope->SetRequestAttributes(padOption);

	return bSuccess;
}

bool MainDlgService::UpdatePadOption(int in_option1, int in_option2)
{
	bool bSuccess = false;
	bSuccess = m_mainDlgModel->UpdatePadOption(in_option1, in_option2);

	if (bSuccess)
		m_mainDlgModel->Commit();
	else
		m_mainDlgModel->Rollback();

	return bSuccess;
}

bool MainDlgService::UpdateScenarioListAutoIncrementSeq()
{
	bool bSuccess = false;
	bSuccess = m_mainDlgModel->UpdateScenarioListAutoIncrementSeq();

	if (bSuccess)
		m_mainDlgModel->Commit();
	else
		m_mainDlgModel->Rollback();

	return bSuccess;
}

bool MainDlgService::UpdateNoteInformationAutoIncrementSeq()
{
	bool bSuccess = false;
	bSuccess = m_mainDlgModel->UpdateNoteInformationAutoIncrementSeq();

	if (bSuccess)
		m_mainDlgModel->Commit();
	else
		m_mainDlgModel->Rollback();

	return bSuccess;
}