#pragma once
#include "Log/LogProcess.h"
#include "File/FileProcess.h"
#include "ComplexMap.h"
#include "ComplexStringTokenizer.h"

#ifdef _DEBUG
#pragma comment(lib, "ComplexUtilProcess_dbg.lib")
#else
#pragma comment(lib, "ComplexUtilProcess.lib")
#endif

enum LogType
{
	LT_EVENT = 0,
	LT_PROCESS,
	LT_OPERATE
};

enum LogProcessStatus
{
	LPS_NONE = 0,
	LPS_SUCCESS,
	LPS_FAIL
};

class LogManager
{
public:

	LogManager();
	~LogManager();

	bool OnPutLog(ComplexString logMsg, LogType type);

	bool OnLoadLogData();
	ComplexMap<ComplexString, ComplexVector<ComplexString>>* GetLogData();
	ComplexVector<ComplexString>* GetLogDataElement(ComplexString logFileName);

	void Clear();

	bool IsProcessing() const;
	LogProcessStatus GetProcessStatus() const;

private:

	bool AnalysisLogFile(ComplexString analizeLogFilePath, ComplexVector<ComplexString>* out);

	// key : �α� ���ϸ�
	// value : �α׳��� ','�� �������� �ϳ��� ������Ʈ �߰���
	ComplexMap<ComplexString, ComplexVector<ComplexString>> m_logMap;

	ComplexStringTokenizer m_stringTokens;

	bool m_bProcessing;

	LogProcessStatus m_status;
};

