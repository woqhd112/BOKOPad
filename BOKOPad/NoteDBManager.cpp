#include "pch.h"
#include "NoteDBManager.h"

NoteDBManager::NoteDBManager()
{

}

NoteDBManager::~NoteDBManager()
{

}

bool NoteDBManager::SelectInSceSEQNoteInformation(int in, ComplexVector<NoteInformationVO>* out)
{
	m_inputNoteVO.SetSceSEQ(in);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->SelectInSceSEQNoteInformation())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}

	return false;
}

bool NoteDBManager::UpdateNoteInformationInSetTIMELINE(int in_notSEQ, bool in_setTIMELINE)
{
	m_inputNoteVO.SetNotSEQ(in_notSEQ);
	m_inputNoteVO.SetSetTIMELINE(in_setTIMELINE);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->UpdateNoteInformationInSetTIMELINE())
		return true;

	return false;
}

bool NoteDBManager::InsertNoteInformation(NoteInformationVO& note)
{
	RequestScope->SetRequestAttributes(note);
	if (MVC_Controller->InsertNoteInformation())
		return true;

	return false;
}

bool NoteDBManager::DeleteNoteInformation(int in_notSEQ)
{
	m_inputNoteVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->DeleteNoteInformation())
		return true;

	return false;
}

bool NoteDBManager::SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out)
{
	m_inputNoteVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->SelectOneNoteInformation())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}

	return false;
}