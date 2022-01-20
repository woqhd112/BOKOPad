#include "BOKODBInit.h"


static std::shared_ptr<ComplexDatabase> g_dbConnection;
static bool g_callback_result_column_display;
static bool g_callback_result_lastRecord_display;

static int QueryCallbackFunction(void* notUse, int argc, char**argv, char**azColName)
{
	if (g_callback_result_column_display)
	{
		printf("---------------------------------------------------------------\n");
		for (int i = 0; i < argc; i++)
		{
			printf("%s\t\t", azColName[i]);
		}
		printf("\n---------------------------------------------------------------\n");
	}

	for (int i = 0; i < argc; i++)
	{
		printf("%s\t\t", argv[i]);
	}
	printf("\n");

	g_callback_result_column_display = false;
	g_callback_result_lastRecord_display = true;

	return 0;
}

ComplexDatabase* GetDBInstance()
{
	if (g_dbConnection.get() == NULL)
	{
		g_dbConnection.reset(new ComplexDatabase);
	}
	return g_dbConnection.get();
}

BOKODBInit::BOKODBInit()
	: m_bRunningConsoleStatus(false)
	, m_eQueryStatus(QS_NOTHING)
{
	
}

BOKODBInit::~BOKODBInit()
{
	if (DB_INSTANCE->DisConnectDatabase())
	{
		printf("SQLITE3 disconnect success.\n");
	}
}

void BOKODBInit::ExecuteQuery(ComplexString query, sqlite3_callback query_callback)
{
	bool bSuccess = DB_INSTANCE->ExecuteQuery(query, query_callback);
	if (bSuccess)
	{
		DB_INSTANCE->Commit();
		if (g_callback_result_lastRecord_display)
			printf("\n---------------------------------------------------------------\n");

		printf("Execute Query Success.\n");
		printf("Success Query : %s\n", query.GetBuffer());
	}
	else
	{
		DB_INSTANCE->Rollback();
		printf("Execute Query Failed.\n");
		printf("Failed Query : %s\n", query.GetBuffer());
	}
}

bool BOKODBInit::StartDBConnect()
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
	if (file.Exist(fullPath) == true)
	{
		bExist = true;
	}

	if (DB_INSTANCE->ConnectDatabase("../Workspace/Config/md.db"))
	{
		printf("SQLITE3 connect success.\n");
		ExecuteQuery(DefinedDDLQuerys[DELETE_CASCADE_ON], NULL);
		if (!bExist)
		{
			ExecuteQuery(DefinedDDLQuerys[CREATE_SCENARIO_LIST_TABLE], NULL);
			ExecuteQuery(DefinedDDLQuerys[CREATE_NOTE_INFORMATION_TABLE], NULL);
			ExecuteQuery(DefinedDDLQuerys[CREATE_TIME_LINE_TABLE], NULL);
		}

		m_bRunningConsoleStatus = true;
		return true;
	}

	return false;
}

void BOKODBInit::RunningConsole()
{
	while (m_bRunningConsoleStatus)
	{
		char strInputQuery[1024];
		MemoryInitialize(strInputQuery, 1024);
		g_callback_result_column_display = true;
		g_callback_result_lastRecord_display = false;
		m_eQueryStatus = QS_NOTHING;

		printf("[Test Console] : ");
		scanf_s("%[^\n]s", strInputQuery, 1024);
		getchar();

		ValidateQuery(strInputQuery);

		if (m_eQueryStatus == QS_EMPTY)
			continue;
		else if (m_eQueryStatus == QS_CLEAR)
		{
			system("cls");
			continue;
		}

		ExecuteQuery(strInputQuery, QueryCallbackFunction);
	}
}

void BOKODBInit::ValidateQuery(ComplexString query)
{
	ComplexString strQueryValidate = query;
	try
	{
		strQueryValidate.SetAllLower();
	}
	catch (ComplexUnIdentifiableException e)
	{
		// .. nothing
	}

	if (query == "")
	{
		m_eQueryStatus = QS_EMPTY;
	}
	else if (query == "clear")
	{
		m_eQueryStatus = QS_CLEAR;
	}
	else
	{
		m_eQueryStatus = QS_QUERY;
	}
}
