#pragma once
#include "TimelineDlgModelImp.h"

class TimelineDlgService
{
public:

	TimelineDlgService();
	~TimelineDlgService();

	bool SelectInSceSEQTimeline(int sceSEQ);
	bool InsertTimeline(int timeIDX, int notSEQ, int sceSEQ);
	bool DeleteTimeline(int notSEQ);
	bool UpdateTimelineInTimeIDX(int sceSEQ, int timeIDX);

private:

	TimelineDlgModelImp* m_timelineDlgModel;

};

