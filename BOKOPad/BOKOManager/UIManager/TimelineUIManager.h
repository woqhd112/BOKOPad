#pragma once
#include "UIManagerManagement.h"

class TimelineUIManager : public UIManagerManagement
{
public:

	TimelineUIManager();
	virtual ~TimelineUIManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool InvokeHelper(PerformanceMessage message);

	bool TimelineDragDown();
	bool TimelineDragMove();
	bool TimelineDragUp();
	bool NoteInsert();
	

	bool m_bCursorAttach;
	bool m_bCursorDetach;

};

