#include "pch.h"
#include "ApplicationProcess.h"

namespace ComplexUtilProcess
{
	COMPLEXUTILPROCESS_DLL DWORD ExecuteProcess(ComplexString processName, bool isHide)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		if (isHide)
		{
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
		}

		if (!::CreateProcess(NULL, (LPSTR)processName.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Failed CreateProcess(%d)\n", ::GetLastError());
			return 0;
		}

		printf("Success CreateProcess(PID: %u)\n", pi.dwProcessId);
		return pi.dwProcessId;
	}

	COMPLEXUTILPROCESS_DLL DWORD FindProcess(ComplexString processName)
	{
		HANDLE hProcess = NULL;
		DWORD dwProcesses[1024] = {0,};
		DWORD dwNeeded = 0;
		DWORD processID = 0;

		::ZeroMemory(&dwProcesses, sizeof(DWORD) * 1024);

		if (::EnumProcesses((DWORD*)&dwProcesses, sizeof(DWORD) * 1024, &dwNeeded) == TRUE)
		{
			int nCountOfProcess = dwNeeded / sizeof(DWORD);

			for (int i = 0; i < nCountOfProcess; ++i)
			{
				processID = dwProcesses[i];
				char szProcessName[_MAX_PATH + 1] = "<unknown>";
				if ((hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID)) != NULL)
				{
					HMODULE hMod;
					if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &dwNeeded))
						::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
				}
				::CloseHandle(hProcess);

				if (processName.Compare(szProcessName) == true)
					return processID;
			}
		}

		return 0;
	}

	COMPLEXUTILPROCESS_DLL bool TerminateProcess(DWORD processID)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, processID);
		::TerminateProcess(hProcess, 0);
		::CloseHandle(hProcess);
		printf("terminate (PID: %u)\n", processID);

		return true;
	}

	COMPLEXUTILPROCESS_DLL bool TerminateProcesses(ComplexString processName)
	{
		ComplexVector<DWORD> processIdVector;
		HANDLE	hProcess = NULL;
		DWORD	dwProcesses[1024] = { 0, },
			dwNeeded = 0,
			processID = 0;
		::ZeroMemory(&dwProcesses, sizeof(DWORD) * 1024);

		if (TRUE == ::EnumProcesses((DWORD*)&dwProcesses, sizeof(DWORD) * 1024, &dwNeeded))
		{
			int nCountOfProcess = dwNeeded / sizeof(DWORD);

			for (int nIndex = 0; nIndex < nCountOfProcess; ++nIndex)
			{
				processID = dwProcesses[nIndex];
				char szProcessName[_MAX_PATH + 1] = "<unknown>";
				if (NULL != (hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID)))
				{
					HMODULE hMod;
					if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &dwNeeded))
						::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
				}
				::CloseHandle(hProcess);

				if (strcmp(szProcessName, processName) == 0)
					processIdVector.push_back(processID);
			}
		}

		if (processIdVector.empty())
			return false;

		int size = (int)processIdVector.size();
		for (int i = 0; i < size; i++)
		{
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, processIdVector[i]);
			::TerminateProcess(hProcess, 0);
			::CloseHandle(hProcess);
			printf("terminate %s  (PID: %u)\n", processName.GetBuffer(), processIdVector[i]);
		}

		return true;
	}

	COMPLEXUTILPROCESS_DLL DWORD GetProcessID(ComplexString processName)
	{
		HANDLE	hProcess = NULL;
		DWORD	dwProcesses[1024] = { 0, },
			dwNeeded = 0,
			processID = 0;
		::ZeroMemory(&dwProcesses, sizeof(DWORD) * 1024);

		if (TRUE == ::EnumProcesses((DWORD*)&dwProcesses, sizeof(DWORD) * 1024, &dwNeeded))
		{
			int nCountOfProcess = dwNeeded / sizeof(DWORD);

			for (int nIndex = 0; nIndex < nCountOfProcess; ++nIndex)
			{
				processID = dwProcesses[nIndex];
				char szProcessName[_MAX_PATH + 1] = "<unknown>";
				if (NULL != (hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID)))
				{
					HMODULE hMod;
					if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &dwNeeded))
						::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
				}
				::CloseHandle(hProcess);

				if (strcmp(szProcessName, processName.GetBuffer()) == 0)
					return processID;
			}
		}

		return 0;
	}
}