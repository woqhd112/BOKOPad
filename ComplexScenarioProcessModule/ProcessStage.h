#pragma once
#include "commonInclude.h"
#include "DragProcess.h"
#include "TimelineProcess.h"
#include "NoteProcess.h"
#include "ComplexUniqueQueue.h"

struct DragData
{
	int sceIndex;
	int noteSEQ;
	int sceSEQ;
	ComplexString noteCONTENT;

	int mousePos_X;
	int mousePos_Y;
};

class DragStage;

class COMPLEXSCENARIOPROCESSMODULE_DLL StageManager
{
public:

	StageManager();
	~StageManager();

	void InitStage();
	void SignalDragData(DragData dragData);

private:

	bool m_bInit;
	DragStage* m_dragStageThread;

};

class DragStage : private ComplexThread
{
public:

	friend class StageManager;

	DragStage();
	~DragStage();

protected:

	virtual void Run();

private:

	DragProcess* m_pDragProc;
	//TimelineProcess* m_pTimeProc;
	//NoteProcess* m_pNoteProc;
	void DragSignal(DragData dragData);

	ComplexLock m_lock;
	ComplexCondition m_cond;
	ComplexUniqueQueue<DragData> m_dragQueue;

};
