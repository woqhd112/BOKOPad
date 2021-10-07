#include "pch.h"
#include "TimelineManager.h"
#include "Timeline.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKODragDlg.h"

TimelineManager::TimelineManager()
	: m_bCursorAttach(true)
	, m_bCursorDetach(false)
{

}

TimelineManager::~TimelineManager()
{

}

bool TimelineManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return HelpInvoker(message);
}

bool TimelineManager::HelpInvoker(PerformanceMessage message)
{
	bool bHelpSuccess = false;

	if (message == PM_TIMELINE_DRAG_DOWN)
	{
		bHelpSuccess = TimelineDragDown();
	}
	else if (message == PM_TIMELINE_DRAG_MOVE)
	{
		bHelpSuccess = TimelineDragMove();
	}
	else if (message == PM_TIMELINE_DRAG_UP)
	{
		bHelpSuccess = TimelineDragUp();
	}
	else if (message = PM_NOTE_INSERT)
	{
		bHelpSuccess = NoteInsert();
	}

	return bHelpSuccess;
}

bool TimelineManager::TimelineDragDown()
{
	m_dragState = DUS_THIS_TIMELINE;

	CURSOR_CROSS;
	m_mainDlg->SetCapture();

	return true;
}

bool TimelineManager::TimelineDragMove()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	// 마우스 커서 이벤트처리 타임라인일 때
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);

	CRect rect;
	m_mainDlg->GetWindowRect(rect);
	if (PtInRect(rect, pt))
	{
		if (m_bCursorDetach)
		{
			m_bCursorAttach = true;
			CursorCountRestore(0);
			m_dragDlg->ShowWindow(SW_HIDE);
			CURSOR_CROSS;
			m_bCursorDetach = false;

		}
		//iter1->value.value->m_timeline.ThickEventTimeline(dragDataStruct->noteSEQ, pt, false);
	}
	else
	{
		if (m_bCursorAttach)
		{
			m_bCursorDetach = true;
			CursorCountRestore(-1);
			m_dragDlg->ShowWindow(SW_SHOW);
			m_bCursorAttach = false;
		}
	}

	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);

	ReleaseDragStruct();

	return true;
}

bool TimelineManager::TimelineDragUp()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}


	// 해당 위치가 어딘지에 따라 처리해야하는 방법이 다름
	// 마우스의 포인트가 현재 다이얼로그가 아닐때 처리
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.begin();
	while (iter1 != m_scenarioDlgManager.end())
	{
		CRect rect;
		iter1->value.value->GetWindowRect(rect);
		if (PtInRect(rect, pt))
			break;

		iter1++;
	}

	// 다이얼로그를 찾지 못하였을 때
	if (iter1 == m_scenarioDlgManager.end())
	{
		m_dragState = DUS_NOTHING;
	}
	// 찾은 다이얼로그가 현재 다이얼로그일 때
	else if (iter1->value.value == m_mainDlg->GetParent())
	{
		m_dragState = DUS_THIS;
		// 타임라인인지 구분

		CRect rect;
		m_mainDlg->GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_THIS_TIMELINE;
		}
	}
	// 찾은 다이얼로그가 다른 다이얼로그일 때
	else
	{
		m_dragState = DUS_ANOTHER;

		// 다른 다이얼로그일 때 해당 다이얼로그의 seq번호를 구조체에 저장
		ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.find(iter1->value.key);
		if (iter2 != m_scenarioSeqMap.end())
			dragDataStruct->target_sceSEQ = iter2->value.value;

		// 타임라인인지 구분 (이걸 처리할까..)
		/*CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_ANOTHER_TIMELINE;
		}*/
	}

	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	CursorCountRestore(0);
	CURSOR_ARROW;
	ReleaseCapture();
	return true;
}

bool TimelineManager::NoteInsert()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	if (m_scenarioDlgManager.empty())
	{
		ReleaseDragStruct();
		return false;
	}

	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter = m_scenarioDlgManager.find(dragDataStruct->sceIndex);

	if (iter == m_scenarioDlgManager.end())
	{
		ReleaseDragStruct();
		return false;
	}

	if (iter->value.value->SignalUpdateSetTIME(dragDataStruct->noteSEQ) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();

	return true;
}