#include "pch.h"
#include "ScenarioUIManager.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKODragDlg.h"

ScenarioUIManager::ScenarioUIManager()
{
	m_bIsDragging = true;
}

ScenarioUIManager::~ScenarioUIManager()
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

bool ScenarioUIManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(message);
}

bool ScenarioUIManager::HelpInvoker(PerformanceMessage message)
{
	bool bHelpSuccess = false;

	if (message == PM_CREATE)
	{
		bHelpSuccess = Create();
	}
	else if (message == PM_DESTROY)
	{
		bHelpSuccess = Destroy();
	}
	else if (message == PM_SHOW)
	{
		bHelpSuccess = Show();
	}
	else if (message == PM_HIDE)
	{
		bHelpSuccess = Hide();
	}
	else if (message == PM_EXIST)
	{
		bHelpSuccess = Exist();
	}
	else if (message == PM_SCENARIO_CLEAR)
	{
		bHelpSuccess = Clear();
	}
	else if (message == PM_NOTE_RELOAD)
	{
		bHelpSuccess = NoteReload();
	}
	else if (message == PM_IS_DRAGGING_MODE)
	{
		bHelpSuccess = IsDraggingMode();
	}
	else if (message == PM_DRAG_PROCESS_LOCK)
	{
		bHelpSuccess = DragProcessLock();
	}

	return bHelpSuccess;
}

bool ScenarioUIManager::Create()
{
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}
	
	ScenarioManagerStruct inputScenarioStruct;
	inputScenarioStruct.scenarioData = scenarioDataStruct->scenarioData;
	inputScenarioStruct.scenarioIndex = scenarioDataStruct->scenarioIndex;

	BOKOScenarioDetailDlg* scenarioDetail = new BOKOScenarioDetailDlg(inputScenarioStruct, m_mainDlg);
	bool bCreate = (bool)scenarioDetail->Create(BOKOScenarioDetailDlg::IDD, m_mainDlg);
	if (bCreate)
	{
		try
		{
			m_scenarioDlgManager.insert(inputScenarioStruct.scenarioIndex, scenarioDetail);
			m_scenarioSeqMap.insert(scenarioDataStruct->scenarioIndex, scenarioDataStruct->scenarioData.GetSceSEQ());
		}
		catch (ComplexDuplicateException e)
		{
			// Ű �ߺ��� �����ϰ� ����
			scenarioDetail->DestroyWindow();
			delete scenarioDetail;
			scenarioDetail = nullptr;
			ReleaseScenarioStruct();
			return false;
		}

		scenarioDetail->ShowWindow(SW_SHOW);
		if (scenarioDetail->SignalLoadScenarioList() == false)
		{
			m_scenarioDlgManager.erase(inputScenarioStruct.scenarioIndex);
			m_scenarioSeqMap.erase(inputScenarioStruct.scenarioIndex);
			scenarioDetail->DestroyWindow();
			delete scenarioDetail;
			scenarioDetail = nullptr;
			ReleaseScenarioStruct();
			return false;
		}
	}
	else
	{
		delete scenarioDetail;
		scenarioDetail = nullptr;
	}

	ReleaseScenarioStruct();

	return bCreate;
}

bool ScenarioUIManager::Destroy()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	int scenarioIndexKey = iter->value.key;
	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	m_scenarioDlgManager.erase(scenarioIndexKey);
	m_scenarioSeqMap.erase(scenarioIndexKey);
	findScenarioDlg->DestroyWindow();
	delete findScenarioDlg;
	findScenarioDlg = nullptr;

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::Show()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_SHOW);

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::Hide()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_HIDE);

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::Exist()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::Clear()
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
	m_scenarioSeqMap.clear();

	return true;
}

bool ScenarioUIManager::NoteReload()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	if (iter->value.value->SignalReloadNoteList() == false)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::IsDraggingMode()
{
	if (m_scenarioDlgManager.empty())
		return false;

	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (scenarioDataStruct->scenarioIndex < 0)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(scenarioDataStruct->scenarioIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseScenarioStruct();
		return false;
	}

	ReleaseScenarioStruct();
	return iter->value.value->m_bDragModeCheck;
}

bool ScenarioUIManager::DragProcessLock()
{
	return m_bIsDragging;
}