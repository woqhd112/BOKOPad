#pragma once
#include "ManagerManagement.h"


class ScenarioManager : public ManagerManagement
{
public:

	ScenarioManager();
	virtual ~ScenarioManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool HelpInvoker(PerformanceMessage message);
	bool BringScenarioStruct();
	void ReleaseScenarioStruct();

	bool Create();
	bool Destroy();
	bool Show();
	bool Hide();
	bool Exist();

	bool DragMove();
	bool DragDown();
	bool DragUp();
	bool TimeLineAttach();
	bool TimeLineDetach();
	bool TimeLineContactGridline();
	bool TimeLineNotContactGridline();
	bool DlgAttach();

private:

	ScenarioManagerStruct* m_GetScenarioStruct;
	ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
};

