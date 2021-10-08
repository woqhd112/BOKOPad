#pragma once
#include "TimelineDlgModelImp.h"

class TimelineDlgService
{
public:

	TimelineDlgService();
	~TimelineDlgService();

	bool SelectInSceSEQTimeline(int sceSEQ);
	bool InsertTimeline(int timeIDX, int notSEQ, int sceSEQ);
	bool DeleteTimeline(int notSEQ, int sceSEQ);
	bool UpdateTimelineInTimeIDXPlus(int notSEQ, int timeIDX);
	bool UpdateTimelineInTimeIDXMinus(int notSEQ, int timeIDX);
	bool UpdateTimelineInTimeIDX(int notSEQ, int baseTimeIDX, int updateTimeIDX);
	bool SelectInTimeIDXTimelineInNotSEQ(int notSEQ);

private:

	TimelineDlgModelImp* m_timelineDlgModel;

};

