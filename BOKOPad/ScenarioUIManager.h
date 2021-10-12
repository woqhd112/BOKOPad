#pragma once
#include "UIManagerManagement.h"


class ScenarioUIManager : public UIManagerManagement
{
public:

	ScenarioUIManager();
	virtual ~ScenarioUIManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool HelpInvoker(PerformanceMessage message);

	bool Create();
	bool Destroy();
	bool Show();
	bool Hide();
	bool Exist();
	bool Clear();

	bool NoteReload();
	bool IsDraggingMode();

	bool DragProcessLock();

private:

	/*ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
	ComplexMap<int, int> m_scenarioSeqMap;*/

};

