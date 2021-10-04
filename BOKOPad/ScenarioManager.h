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

	bool Create();
	bool Destroy();
	bool Show();
	bool Hide();
	bool Exist();

	bool TimeLineContactGridline();
	bool TimeLineNotContactGridline();

private:

	/*ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
	ComplexMap<int, int> m_scenarioSeqMap;*/

};

