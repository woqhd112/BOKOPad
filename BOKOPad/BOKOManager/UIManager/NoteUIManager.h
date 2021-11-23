#pragma once
#include "UIManagerManagement.h"

class NoteUIManager : public UIManagerManagement
{
public:

	NoteUIManager();
	virtual ~NoteUIManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	virtual bool InvokeHelper(PerformanceMessage message);

	bool Insert();
	bool Delete();
	bool Show();
	bool Hide();
	bool Move();
	bool NoteClick();
	bool NoteCheckDelete();
	bool DragMove();
	bool DragDown();
	bool DragUp();
	bool DragOff();
	bool Clear();
	bool InsertNoteViewUpdate();
	bool SeqNoteViewUpdate();

	bool LoadDraggingMode();
	bool UnloadDraggingMode();

	bool DragNothing();
	bool DragThisAttach();
	bool DragAnotherAttach();
	bool DragThisTimelineAttach();
	bool DragAnotherTimelineAttach();

	bool FindNoteIndex();

	bool RollbackAnotherAttach();
	bool RollbackThisTimelineAttach();
	bool RollbackTimelineAnotherAttach();
	bool RollbackThisAnotherTimelineAttach();

private:

	int m_variableCtrlPos_x;
	int m_variableCtrlPos_y;


	bool m_bCursorAttach;
	bool m_bCursorDetach;
	/*ComplexMap<int, NotePadStruct> m_notePadManager;
	ComplexMap<int, int> m_noteSeqMap;*/

};

