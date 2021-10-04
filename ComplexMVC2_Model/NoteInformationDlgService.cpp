#include "pch.h"
#include "NoteInformationDlgService.h"

NoteInformationDlgService::NoteInformationDlgService()
	: m_noteInformationDlgModel(new NoteInformationDlgModelImp)
{

}

NoteInformationDlgService::~NoteInformationDlgService()
{
	if (m_noteInformationDlgModel)
	{
		delete m_noteInformationDlgModel;
		m_noteInformationDlgModel = nullptr;
	}
}

bool NoteInformationDlgService::SelectInSceSEQNoteInformation(int in_sceSEQ)
{
	bool bSuccess = false;

	ComplexVector<NoteInformationVO> noteInformationContainer;
	bSuccess = m_noteInformationDlgModel->SelectInSceSEQNoteInformation(in_sceSEQ, &noteInformationContainer);

	if (bSuccess)
		RequestScope->SetRequestAttributes(noteInformationContainer);

	return bSuccess;
}

bool NoteInformationDlgService::SelectOneNoteInformation(int in_notSEQ)
{
	bool bSuccess = false;

	NoteInformationVO noteInformation;
	bSuccess = m_noteInformationDlgModel->SelectOneNoteInformation(in_notSEQ, &noteInformation);

	if (bSuccess)
		RequestScope->SetRequestAttributes(noteInformation);

	return bSuccess;
}

bool NoteInformationDlgService::InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ)
{
	bool bSuccess = false;

	// sceSEQ������ notCNT �� ��������..
	ScenarioListVO scenarioList;
	bSuccess = m_noteInformationDlgModel->SelectInNotCNTScenarioListInSceSEQ(in_sceSEQ, &scenarioList);

	if (bSuccess)
		bSuccess = m_noteInformationDlgModel->InsertNoteInformation(in_notCONTENT, in_notLOCK, in_sceSEQ);

	if (bSuccess)
	{
		// �ó����� ����Ʈ cnt ���� �߰�
		bSuccess = m_noteInformationDlgModel->UpdateScenarioListInNotCNT(in_sceSEQ, scenarioList.GetNotCNT() + 1);
	}

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}

bool NoteInformationDlgService::UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ)
{
	bool bSuccess = false;

	bSuccess = m_noteInformationDlgModel->UpdateNoteInformation(in_notCONTENT, in_notLOCK, in_notSEQ);

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}

bool NoteInformationDlgService::DeleteNoteInformation(int in_notSEQ)
{
	bool bSuccess = false;

	// notSEQ������ sceSEQ �� ��������..
	ScenarioListVO scenarioList;
	bSuccess = m_noteInformationDlgModel->SelectInSceSEQScenarioListInNotSEQ(in_notSEQ, &scenarioList);

	if (bSuccess)
		bSuccess = m_noteInformationDlgModel->DeleteNoteInformation(in_notSEQ);

	// ��Ʈ ���� �� �ó����� ����Ʈ�� cnt ����
	if (bSuccess)
		bSuccess = m_noteInformationDlgModel->UpdateScenarioListInNotCNT(scenarioList.GetSceSEQ(), scenarioList.GetNotCNT() - 1);	

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}

bool NoteInformationDlgService::UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ)
{
	bool bSuccess = false;

	bSuccess = m_noteInformationDlgModel->UpdateNoteInformationInNotContent(in_notCONTENT, in_notSEQ);

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}

bool NoteInformationDlgService::UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ)
{
	bool bSuccess = false;

	bSuccess = m_noteInformationDlgModel->UpdateNoteInformationInNotLOCK(in_notLOCK, in_notSEQ);

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}

bool NoteInformationDlgService::UpdateNoteInformationInSceSEQ(int in_notSEQ, int in_sceSEQ)
{
	bool bSuccess = false;

	bSuccess = m_noteInformationDlgModel->UpdateNoteInformationInSceSEQ(in_notSEQ, in_sceSEQ);

	if (bSuccess)
		m_noteInformationDlgModel->Commit();
	else
		m_noteInformationDlgModel->Rollback();

	return bSuccess;
}