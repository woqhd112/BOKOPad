#pragma once
#include "QueryDefine.h"
#include "DataScope.h"
#include <memory>

#ifdef _DEBUG 
#define DB_MODULE_NANE "BOKOPad_dbg.exe"
#else
#define DB_MODULE_NANE "BOKOPad.exe"
#endif

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