#include "pch.h"
#include "DlgModel.h"
#include <windows.h>

static std::shared_ptr<ComplexDatabase> g_dbConnection;
static std::shared_ptr<DBDataKernel> g_dataKernel;

DBDataKernel* GetDataKernelInstance()
{
	if (g_dataKernel.get() == NULL)
	{
		g_dataKernel.reset(new DBDataKernel);
	}
	return g_dataKernel.get();
}

ComplexDatabase* GetDBInstance()
{
	if (g_dbConnection.get() == NULL)
	{
		g_dbConnection.reset(new ComplexDatabase);
	}
	return g_dbConnection.get();
}

DlgModel::DlgModel()
{
	//g_dataKernel = ComplexSingleton<DBDataKernel>::GetInstance();
	//g_dbConnection = ComplexSingleton<ComplexDatabase>::GetInstance();
	if (DB_INSTANCE->IsConnect() == false)
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

	// 폴더생성 처리...
	ComplexFile file;
	if(file.Exist(fullPath) == true)
	{
		bExist = true;
	}

	if (DB_INSTANCE->ConnectDatabase("../Workspace/Config/md.db"))
	{
		DB_INSTANCE->ExecuteQuery(DefinedDDLQuerys[DELETE_CASCADE_ON], NULL);
		if (!bExist)
		{
			DB_INSTANCE->ExecuteQuery(DefinedDDLQuerys[CREATE_SCENARIO_LIST_TABLE], NULL);
			DB_INSTANCE->ExecuteQuery(DefinedDDLQuerys[CREATE_NOTE_INFORMATION_TABLE], NULL);
			DB_INSTANCE->ExecuteQuery(DefinedDDLQuerys[CREATE_TIME_LINE_TABLE], NULL);
		}
		// 트랜잭션 처리를 위해 주석
		//g_dbConnection.SetAutoCommit(false);
	}
}

void DlgModel::Commit()
{
	DB_INSTANCE->Commit();
}

void DlgModel::Rollback()
{
	DB_INSTANCE->Rollback();
}
