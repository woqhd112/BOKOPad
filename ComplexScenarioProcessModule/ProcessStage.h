#pragma once
#include "commonInclude.h"
#include "DragProcess.h"
#include "TimelineProcess.h"
#include "NoteProcess.h"
#include "ComplexUniqueQueue.h"

class CWnd;
class DragStage;

class StageManager : public ComplexThread
{
public:

	StageManager();
	~StageManager();

	void InitStage();
	void SignalDragData(DragData& dragData);
	void UnloadDragData();
	void ProcessingDragData(DragData& dragData);
	DragData* GetDragData();

protected:

	virtual void Run();

private:

	enum DragState
	{
		DS_NOTHING = 0,
		DS_CREATE,
		DS_MOVING,
		DS_DESTROY
	};

	void SetDragState(DragState ds);

	bool m_bInit;
	//DragStage* m_dragStageThread;

	DragState m_ds;

	ComplexLock m_lock;
	ComplexLock m_dsLock;

	ComplexCondition m_cond;
	DragData* m_dragData;

	DragProcess* m_pDragProc;
	TimelineProcess* m_pTimelineProc;
	NoteProcess* m_pNoteProc;
};
