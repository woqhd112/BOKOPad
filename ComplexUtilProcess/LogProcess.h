#pragma once
#include "ComplexThread.h"
#include "ComplexCondition.h"
#include "ComplexLock.h"
#include "ComplexLog.h"
#include "ComplexString.h"
#include "ComplexUniqueQueue.h"
#include "ComplexTime.h"

#ifdef COMPLEXUTILPROCESS_EXPORTS
#define COMPLEXUTILPROCESS_DLL _declspec(dllexport)
#else
#define COMPLEXUTILPROCESS_DLL _declspec(dllimport)
#endif

using namespace ComplexLibrary;

class LogProcess : public ComplexThread
{
public:

	LogProcess();
	~LogProcess();

	void Init();
	bool EventLog(ComplexString eventText);
	bool OperateLog(ComplexString operateText);
	bool ProcessLog(ComplexString processText);
	void CreateDir();

private:

	ComplexString GetCurrentTimeText();
	void WriteLog();

private:

	bool m_bInit;
	ComplexString m_strWritePath;

	ComplexCondition m_cond;

	ComplexLog* m_log;

	ComplexUniqueSwapQueue<ComplexString> m_logQueue;

protected:

	virtual void Run();

};

LogProcess* GetLogProcess();
#define LogProc (GetLogProcess())

#ifdef __cplusplus
extern "C"
{
#endif

	namespace ComplexUtilProcess
	{
		COMPLEXUTILPROCESS_DLL void INIT_LOG();
		COMPLEXUTILPROCESS_DLL bool LOG_E(ComplexString log);
		COMPLEXUTILPROCESS_DLL bool LOG_O(ComplexString log);
		COMPLEXUTILPROCESS_DLL bool LOG_P(ComplexString log);
	}

#ifdef __cplusplus
}
#endif