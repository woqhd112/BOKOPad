#pragma once

#ifdef _DEBUG
#pragma comment(lib, "ComplexLibrary_dbg.lib")
#define DB_MODULE_NANE "BOKOPad_dbg.exe"
#else
#pragma comment(lib, "ComplexLibrary.lib")
#define DB_MODULE_NANE "BOKOPad.exe"
#endif

#include "ComplexString.h"
#include "ComplexFile.h"
#include "ComplexDatabase.h"
#include "ComplexMemory.h"
#include "Model/QueryDefine.h"
#include <memory>
#include <windows.h>
#include <stdio.h>

using namespace ComplexLibrary;

enum QueryStatus
{
	QS_NOTHING = 0,
	QS_EMPTY,
	QS_CLEAR,
	QS_QUERY,
};

class BOKODBInit
{
public:

	BOKODBInit();
	~BOKODBInit();

	bool StartDBConnect();
	void RunningConsole();

private:

	bool m_bRunningConsoleStatus;
	QueryStatus m_eQueryStatus;

	void ExecuteQuery(ComplexString query, sqlite3_callback query_callback);
	void ValidateQuery(ComplexString query);
};

ComplexDatabase* GetDBInstance();
#define DB_INSTANCE (GetDBInstance())
