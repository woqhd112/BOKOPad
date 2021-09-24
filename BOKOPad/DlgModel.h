#pragma once
#include "QueryDefine.h"
#include "DataScope.h"

void PrePareStatementInsertPadOption();

class DlgModel
{
public:

	DlgModel();
	~DlgModel();

	void LoadDatabase();
	void Commit();
	void Rollback();

private:

};

DBDataKernel* GetDataKernelInstance();
ComplexDatabase* GetDBInstance();
#define DB_INSTANCE (GetDBInstance())
#define DATA_INSTANCE (GetDataKernelInstance())