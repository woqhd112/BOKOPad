#pragma once
#include "ManagerManagement.h"

class TimelineManager : public ManagerManagement
{
public:

	TimelineManager();
	virtual ~TimelineManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool HelpInvoker(PerformanceMessage message);

	bool TimelineDragDown();
	bool TimelineDragMove();
	bool TimelineDragUp();
	bool NoteInsert();

	bool m_bCursorAttach;
	bool m_bCursorDetach;

};

