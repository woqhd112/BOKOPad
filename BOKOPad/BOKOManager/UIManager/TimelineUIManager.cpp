#include "pch.h"
#include "TimelineUIManager.h"
#include "..\..\Timeline.h"
#include "..\..\BOKOScenarioDetailDlg.h"
#include "..\..\BOKODragDlg.h"

TimelineUIManager::TimelineUIManager()
	: m_bCursorAttach(true)
	, m_bCursorDetach(false)
{

}

TimelineUIManager::~TimelineUIManager()
{

}

bool TimelineUIManager::SendMessages(PerformanceMessage message)
{
	if (!m_bAttach)
		return false;

	return InvokeHelper(message);
}

bool TimelineUIManager::InvokeHelper(PerformanceMessage message)
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

bool TimelineUIManager::TimelineDragDown()
{
	m_dragState = DUS_THIS_TIMELINE;

	CURSOR_CROSS;
	m_mainDlg->SetCapture();

	return true;
}

bool TimelineUIManager::TimelineDragMove()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}

	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);
	ComplexMap<int, BOKOScenarioDetailDlg*>::iterator iter1 = m_scenarioDlgManager.begin();

	bool bFind = false;
	while (iter1 != m_scenarioDlgManager.end())
	{
		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			//TRACE("%d��° �ó����� ����\n", iter1->value.key);
			if (m_bCursorDetach)
			{
				m_bCursorAttach = true;
				CursorCountRestore(0);
				m_dragDlg->ShowWindow(SW_HIDE);
				CURSOR_CROSS;
				m_bCursorDetach = false;

			}
			// �ƴ� �̰� �� NOTE BY TIMELINE���� �Ǿ��־��� ����;;
			iter1->value.value->m_timeline.ThickEventTimeline(dragDataStruct->noteSEQ, pt, Timeline::TTA_TIMELINE_BY_TIMELINE_DRAG_EVENT_APPROCH);
			bFind = true;
			break;
		}
		iter1++;
	}

	if (!bFind)
	{
		if (m_bCursorAttach)
		{
			//TRACE("!!\n");
			m_bCursorDetach = true;
			CursorCountRestore(-1);
			m_dragDlg->ShowWindow(SW_SHOW);
			m_bCursorAttach = false;
		}
	}

	/*
	// ���콺 Ŀ�� �̺�Ʈó�� Ÿ�Ӷ����� ��
	CPoint pt(dragDataStruct->mousePos_X, dragDataStruct->mousePos_Y);

	CRect rect;
	Timeline* timeline = (Timeline*)m_mainDlg;
	timeline->GetWindowRect(rect);
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
		timeline->ThickEventTimeline(dragDataStruct->noteSEQ, pt, Timeline::TTA_TIMELINE_BY_TIMELINE_DRAG_EVENT_APPROCH);
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
	}*/

	m_dragDlg->MoveWindow(int(dragDataStruct->mousePos_X - (DRAG_DLG_WIDTH / 2)), int(dragDataStruct->mousePos_Y - (DRAG_DLG_HEIGHT / 2)), DRAG_DLG_WIDTH, DRAG_DLG_HEIGHT);

	ReleaseDragStruct();

	return true;
}

bool TimelineUIManager::TimelineDragUp()
{
	DragDataStruct* dragDataStruct = BringDragStruct();

	if (dragDataStruct == nullptr)
		return false;

	if (m_dragDlg == nullptr)
	{
		ReleaseDragStruct();
		return false;
	}


	// �ش� ��ġ�� ������� ���� ó���ؾ��ϴ� ����� �ٸ�
	// ���콺�� ����Ʈ�� ���� ���̾�αװ� �ƴҶ� ó��
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

	// ���̾�α׸� ã�� ���Ͽ��� ��
	if (iter1 == m_scenarioDlgManager.end())
	{
		m_dragState = DUS_NOTHING;
	}
	// ã�� ���̾�αװ� ���� ���̾�α��� ��
	else if (iter1->value.value == m_mainDlg->GetParent())
	{
		m_dragState = DUS_THIS;
		dragDataStruct->target_sceSEQ = -1;
		// Ÿ�Ӷ������� ����

		CRect rect;
		m_mainDlg->GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_THIS_TIMELINE;
		}
	}
	// ã�� ���̾�αװ� �ٸ� ���̾�α��� ��
	else
	{
		m_dragState = DUS_ANOTHER;

		// �ٸ� ���̾�α��� �� �ش� ���̾�α��� seq��ȣ�� ����ü�� ����
		ComplexMap<int, int>::iterator iter2 = m_scenarioSeqMap.find(iter1->value.key);
		if (iter2 != m_scenarioSeqMap.end())
			dragDataStruct->target_sceSEQ = iter2->value.value;

		CRect rect;
		iter1->value.value->m_timeline.GetWindowRect(rect);
		if (PtInRect(rect, pt))
		{
			m_dragState = DUS_ANOTHER_TIMELINE;
		}
	}

	m_dragDlg->ShowWindow(SW_HIDE);
	ReleaseDragStruct();
	CursorCountRestore(0);
	CURSOR_ARROW;
	ReleaseCapture();
	return true;
}

bool TimelineUIManager::NoteInsert()
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

	if (iter->value.value->SignalUpdateSetTIME(dragDataStruct->noteSEQ, false) == false)
	{
		ReleaseDragStruct();
		return false;
	}

	ReleaseDragStruct();

	return true;
}
