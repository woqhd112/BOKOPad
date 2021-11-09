#pragma once
#include "Control/DlgController.h"
#include "TransactionNameDefine.h"
#include "Model/VO/PadOptionVO.h"
#include "Model/VO/ScenarioListVO.h"
#include "Model/VO/NoteInformationVO.h"
#include "Model/VO/TimelineVO.h"

#ifdef _DEBUG
#pragma comment(lib, "ComplexMVC2_Model_dbg.lib")
#else
#pragma comment(lib, "ComplexMVC2_Model.lib")
#endif

class DBManagerManagement
{
public:

	DBManagerManagement();
	~DBManagerManagement();

	void StartTransaction(const char* transactionName);
	void RollbackTransaction();
	void CommitTransaction();

private:

	bool m_bOperateTransaction;
	char* m_saveTransactionName;

protected:

	static DlgController* MVC_Controller;

	ScenarioListVO m_inputScenarioVO;
	NoteInformationVO m_inputNoteVO;
	TimelineVO m_inputTimeVO;

};

