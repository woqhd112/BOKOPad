#include "pch.h"
#include "ProcessStage.h"


StageManager::StageManager()
	: m_bInit(false)
{

}

StageManager::~StageManager()
{
	if (m_bInit)
	{
		if (m_dragStageThread)
		{
			delete m_dragStageThread;
			m_dragStageThread = nullptr;
		}
	}
}

void StageManager::InitStage()
{
	m_bInit = true;
	m_dragStageThread = new DragStage;
	m_dragStageThread->Start();
}

void StageManager::SignalDragData(DragData dragData)
{
	if (m_bInit)
		m_dragStageThread->DragSignal(dragData);
}

DragStage::DragStage()
{
	m_pDragProc = new DragProcess;
}

DragStage::~DragStage()
{
	if (m_pDragProc)
	{
		delete m_pDragProc;
		m_pDragProc = nullptr;
	}

	this->Stop();
	m_cond.Destroy();
	this->Join();
}

void DragStage::DragSignal(DragData dragData)
{
	m_dragQueue.lock();
	m_dragQueue.push(dragData);
	m_dragQueue.unlock();

	m_cond.Signal();
}


void DragStage::Run()
{
	while (this->IsRunning())
	{
		m_cond.Wait();

		DragData data;

		m_dragQueue.lock();
		data = m_dragQueue.front();
		m_dragQueue.pop();
		m_dragQueue.unlock();
	}
}