#include "pch.h"
#include "ProcessStage.h"


StageManager::StageManager()
	: m_bInit(false)
	, m_dragData(new DragData)
	, m_pDragProc(new DragProcess)
	, m_pTimelineProc(new TimelineProcess)
	, m_pNoteProc(new NoteProcess)
	, m_ds(DS_NOTHING)
{
	m_dragData->Reset();
}

StageManager::~StageManager()
{
	if (m_bInit)
	{
		this->Stop();
		m_cond.Destroy();
		this->Join();
	}

	if (m_pDragProc)
	{
		delete m_pDragProc;
		m_pDragProc = nullptr;
	}

	if (m_pTimelineProc)
	{
		delete m_pTimelineProc;
		m_pTimelineProc = nullptr;
	}

	if (m_pNoteProc)
	{
		delete m_pNoteProc;
		m_pNoteProc = nullptr;
	}
}

void StageManager::InitStage()
{
	m_bInit = true;
	this->Start();
}

void StageManager::SignalDragData(DragData& dragData)
{
	ComplexScopedLock lock(&m_lock);

	if (!m_bInit)
		return;

	m_dragData->sceIndex = dragData.sceIndex;
	m_dragData->sceSEQ = dragData.sceSEQ;
	m_dragData->noteSEQ = dragData.noteSEQ;
	m_dragData->noteCONTENT = dragData.noteCONTENT;
	m_dragData->mousePos_Y = dragData.mousePos_Y;
	m_dragData->mousePos_X = dragData.mousePos_X;
	SetDragState(DS_CREATE);

	m_cond.Signal();
}

void StageManager::SetDragState(DragState ds)
{
	ComplexScopedLock lock(&m_dsLock);

	if (!m_bInit)
		return;

	m_ds = DS_CREATE;
}

void StageManager::UnloadDragData()
{
	ComplexScopedLock lock(&m_lock);

	if (!m_bInit)
		return;

	m_dragData->Reset();
	SetDragState(DS_DESTROY);

	m_cond.Signal();
}

void StageManager::ProcessingDragData(DragData& dragData)
{
	ComplexScopedLock lock(&m_lock);

	if (!m_bInit)
		return;

	m_dragData->sceIndex = dragData.sceIndex;
	m_dragData->sceSEQ = dragData.sceSEQ;
	m_dragData->noteSEQ = dragData.noteSEQ;
	m_dragData->noteCONTENT = dragData.noteCONTENT;
	m_dragData->mousePos_Y = dragData.mousePos_Y;
	m_dragData->mousePos_X = dragData.mousePos_X;
	SetDragState(DS_MOVING);

	m_cond.Signal();
}

DragData* StageManager::GetDragData()
{
	ComplexScopedLock lock(&m_lock);

	if (!m_bInit)
		return nullptr;

	return m_dragData;
}

void StageManager::Run()
{
	while (this->IsRunning())
	{
		m_cond.Wait();

		if (m_ds == DS_CREATE)
		{
			m_pDragProc->CreateDragDlg(m_dragData);
		}
		else if (m_ds == DS_MOVING)
		{
			m_pDragProc->MovingDragDlg(m_dragData);
		}
		else if (m_ds == DS_DESTROY)
		{
			m_pDragProc->DestroyDragDlg(m_dragData);
		}
	}
}
