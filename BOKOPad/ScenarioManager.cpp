#include "pch.h"
#include "ScenarioManager.h"

ScenarioManager::ScenarioManager()
	: m_bAttach(false)
	, m_mainDlg(nullptr)
{
	
}

ScenarioManager::~ScenarioManager()
{

}

void ScenarioManager::AttachManager(CWnd* dlgAttachPointer)
{
	m_bAttach = true;
	m_mainDlg = dlgAttachPointer;
}

CWnd* ScenarioManager::DetachManager()
{
	m_bAttach = false;
	return m_mainDlg;
}

bool ScenarioManager::SendMessages(SendMessageProcess bindMessage, int scenarioIndex)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(bindMessage, scenarioIndex);
}

bool ScenarioManager::HelpInvoker(SendMessageProcess bindMessage, int scenarioIndex)
{
	bool bHelpSuccess = true;

	switch (bindMessage)
	{
	case SMP_CREATE:
		bHelpSuccess = Create(scenarioIndex);
		break;
	case SMP_DESTROY:
		bHelpSuccess = Destroy(scenarioIndex);
		break;
	case SMP_SHOW:
		bHelpSuccess = Show(scenarioIndex);
		break;

	case SMP_HIDE:
		bHelpSuccess = Hide(scenarioIndex);
		break;

	case SWP_EXIST:
		bHelpSuccess = Exist(scenarioIndex);
		break;

	default:
		break;
	}

	return bHelpSuccess;
}

bool ScenarioManager::Create(int createScenario)
{
	if (createScenario < 0)
		return false;

	BOKOScenarioDetailDlg* scenarioDetail = new BOKOScenarioDetailDlg(createScenario, m_mainDlg);
	bool bCreate = (bool)scenarioDetail->Create(scenarioDetail->IDD, m_mainDlg);
	if (bCreate)
	{
		try
		{
			m_scenarioDlgManager.insert(createScenario, scenarioDetail);
		}
		catch (ComplexDuplicateException e)
		{
			// 키 중복은 삭제하고 리턴
			scenarioDetail->DestroyWindow();
			delete scenarioDetail;
			scenarioDetail = nullptr;
			return false;
		}

		scenarioDetail->ShowWindow(SW_SHOW);
	}

	return bCreate;
}

bool ScenarioManager::Destroy(int destroyScenario)
{
	if (destroyScenario < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(destroyScenario);
	if (iter == m_scenarioDlgManager.end())
		return false;


	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	m_scenarioDlgManager.erase(iter->value.key);
	findScenarioDlg->DestroyWindow();
	delete findScenarioDlg;
	findScenarioDlg = nullptr;

	return true;
}

bool ScenarioManager::Show(int showScenario)
{
	if (showScenario < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(showScenario);
	if (iter == m_scenarioDlgManager.end())
		return false;

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_SHOW);

	return true;
}

bool ScenarioManager::Hide(int hideScenario)
{
	if (hideScenario < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(hideScenario);
	if (iter == m_scenarioDlgManager.end())
		return false;

	BOKOScenarioDetailDlg* findScenarioDlg = iter->value.value;
	findScenarioDlg->ShowWindow(SW_HIDE);

	return true;
}

bool ScenarioManager::Exist(int existScenario)
{
	if (existScenario < 0)
		return false;
	if (m_scenarioDlgManager.empty())
		return false;

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(existScenario);
	if (iter == m_scenarioDlgManager.end())
		return false;

	return true;
}