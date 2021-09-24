#include "pch.h"
#include "MainDlgService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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