#pragma once
#include "../DlgModel.h"

void PrepareStatementSelectInSceSEQTimeline();
void ResultSetSelectInSceSEQTimeline();
void PrepareStatementInsertTimeline();
void PrepareStatementDeleteTimeline();
void PrepareStatementUpdateTimelineInTimeIDXPlus();
void PrepareStatementUpdateTimelineInTimeIDXMinus();
void PrepareStatementUpdateTimelineInTimeIDX();
void PrepareStatementUpdateTimelineInTimeIDXMinusInSceSEQ();
void PrepareStatementSelectCountTimelineInSceSEQ();
void ResultSetSelectCountTimelineInSceSEQ();
void PrepareStatementSelectInTimeIDXTimelineInNotSEQ();
void ResultSetSelectInTimeIDXTimelineInNotSEQ();

class TimelineDlgModelImp : public DlgModel
{
public:

	TimelineDlgModelImp();
	virtual ~TimelineDlgModelImp();

	bool SelectInSceSEQTimeline(int in_sceSEQ, ComplexVector<TimelineVO>* out_timelineContainer);
	bool InsertTimeline(int in_timeIDX, int in_notSEQ, int in_sceSEQ);
	bool DeleteTimeline(int in_notSEQ);
	bool UpdateTimelineInTimeIDXPlus(int in_notSEQ, int in_timeIDX);
	bool UpdateTimelineInTimeIDXMinus(int in_notSEQ, int in_timeIDX);
	bool UpdateTimelineInTimeIDX(int in_notSEQ, int in_baseTimeIDX, int in_updateTimeIDX);
	bool UpdateTimelineInTimeIDXMinusInSceSEQ(int in_sceSEQ, int in_timeIDX);
	bool SelectCountTimelineInSceSEQ(int in_sceSEQ, int* out_count);
	bool SelectInTimeIDXTimelineInNotSEQ(int in_notSEQ, TimelineVO* out_timeline);

private:


};

