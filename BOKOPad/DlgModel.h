#pragma once
#include "QueryDefine.h"
#include "DataScope.h"

void PrePareStatementInsertPadOption();
void CreateConfigFile(CString& strFullPath);
void GetModulePath(CString& strPath);
void CreateDefaultDirectory(CString& strFullPath, CString strAppendPath);

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