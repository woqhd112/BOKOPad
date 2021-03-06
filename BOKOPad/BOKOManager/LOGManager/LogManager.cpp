#include "pch.h"
#include "LogManager.h"
#include "..\..\BOKOProgressPopup.h"
#include "..\..\BOKOPad.h"

LogManager::LogManager()
	: m_status(LPS_NONE)
{
	ComplexUtilProcess::INITIALIZE_LOG();
}

LogManager::~LogManager()
{
	ComplexUtilProcess::TERMINATE_LOG();
}

bool LogManager::OnPutLog(ComplexString logMsg, LogType type)
{
	bool bLogSuccess = false; 
#ifdef ADMIN_CONFIRM_VIEW
	if (type == LT_EVENT)
	{
		bLogSuccess = ComplexUtilProcess::LOG_E(logMsg);
	}
	else if (type == LT_PROCESS)
	{
		bLogSuccess = ComplexUtilProcess::LOG_P(logMsg);
	}
	else if (type == LT_OPERATE)
	{
		bLogSuccess = ComplexUtilProcess::LOG_O(logMsg);
	}
#endif
	return bLogSuccess;
}


bool LogManager::OnLoadLogData()
{
	Clear();
	char dir_[2048] = { 0 };
	::GetModuleFileNameA(NULL, dir_, 2048);

	ComplexString fullPath, fileName;
	ComplexString folderName = dir_;

	folderName.Remove(DB_MODULE_NANE);
	folderName.AppendFormat("%s", "LOG\\");
	fullPath = folderName;
	fullPath.AppendFormat("%s", "*.*");

	fullPath.ReplaceAll("\\", "/");

	CFileFind finder, countFinder;
	
	BOOL test = countFinder.FindFile(fullPath.GetBuffer());
	int fileCount = 0;
	while (test)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		test = countFinder.FindNextFile();
		// folder 일 경우는 continue 
		if (countFinder.IsDirectory())
			continue;
		else if (countFinder.IsDots())
			continue;

		fileCount++;
	}
	BOKOProgressPopup progress(&m_bProcessing, fileCount);
	progress.Create(BOKOProgressPopup::IDD);
	progress.ShowWindow(SW_SHOW);
	CPaintDC dc(&progress);
	progress.DrawFrame(&dc);

	BOOL bWorking = finder.FindFile(fullPath.GetBuffer());
	
	while (bWorking && m_bProcessing)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		// folder 일 경우는 continue 
		if (finder.IsDirectory())
			continue;
		else if (finder.IsDots())
			continue;
		// 파일 일때 //파일의 이름 
		fileName = finder.GetFileName().GetBuffer();

		progress.SetAnalyzeFileName(fileName);
		if (progress.AddProgressPos())
			progress.Success();

		ComplexVector<ComplexString> logDataContainer;
		if (AnalysisLogFile(folderName + fileName, &logDataContainer))
		{
			m_logMap.insert(finder.GetFileTitle().GetBuffer(), logDataContainer);
		}
		else
			return false;
	}

	// 중간에 x버튼 눌러 멈췄을 경우는 전부 종료해야함
	if (m_bProcessing == false)
	{
		m_status = LPS_FAIL;
		progress.SendMessageA(WM_CLOSE);
		return false;
	}

	m_status = LPS_SUCCESS;
	progress.SendMessageA(WM_CLOSE);

	return true;
}

bool LogManager::IsProcessing() const
{
	return m_bProcessing;
}

LogProcessStatus LogManager::GetProcessStatus() const
{
	return m_status;
}

bool LogManager::AnalysisLogFile(ComplexString analizeLogFilePath, ComplexVector<ComplexString>* out)
{
	ComplexFile file;
	ComplexString logFileBuf;
	if (file.Read(analizeLogFilePath, logFileBuf) == false)
		return false;

	if (!logFileBuf.IsEmpty()) 
	{
		m_stringTokens.Clear();

		logFileBuf.RemoveAll("\n");
		m_stringTokens.ApplyStringTokenize(logFileBuf, ',');

		while (m_stringTokens.HasNextToken() && m_bProcessing)
		{
			ComplexString token = m_stringTokens.NextToken();
			out->push_back(token);
		}
	}

	return true;
}

ComplexMap<ComplexString, ComplexVector<ComplexString>>* LogManager::GetLogData()
{
	return &m_logMap;
}

ComplexVector<ComplexString>* LogManager::GetLogDataElement(ComplexString logFileName)
{
	ComplexMap<ComplexString, ComplexVector<ComplexString>>::iterator iter = m_logMap.find(logFileName);
	if (iter == m_logMap.end())
		return nullptr;

	return &iter->value.value;
}


void LogManager::Clear()
{
	m_logMap.clear();
}
