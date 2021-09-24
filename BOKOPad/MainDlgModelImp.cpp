#include "pch.h"
#include "MainDlgModelImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ResultSetSelectAllPadOption()
{
	int test1 = DB_INSTANCE->GetInt(0);
	int test2 = DB_INSTANCE->GetInt(1);
	DATA_INSTANCE->SetPadOption(test1, test2);
}

void PrepareStatementUpdatePadOption()
{
	int option1, option2;
	DATA_INSTANCE->GetPadOption(&option1, &option2);
	DB_INSTANCE->SetInt(1, option1);
	DB_INSTANCE->SetInt(2, option2);
	DATA_INSTANCE->ResetPadOption();
}

MainDlgModelImp::MainDlgModelImp()
{

}

MainDlgModelImp::~MainDlgModelImp()
{

}

bool MainDlgModelImp::SelectAllPadOption(PadOptionVO* out_optionVO)
{
	bool bSuccess = false;
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_ALL_PAD_OPTION_TABLE], NULL, ResultSetSelectAllPadOption, NULL);

	if (bSuccess)
		DATA_INSTANCE->GetPadOption(out_optionVO);

	DATA_INSTANCE->ResetPadOption();

	return bSuccess;
}

bool MainDlgModelImp::UpdatePadOption(int in_option1, int in_option2)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetPadOption(in_option1, in_option2);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_PAD_OPTION_TABLE], PrepareStatementUpdatePadOption);

	DATA_INSTANCE->ResetPadOption();

	return bSuccess;
}
