#include "pch.h"
#include "ScenarioDBManager.h"

ScenarioDBManager::ScenarioDBManager()
{

}

ScenarioDBManager::~ScenarioDBManager()
{
	// mvc 컨트롤러는 static인 시나리오 매니저가 종료될때 해제한다.
	if (MVC_Controller)
	{
		delete MVC_Controller;
		MVC_Controller = nullptr;
	}
}

bool ScenarioDBManager::SelectAllPadOption(PadOptionVO* out)
{
	if (MVC_Controller->SelectAllPadOption())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}
	return false;
}

bool ScenarioDBManager::SelectAllScenarioList(ComplexVector<ScenarioListVO>* out)
{
	if (MVC_Controller->SelectAllScenarioList())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}
	return false;
}

bool ScenarioDBManager::InsertScenarioList(ComplexString in)
{
	RequestScope->SetRequestAttributes(ScenarioListVO(0, 0, in));
	if (MVC_Controller->InsertScenarioList())
		return true;

	return false;
}

bool ScenarioDBManager::DeleteScenarioList(int in)
{
	m_inputScenarioVO.SetSceSEQ(in);
	RequestScope->SetRequestAttributes(m_inputScenarioVO);
	m_inputScenarioVO.Clear();

	if (MVC_Controller->DeleteScenarioList())
		return true;

	return false;
}

bool ScenarioDBManager::UpdateScenarioListAutoIncrementSeq()
{
	if (MVC_Controller->UpdateScenarioListAutoIncrementSeq())
		return true;

	return false;
}

bool ScenarioDBManager::SelectInSceSEQScenarioListInSceTITLE(ComplexString in_sceTITLE, ScenarioListVO* out)
{
	m_inputScenarioVO.SetSceTITLE(in_sceTITLE);
	RequestScope->SetRequestAttributes(m_inputScenarioVO);
	m_inputScenarioVO.Clear();

	if (MVC_Controller->SelectInSceSEQScenarioListInSceTITLE())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}

	return false;
}

bool ScenarioDBManager::SelectInSceSEQNoteInformation(int in_sceSEQ, ComplexVector<NoteInformationVO>* out)
{
	m_inputNoteVO.SetSceSEQ(in_sceSEQ);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->SelectInSceSEQNoteInformation())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}
	return false;
}

bool ScenarioDBManager::InsertNoteInformation(NoteInformationVO& in)
{
	RequestScope->SetRequestAttributes(in);
	if (MVC_Controller->InsertNoteInformation())
		return true;

	return false;
}

bool ScenarioDBManager::UpdateNoteInformationInNotContent(int in_notSEQ, ComplexString in_notCONTENT)
{
	m_inputNoteVO.SetNotSEQ(in_notSEQ);
	m_inputNoteVO.SetNotCONTENT(in_notCONTENT);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->UpdateNoteInformationInNotContent())
		return true;

	return false;
}

bool ScenarioDBManager::DeleteAllScenarioList()
{
	if (MVC_Controller->DeleteAllScenarioList())
		return true;

	return false;
}