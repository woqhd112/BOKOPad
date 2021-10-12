#pragma once
#include "DBManagerManagement.h"

class TimelineDBManager : public DBManagerManagement
{
public:

	TimelineDBManager();
	virtual ~TimelineDBManager();

	bool SelectInSceSEQTimeline(int in_sceSEQ, ComplexVector<TimelineVO>* out);
	bool UpdateTimelineInTimeIDXPlus(int in_notSEQ, int in_timeIDX);
	bool UpdateTimelineInTimeIDXMinus(int in_notSEQ, int in_timeIDX);
	bool InsertTimeline(TimelineVO& time);
	bool SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out);
	bool SelectInTimeIDXTimelineInNotSEQ(int in_notSEQ, TimelineVO* out);
	bool DeleteTimeline(int in_notSEQ, int in_sceSEQ);
	bool DeleteNoteInformation(int in_notSEQ);
	bool UpdateTimelineInTimeIDX(int in_notSEQ, int in_timeIDX, int in_updateTimeIDX);

private:


};

