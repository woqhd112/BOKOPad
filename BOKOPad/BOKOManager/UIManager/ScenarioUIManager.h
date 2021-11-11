#pragma once
#include "UIManagerManagement.h"


class ScenarioUIManager : public UIManagerManagement
{
public:

	ScenarioUIManager();
	virtual ~ScenarioUIManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool InvokeHelper(PerformanceMessage message);

	bool Create();
	bool Destroy();
	bool Show();
	bool Hide();
	bool Exist();
	bool Clear();

	bool NoteReload();
	bool TimeLineReload();
	bool IsDraggingMode();

	bool DragProcessLock();
	bool ThisOneViewRefresh();
	bool TargetOneViewRefresh();

	bool TimelineOneViewExpandAllClose();

private:

	/*ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
	ComplexMap<int, int> m_scenarioSeqMap;*/

};

