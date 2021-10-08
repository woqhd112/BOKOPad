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

	/*if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();*/

	return bSuccess;
}

bool TimelineDlgService::DeleteTimeline(int notSEQ, int sceSEQ)
{
	bool bSuccess = false;

	// 삭제 전 해당 notSEQ에 맞는 timeIDX를 가져온다.
	TimelineVO time;
	bSuccess = m_timelineDlgModel->SelectInTimeIDXTimelineInNotSEQ(notSEQ, &time);

	// 타임라인 테이블의 총 레코드 개수를 가져온다.
	int recordCount = 0;
	if (bSuccess)
		bSuccess = m_timelineDlgModel->SelectCountTimelineInSceSEQ(sceSEQ, &recordCount);

	// 해당 notSEQ의 타임라인을 삭제한다.
	if (bSuccess)
		bSuccess = m_timelineDlgModel->DeleteTimeline(notSEQ);

	// 삭제한 timeIDX 이후의 레코드들을 전부
	int deleteTimeIDX = time.GetTimeIDX();

	if (recordCount > deleteTimeIDX)
	{
		for (int i = deleteTimeIDX + 1; i <= recordCount; i++)
		{
			bSuccess = m_timelineDlgModel->UpdateTimelineInTimeIDXMinusInSceSEQ(sceSEQ, i);
			if (!bSuccess)
				break;
		}
	}
	else
		bSuccess = false;

	/*if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();*/

	return bSuccess;
}

bool TimelineDlgService::UpdateTimelineInTimeIDXPlus(int notSEQ, int timeIDX)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->UpdateTimelineInTimeIDXPlus(notSEQ, timeIDX);

	/*if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();*/

	return bSuccess;
}

bool TimelineDlgService::UpdateTimelineInTimeIDXMinus(int notSEQ, int timeIDX)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->UpdateTimelineInTimeIDXMinus(notSEQ, timeIDX);

	/*if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();*/

	return bSuccess;
}

bool TimelineDlgService::UpdateTimelineInTimeIDX(int notSEQ, int baseTimeIDX, int updateTimeIDX)
{
	bool bSuccess = false;

	bSuccess = m_timelineDlgModel->UpdateTimelineInTimeIDX(notSEQ, baseTimeIDX, updateTimeIDX);

	/*if (bSuccess)
		m_timelineDlgModel->Commit();
	else
		m_timelineDlgModel->Rollback();*/

	return bSuccess;
}

bool TimelineDlgService::SelectInTimeIDXTimelineInNotSEQ(int notSEQ)
{
	bool bSuccess = false;

	TimelineVO time;
	bSuccess = m_timelineDlgModel->SelectInTimeIDXTimelineInNotSEQ(notSEQ, &time);

	if (bSuccess)
	{
		RequestScope->SetRequestAttributes(time);
	}

	return bSuccess;
}