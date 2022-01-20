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
		// ���μ��� ����
		COMPLEXUTILPROCESS_DLL DWORD ExecuteProcess(ComplexString processName, bool isHide = false);
		// ���μ��� ã��
		COMPLEXUTILPROCESS_DLL DWORD FindProcess(ComplexString processName);
		// ���μ��� ����
		COMPLEXUTILPROCESS_DLL bool TerminateProcess(DWORD processID);
		// �ش� �̸��� ���� ��� ���μ��� ����
		COMPLEXUTILPROCESS_DLL bool TerminateProcesses(ComplexString processName);
		// ���μ��� ���̵� ã��
		COMPLEXUTILPROCESS_DLL DWORD GetProcessID(ComplexString processName);
	}

#ifdef __cplusplus
}
#endif