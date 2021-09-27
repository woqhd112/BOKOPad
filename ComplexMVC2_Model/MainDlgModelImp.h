#pragma once
#include "DlgModel.h"

void ResultSetSelectAllPadOption();
void PrepareStatementUpdatePadOption();

class MainDlgModelImp : public DlgModel
{
public:

	MainDlgModelImp();
	virtual ~MainDlgModelImp();

	bool SelectAllPadOption(PadOptionVO* out_optionVO);
	bool UpdatePadOption(int in_option1, int in_option2);
	bool UpdateScenarioListAutoIncrementSeq();

private:

};

