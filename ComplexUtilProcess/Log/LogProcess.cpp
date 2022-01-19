#include "pch.h"
#include "LogProcess.h"


LogProcess::LogProcess()
	: m_log(new ComplexLog)
	, m_bInit(false)
{

}

LogProcess::~LogProcess()
{
	Terminate();
}


void LogProcess::Init()
{
	m_bInit = true;

	CreateDir();

	this->Start();
}

void LogProcess::Terminate()
{
	m_bInit = false;

	this->Stop();
	this->Join();

	WriteLog();

	if (m_log)
	{
		delete m_log;
		m_log = nullptr;
	}
}

void LogProcess::CreateDir()
{
	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);

	ComplexString fullPath, fileName;
	ComplexString folderName = dir_;

	folderName.Remove(DB_MODULE_NANE);
	folderName.AppendFormat("%s", "LOG\\");
	::CreateDirectoryA(folderName, NULL);

	ComplexTimeTable tt = ComplexTime::GetCurrentTimes();
	m_strWritePath.Format("%s%04d_%02d_%02d_%02d.txt", folderName.GetBuffer(), tt.year, tt.month, tt.day, tt.hour);
}

ComplexString LogProcess::GetCurrentTimeText()
{
	ComplexTimeTable tt = ComplexTime::GetCurrentTimes();
	ComplexString strFormat;
	
	strFormat.Format("%02d:%02d:%02d", tt.hour, tt.minute, tt.second);
	return strFormat;
}

bool LogProcess::EventLog(ComplexString eventText)
{
	if (!m_bInit)
		return false;

	ComplexString strFormat;
	strFormat.Format("[Event][%s] : %s,", GetCurrentTimeText().GetBuffer(), eventText.GetBuffer());

	m_logQueue.lock();
	m_logQueue.push(strFormat);
	m_logQueue.unlock();

	return true;
}

bool LogProcess::OperateLog(ComplexString operateText)
{
	if (!m_bInit)
		return false;

	ComplexString strFormat;
	strFormat.Format("[Operate][%s] : %s,", GetCurrentTimeText().GetBuffer(), operateText.GetBuffer());

	m_logQueue.lock();
	m_logQueue.push(strFormat);
	m_logQueue.unlock();

	return true;
}

bool LogProcess::ProcessLog(ComplexString processText)
{
	if (!m_bInit)
		return false;

	ComplexString strFormat;
	strFormat.Format("[Process][%s] : %s,", GetCurrentTimeText().GetBuffer(), processText.GetBuffer());

	m_logQueue.lock();
	m_logQueue.push(strFormat);
	m_logQueue.unlock();

	return true;
}

void LogProcess::WriteLog()
{
	m_logQueue.swapQueue();

	ComplexString data;
	while (m_logQueue.emptyOutputQueue() == false)
	{
		data = m_logQueue.frontOutputQueue();
		m_logQueue.popOutputQueue();
		m_log->FileLog(m_strWritePath, "%s\n", data.GetBuffer());
		data.Replace(",", "\n");
		m_log->PrintLog("%s", data.GetBuffer());
		OutputDebugStringA(data.GetBuffer());
	}
}

void LogProcess::Run()
{
	bool isEmpty;
	while (this->IsRunning())
	{
		m_logQueue.lock();
		isEmpty = m_logQueue.empty();
		m_logQueue.unlock();

		if (isEmpty)
		{
			ComplexThread::Sleep(200);
			continue;
		}

		WriteLog();

		ComplexThread::Sleep(200);
	}
}

static LogProcess g_log;

LogProcess* GetLogProcess()
{
	return &g_log;
}

namespace ComplexUtilProcess
{
	COMPLEXUTILPROCESS_DLL void INITIALIZE_LOG()
	{
		LogProc->Init();
	}

	COMPLEXUTILPROCESS_DLL void TERMINATE_LOG()
	{
		LogProc->Terminate();
	}

	COMPLEXUTILPROCESS_DLL bool LOG_E(ComplexString log)
	{
		LogProc->CreateDir();
		return LogProc->EventLog(log);
	}

	COMPLEXUTILPROCESS_DLL bool LOG_O(ComplexString log)
	{
		LogProc->CreateDir();
		return LogProc->OperateLog(log);
	}

	COMPLEXUTILPROCESS_DLL bool LOG_P(ComplexString log)
	{
		LogProc->CreateDir();
		return LogProc->ProcessLog(log);
	}
}