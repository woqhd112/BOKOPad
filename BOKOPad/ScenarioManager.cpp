#include "pch.h"
#include "ScenarioManager.h"
#include "BOKOScenarioDetailDlg.h"

ScenarioManager::ScenarioManager()
{
	m_pStageManager->InitStage();
}

ScenarioManager::~ScenarioManager()
{
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.begin();

	while (iter != m_scenarioDlgManager.end())
	{
		BOKOScenarioDetailDlg* dlg = iter->value.value;
		dlg->DestroyWindow();
		delete dlg;
		dlg = nullptr;

		iter++;
	}
	m_scenarioDlgManager.clear();
}

bool ScenarioManager::BringScenarioStruct()
{
	if (m_pPutScenarioStruct == nullptr)
		return false;

	m_GetScenarioStruct = m_pPutScenarioStruct;
	return true;
}

void ScenarioManager::ReleaseScenarioStruct()
{
	m_pPutScenarioStruct = nullptr;
	m_GetScenarioStruct = nullptr;
}

bool ScenarioManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(message);
}

bool ScenarioManager::HelpInvoker(PerformanceMessage message)
{
	bool bHelpSuccess = false;

	if (message == PM_CREATE)
	{
		if (BringScenarioStruct())
		{
			bHelpSuccess = Create();
		}
	}
	else if (message == PM_DESTROY)
	{
		if (BringScenarioStruct())
		{
			bHelpSuccess = Destroy();
		}
	}
	else if (message == PM_SHOW)
	{
		if (BringScenarioStruct())
		{
			bHelpSuccess = Show();
		}
	}
	else if (message == PM_HIDE)
	{
		if (BringScenarioStruct())
		{
			bHelpSuccess = Hide();
		}
	}
	else if (message == PM_EXIST)
	{
		if (BringScenarioStruct())
		{
			bHelpSuccess = Exist();
		}
	}
	else if (message == PM_DRAG_MOVE)
	{
		bHelpSuccess = DragMove();
	}
	else if (message == PM_DRAG_DOWN)
	{
		bHelpSuccess = DragDown();
	}
	else if (message == PM_DRAG_UP)
	{
		bHelpSuccess = DragUp();
	}
	else if (message == PM_TIMELINE_ATTACH)
	{
		bHelpSuccess = TimeLineAttach();
	}
	else if (message == PM_TIMELINE_DETACH)
	{
		bHelpSuccess = TimeLineDetach();
	}
	else if (message == PM_TIMELINE_CONTACT_GRIDLINE)
	{
		bHelpSuccess = TimeLineContactGridline();
	}
	else if (message == PM_TIMELINE_NOT_CONTACT_GRIDLINE)
	{
		bHelpSuccess = TimeLineNotContactGridline();
	}
	else if (message == PM_DLG_ATTACH)
	{
		bHelpSuccess = DlgAttach();
	}


	return bHelpSuccess;
}

bool ScenarioManager::Create()
{
	if (m_GetScenarioStruct->scenarioIndex < 0)
		return false;

	ScenarioManagerStruct inputScenarioStruct;
	inputScenarioStruct.scenarioData = m_GetScenarioStruct->scenarioData;
	inputScenarioStruct.scenarioIndex = m_GetScenarioStruct->scenarioIndex;

	BOKOScenarioDetailDlg* scenarioDetail = new BOKOScenarioDetailDlg(inputScenarioStruct, m_mainDlg);
	bool bCreate = (bool)scenarioDetail->Create(scenarioDetail->IDD, m_mainDlg);
	if (bCreate)
	{
		try
		{
			m_scenarioDlgManager.insert(inputScenarioStruct.scenarioIndex, scenarioDetail);
		}
		catch (ComplexDuplicateException e)
		{
			// 키 중복은 삭제하고 리턴
			scenarioDetail->DestroyWindow();
			delete scenarioDetail;
			scenarioDetail = nullptr;
			ReleaseScenarioStruct();
			return false;
		}

		scenarioDetail->ShowWindow(SW_SHOW);
	}
	else
	{
		delete scenarioDetail;
		scenarioDetail = nullptr;
	}

	ReleaseScenarioStruct();

	return bCreate;
}

bool ScenarioManager::Destroy()
{
	if (m_GetScenarioStruct->scenarioIndex < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(m_GetScenarioStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
		return false;


	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	m_scenarioDlgManager.erase(iter->value.key);
	findScenarioDlg->DestroyWindow();
	delete findScenarioDlg;
	findScenarioDlg = nullptr;

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioManager::Show()
{
	if (m_GetScenarioStruct->scenarioIndex < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(m_GetScenarioStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
		return false;

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_SHOW);

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioManager::Hide()
{
	if (m_GetScenarioStruct->scenarioIndex < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(m_GetScenarioStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
		return false;

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_HIDE);

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioManager::Exist()
{
	if (m_GetScenarioStruct->scenarioIndex < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(m_GetScenarioStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
		return false;

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioManager::DragMove()
{
	bool bSuccess = false;

	return bSuccess;
}

bool ScenarioManager::DragDown()
{
	bool bSuccess = false;

	m_mainDlg->SetCapture();
	// thread에 시그널 보내기

	return bSuccess;
}

bool ScenarioManager::DragUp()
{
	bool bSuccess = false;

	ReleaseCapture();
	// thread에 시그널 보내기

	return bSuccess;
}

bool ScenarioManager::TimeLineAttach()
{
	bool bSuccess = false;

	return bSuccess;
}

bool ScenarioManager::TimeLineDetach()
{
	bool bSuccess = false;

	return bSuccess;
}

bool ScenarioManager::TimeLineContactGridline()
{
	bool bSuccess = false;

	return bSuccess;
}

bool ScenarioManager::TimeLineNotContactGridline()
{
	bool bSuccess = false;

	return bSuccess;
}

bool ScenarioManager::DlgAttach()
{
	bool bSuccess = false;

	return bSuccess;
}
