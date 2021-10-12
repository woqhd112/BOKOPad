
// BOKOPad.h: PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 주 기호입니다.
#ifdef _DEBUG
//#pragma comment(lib, "ComplexMVC2_Model_dbg.lib")
#pragma comment(lib, "ComplexLibrary_dbg.lib")
#pragma comment(lib, "ComplexUtilProcess_dbg.lib")
#else
//#pragma comment(lib, "ComplexMVC2_Model.lib")
#pragma comment(lib, "ComplexLibrary.lib")
#pragma comment(lib, "ComplexUtilProcess.lib")
#endif
#include "ScenarioDBManager.h"
#include "ScenarioUIManager.h"

// CBOKOPadApp:
// 이 클래스의 구현에 대해서는 BOKOPad.cpp을(를) 참조하세요.
//

static ScenarioDBManager* Scenario_DB_Manager = ComplexSingleton<ScenarioDBManager>::GetInstancePtr();
static ScenarioUIManager* Scenario_UI_Manager = ComplexSingleton<ScenarioUIManager>::GetInstancePtr();

class CBOKOPadApp : public CWinApp
{
public:
	CBOKOPadApp();
	~CBOKOPadApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CBOKOPadApp theApp;
