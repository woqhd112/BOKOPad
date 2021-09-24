#include "pch.h"
#include "DlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ComplexDatabase g_dbConnection;
static DBDataKernel g_dataKernel;

DBDataKernel* GetDataKernelInstance()
{
	return &g_dataKernel;
}

ComplexDatabase* GetDBInstance()
{
	return &g_dbConnection;
}

void PrePareStatementInsertPadOption()
{
	DB_INSTANCE->SetInt(1, 2);
	DB_INSTANCE->SetInt(2, 3);
}

DlgModel::DlgModel()
{
	g_dataKernel = ComplexSingleton<DBDataKernel>::GetInstance();
	g_dbConnection = ComplexSingleton<ComplexDatabase>::GetInstance();
	if (g_dbConnection.IsConnect() == false)
	{
		LoadDatabase();
	}
}

DlgModel::~DlgModel()
{
	ComplexSingleton<ComplexDatabase>::DestroyInstance();
	ComplexSingleton<DBDataKernel>::DestroyInstance();
}

void DlgModel::LoadDatabase()
{
	bool bExist = false;
	ComplexFile file;
	if(file.Exist("Config/md.db") == true)
	{
		bExist = true;
	}

	if (g_dbConnection.ConnectDatabase("Config/md.db"))
	{
		g_dbConnection.SetAutoCommit(false);
		if (!bExist)
		{
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_SCENARIO_LIST_TABLE], NULL);
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_NOTE_INFORMATION_TABLE], NULL);
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_PAD_OPTION_TABLE], NULL);
			g_dbConnection.PrepareStatement_Execute(DefinedDMLQuerys[INSERT_PAD_OPTION_TABLE], PrePareStatementInsertPadOption);
			g_dbConnection.Commit();
		}
	}
}

void DlgModel::Commit()
{

}

void DlgModel::Rollback()
{

}
