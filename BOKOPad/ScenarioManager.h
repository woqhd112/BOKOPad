#pragma once
#include "ComplexMap.h"
#include "BOKOScenarioDetailDlg.h"

using namespace ComplexLibrary;

enum SendMessageProcess
{
	SMP_CREATE = 0,
	SMP_DESTROY,
	SMP_SHOW,
	SMP_HIDE,
	SWP_EXIST
};


class ScenarioManager
{
public:

	ScenarioManager();
	~ScenarioManager();

	void AttachManager(CWnd* dlgAttachPointer);
	CWnd* DetachManager();

	bool SendMessages(SendMessageProcess bindMessage, int scenarioIndex = -1);

private:

	bool HelpInvoker(SendMessageProcess bindMessage, int scenarioIndex = -1);

	bool Create(int createScenario);
	bool Destroy(int destroyScenario);
	bool Show(int showScenario);
	bool Hide(int hideScenario);
	bool Exist(int existScenario);

private:

	bool m_bAttach;

	CWnd* m_mainDlg;
	ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;

};

