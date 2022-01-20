#pragma once
#include "../commonInclude.h"
#include "ComplexVector.h"
#include <Windows.h>
#include <bcrypt.h>
#include <Psapi.h> //for EnumProcesses, EnumProcessModules, GetModuleBaseName
#pragma comment(lib, "psapi")

#ifdef COMPLEXUTILPROCESS_EXPORTS
#define COMPLEXUTILPROCESS_DLL _declspec(dllexport)
#else
#define COMPLEXUTILPROCESS_DLL _declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	namespace ComplexUtilProcess
	{
		// 프로세스 실행
		COMPLEXUTILPROCESS_DLL DWORD ExecuteProcess(ComplexString processName, bool isHide = false);
		// 프로세스 찾기
		COMPLEXUTILPROCESS_DLL DWORD FindProcess(ComplexString processName);
		// 프로세스 종료
		COMPLEXUTILPROCESS_DLL bool TerminateProcess(DWORD processID);
		// 해당 이름을 갖는 모든 프로세스 종료
		COMPLEXUTILPROCESS_DLL bool TerminateProcesses(ComplexString processName);
		// 프로세스 아이디 찾기
		COMPLEXUTILPROCESS_DLL DWORD GetProcessID(ComplexString processName);
	}

#ifdef __cplusplus
}
#endif