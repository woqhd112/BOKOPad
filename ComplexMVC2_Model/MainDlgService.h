#pragma once

#include "MainDlgModelImp.h"

class MainDlgService
{
public:

	MainDlgService();
	~MainDlgService();

	bool SelectAllPadOption();
	bool UpdatePadOption(int in_option1, int in_option2);
	bool UpdateScenarioListAutoIncrementSeq();
	bool UpdateNoteInformationAutoIncrementSeq();

private:

	MainDlgModelImp* m_mainDlgModel;

};

