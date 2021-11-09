#include "pch.h"
#include "TimelineDBManager.h"

TimelineDBManager::TimelineDBManager()
{

}

TimelineDBManager::~TimelineDBManager()
{

}

bool TimelineDBManager::SelectInSceSEQTimeline(int in_sceSEQ, ComplexVector<TimelineVO>* out)
{
	m_inputTimeVO.SetSceSEQ(in_sceSEQ);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();
	if (MVC_Controller->SelectInSceSEQTimeline())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}
	return false;
}

bool TimelineDBManager::UpdateTimelineInTimeIDXPlus(int in_notSEQ, int in_timeIDX)
{
	m_inputTimeVO.SetTimeIDX(in_timeIDX);
	m_inputTimeVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();

	if (MVC_Controller->UpdateTimelineInTimeIDXPlus())
		return true;

	return false;
}

bool TimelineDBManager::UpdateTimelineInTimeIDXMinus(int in_notSEQ, int in_timeIDX)
{
	m_inputTimeVO.SetTimeIDX(in_timeIDX);
	m_inputTimeVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();

	if (MVC_Controller->UpdateTimelineInTimeIDXMinus())
		return true;

	return false;
}

bool TimelineDBManager::InsertTimeline(TimelineVO& time)
{
	RequestScope->SetRequestAttributes(time);
	if (MVC_Controller->InsertTimeline())
		return true;

	return false;
}

bool TimelineDBManager::SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out)
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

bool TimelineDBManager::SelectInTimeIDXTimelineInNotSEQ(int in_notSEQ, TimelineVO* out)
{
	m_inputTimeVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();

	if (MVC_Controller->SelectInTimeIDXTimelineInNotSEQ())
	{
		RequestScope->GetRequestAttributes(out);
		return true;
	}
	return false;
}

bool TimelineDBManager::DeleteTimeline(int in_notSEQ, int in_sceSEQ)
{
	m_inputTimeVO.SetSceSEQ(in_sceSEQ);
	m_inputTimeVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();

	if (MVC_Controller->DeleteTimeline())
		return true;

	return false;
}

bool TimelineDBManager::DeleteNoteInformation(int in_notSEQ)
{
	m_inputNoteVO.SetNotSEQ(in_notSEQ);
	RequestScope->SetRequestAttributes(m_inputNoteVO);
	m_inputNoteVO.Clear();

	if (MVC_Controller->DeleteNoteInformation())
		return true;

	return false;
}

bool TimelineDBManager::UpdateTimelineInTimeIDX(int in_notSEQ, int in_timeIDX, int in_updateTimeIDX)
{
	m_inputTimeVO.SetNotSEQ(in_notSEQ);
	m_inputTimeVO.SetTimeIDX(in_timeIDX);
	RequestScope->SetRequestInt(in_updateTimeIDX);
	RequestScope->SetRequestAttributes(m_inputTimeVO);
	m_inputTimeVO.Clear();

	if (MVC_Controller->UpdateTimelineInTimeIDX())
		return true;

	return false;
}