#pragma once
#include "ManagerManagement.h"

class NoteManager : public ManagerManagement
{
public:

	NoteManager();
	virtual ~NoteManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool HelpInvoker(PerformanceMessage message);

	bool Insert();
	bool Delete();
	bool Show();
	bool Hide();
	bool Move();
	bool DragMove();
	bool DragDown();
	bool DragUp();
	bool Clear();

	bool DragNothing();
	bool DragThisAttach();
	bool DragAnotherAttach();
	bool DragThisTimelineAttach();
	bool DragAnotherTimelineAttach();

	bool FindNoteIndex();

	bool RollbackAnotherAttach();

private:

	int m_variableCtrlPos_x;
	int m_variableCtrlPos_y;



	bool m_bCursorAttach;
	bool m_bCursorDetach;
	/*ComplexMap<int, NotePadStruct> m_notePadManager;
	ComplexMap<int, int> m_noteSeqMap;*/

};

