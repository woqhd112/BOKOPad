#pragma once
#include "DlgModel.h"

void PrepareStatementSelectInSceSEQTimeline();
void ResultSetSelectInSceSEQTimeline();
void PrepareStatementInsertTimeline();
void PrepareStatementDeleteTimeline();
void PrepareStatementUpdateTimelineInTimeIDX();

class TimelineDlgModelImp : public DlgModel
{
public:

	TimelineDlgModelImp();
	virtual ~TimelineDlgModelImp();

	bool SelectInSceSEQTimeline(int in_sceSEQ, ComplexVector<TimelineVO>* out_timelineContainer);
	bool InsertTimeline(int in_timeIDX, int in_notSEQ, int in_sceSEQ);
	bool DeleteTimeline(int in_notSEQ);
	bool UpdateTimelineInTimeIDX(int in_sceSEQ, int in_timeIDX);

private:


};

