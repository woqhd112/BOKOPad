#include "pch.h"
#include "ScenarioUIManager.h"
#include "..\..\BOKOScenarioDetailDlg.h"
#include "..\..\BOKODragDlg.h"

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
	else if (message == PM_THIS_SCENARIO_ONE_VIEW_REFRESH)
	{
		bHelpSuccess = ThisOneViewRefresh();
	}
	else if (message == PM_TARGET_SCENARIO_ONE_VIEW_REFRESH)
	{
		bHelpSuccess = TargetOneViewRefresh();
	}
	else if (message == PM_TIMELINE_RELOAD)
	{
		bHelpSuccess = TimeLineReload();
	}
	else if (message == PM_TIMELINE_ONEVIEW_EXPANDALL_CLOSE)
	{
		bHelpSuccess = TimelineOneViewExpandAllClose();
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
			// 키 중복은 삭제하고 리턴
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
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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

bool ScenarioUIManager::TimeLineReload()
{
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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

	if (iter->value.value->SignalReloadTimeline() == false)
	{
		ReleaseScenarioStruct();
		return false;
	}

	ReleaseScenarioStruct();

	return true;
}

bool ScenarioUIManager::IsDraggingMode()
{
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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

bool ScenarioUIManager::ThisOneViewRefresh()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	if (dragDataStruct->sceIndex < 0)
	{
		ReleaseDragStruct();
		return false;
	}

	// 현재 시나리오의 타임라인 한눈에보기 갱신
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(dragDataStruct->sceIndex);
	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	iter->value.value->SignalTimelineOneViewRefresh();

	ReleaseDragStruct();
	return true;
}

bool ScenarioUIManager::TargetOneViewRefresh()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	if (dragDataStruct->target_sceSEQ == -1)
	{
		ReleaseDragStruct();
		return false;
	}

	// 타겟 시나리오가 존재하면.. (드래그 업으로 target_sceSEQ 값을 부여받은놈이면)
	// 해당 시나리오 타임라인 한눈에보기 갱신
	ComplexMap<int, int>::iterator iter1 = m_scenarioSeqMap.begin();
	bool bFind = false;
	while (iter1 != m_scenarioSeqMap.end())
	{
		if (iter1->value.value == dragDataStruct->target_sceSEQ)
		{
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter3 = m_scenarioDlgManager.find(iter1->value.key);
	if (iter3 == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	iter3->value.value->SignalTimelineOneViewRefresh();

	ReleaseDragStruct();
	return true;
}

bool ScenarioUIManager::TimelineOneViewExpandAllClose()
{
	ScenarioManagerStruct* scenarioDataStruct = BringScenarioStruct();

	if (scenarioDataStruct == nullptr)
		return false;

	if (m_scenarioDlgManager.empty())
	{
		ReleaseScenarioStruct();
		return false;
	}

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

	iter->value.value->m_timeline.SetExpandCloseEvent();

	ReleaseScenarioStruct();

	return true;
}