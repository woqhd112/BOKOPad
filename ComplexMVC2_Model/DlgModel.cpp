#include "pch.h"
#include "DlgModel.h"
#include <windows.h>

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
	//g_dataKernel = ComplexSingleton<DBDataKernel>::GetInstance();
	//g_dbConnection = ComplexSingleton<ComplexDatabase>::GetInstance();
	if (g_dbConnection.IsConnect() == false)
	{
		LoadDatabase();
	}
}

DlgModel::~DlgModel()
{
	//ComplexSingleton<ComplexDatabase>::DestroyInstance();
	//ComplexSingleton<DBDataKernel>::DestroyInstance();
}


void DlgModel::LoadDatabase()
{
	bool bExist = false;

	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);
	
	ComplexString fullPath, fileName;
	ComplexString folderName = dir_;

	folderName.Remove(DB_MODULE_NANE);
	folderName.AppendFormat("%s", "Config\\");
	::CreateDirectoryA(folderName, NULL);

	fileName = "md.db";
	fullPath = folderName + fileName;

	// 弃歹积己 贸府...
	ComplexFile file;
	if(file.Exist(fullPath) == true)
	{
		bExist = true;
	}

	if (g_dbConnection.ConnectDatabase("../Workspace/Config/md.db"))
	{
		if (!bExist)
		{
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_SCENARIO_LIST_TABLE], NULL);
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_NOTE_INFORMATION_TABLE], NULL);
			g_dbConnection.ExecuteQuery(DefinedDDLQuerys[CREATE_PAD_OPTION_TABLE], NULL);
			g_dbConnection.PrepareStatement_Execute(DefinedDMLQuerys[INSERT_PAD_OPTION_TABLE], PrePareStatementInsertPadOption);
		}
		g_dbConnection.SetAutoCommit(false);
	}
}

void DlgModel::Commit()
{
	g_dbConnection.Commit();
}

void DlgModel::Rollback()
{
	g_dbConnection.Rollback();
}
