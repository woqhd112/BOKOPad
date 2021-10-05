#include "TimelineDlgService.h"


TimelineDlgService::TimelineDlgService()
	: m_timelineDlgModel(new TimelineDlgModelImp)
{

}

TimelineDlgService::~TimelineDlgService()
{
	if (m_timelineDlgModel)
	{
		delete m_timelineDlgModel;
		m_timelineDlgModel = nullptr;
	}
}

bool TimelineDlgService::SelectInSceSEQTimeline(int sceSEQ)
{
	bool bSuccess = false;

	ComplexVector<TimelineVO> timeLineContainer;
	bSuccess = m_timelineDlgModel->SelectInSceSEQTimeline(sceSEQ, &timeLineContainer);

	if (bSuccess)
	{
		RequestScope->SetRequestAttributes(timeLineContainer);
	}

	return bSuccess;
}

bool TimelineDlgService::InsertTimeline(int timeIDX, int notSEQ, int sceSEQ)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->InsertTimeline(timeIDX, notSEQ, sceSEQ);

	if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();

	return bSuccess;
}

bool TimelineDlgService::DeleteTimeline(int notSEQ)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->DeleteTimeline(notSEQ);

	if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();

	return bSuccess;
}

bool TimelineDlgService::UpdateTimelineInTimeIDX(int sceSEQ, int timeIDX)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->UpdateTimelineInTimeIDX(sceSEQ, timeIDX);

	if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();

	return bSuccess;
}