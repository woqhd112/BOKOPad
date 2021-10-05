#include "TimelineDlgModelImp.h"

void PrepareStatementSelectInSceSEQTimeline()
{
	int sceSEQ;
	DATA_INSTANCE->GetTimeline(NULL, NULL, &sceSEQ);
	DB_INSTANCE->SetInt(1, sceSEQ);
	DATA_INSTANCE->ResetTimeline();
}

void ResultSetSelectInSceSEQTimeline()
{
	DATA_INSTANCE->SetTimeline(DB_INSTANCE->GetInt(0), DB_INSTANCE->GetInt(1), DB_INSTANCE->GetInt(2));
}

void PrepareStatementInsertTimeline()
{
	int timeIDX, notSEQ, sceSEQ;
	DATA_INSTANCE->GetTimeline(&timeIDX, &notSEQ, &sceSEQ);
	DB_INSTANCE->SetInt(1, timeIDX);
	DB_INSTANCE->SetInt(2, notSEQ);
	DB_INSTANCE->SetInt(3, sceSEQ);
	DATA_INSTANCE->ResetTimeline();
}

void PrepareStatementDeleteTimeline()
{
	int notSEQ;
	DATA_INSTANCE->GetTimeline(NULL, &notSEQ, NULL);
	DB_INSTANCE->SetInt(1, notSEQ);
	DATA_INSTANCE->ResetTimeline();
}

void PrepareStatementUpdateTimelineInTimeIDX()
{
	int sceSEQ, timeIDX;
	DATA_INSTANCE->GetTimeline(&timeIDX, NULL, &sceSEQ);
	DB_INSTANCE->SetInt(1, timeIDX + 1);
	DB_INSTANCE->SetInt(2, timeIDX);
	DB_INSTANCE->SetInt(3, sceSEQ);
	DATA_INSTANCE->ResetTimeline();
}

TimelineDlgModelImp::TimelineDlgModelImp()
{

}

TimelineDlgModelImp::~TimelineDlgModelImp()
{

}

bool TimelineDlgModelImp::SelectInSceSEQTimeline(int in_sceSEQ, ComplexVector<TimelineVO>* out_timelineContainer)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetTimeline(0, 0, in_sceSEQ);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[SELECT_ALL_TIME_LINE_TABLE_IN_SCESEQ], PrepareStatementSelectInSceSEQTimeline, ResultSetSelectInSceSEQTimeline);

	if (bSuccess)
		DATA_INSTANCE->GetTimelineContainer(out_timelineContainer);

	DATA_INSTANCE->ResetTimeline();

	return bSuccess;
}

bool TimelineDlgModelImp::InsertTimeline(int in_timeIDX, int in_notSEQ, int in_sceSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetTimeline(in_timeIDX, in_notSEQ, in_sceSEQ);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[INSERT_TIME_LINE_TABLE], PrepareStatementInsertTimeline);

	DATA_INSTANCE->ResetTimeline();

	return bSuccess;
}

bool TimelineDlgModelImp::DeleteTimeline(int in_notSEQ)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetTimeline(0, in_notSEQ, 0);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[DELETE_TIME_LINE_TABLE], PrepareStatementDeleteTimeline);

	DATA_INSTANCE->ResetTimeline();

	return bSuccess;
}

bool TimelineDlgModelImp::UpdateTimelineInTimeIDX(int in_sceSEQ, int in_timeIDX)
{
	bool bSuccess = false;
	DATA_INSTANCE->SetTimeline(in_timeIDX, 0, in_sceSEQ);
	bSuccess = DB_INSTANCE->PrepareStatement_Execute(DefinedDMLQuerys[UPDATE_TIME_LINE_TABLE_IN_TIMEIDX], PrepareStatementUpdateTimelineInTimeIDX);

	DATA_INSTANCE->ResetTimeline();

	return bSuccess;
}