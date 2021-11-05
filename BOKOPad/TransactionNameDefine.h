#pragma once

enum TRANSACTION_NAME_DEFINE
{
	TND_SCENARIO_CREATE = 0,
	TND_SCENARIO_DELETE,
	TND_SCENARIO_UPDATE,
	TND_NOTE_INSERT,
	TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL,
	TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL,
	TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL,
	TND_DRAG_EVENT_TIMELINE_THIS_SIGNAL,
	TND_DRAG_EVENT_TIMELINE_ANOTHER_SIGNAL,
	TND_DRAG_EVENT_TIMELINE_THIS_TIMELINE_SIGNAL,
	TND_DRAG_EVENT_TIMELINE_ANOTHER_TIMELINE_SIGNAL,
	TND_NOTE_CHECK_DELETE,
	TND_TIMELINE_ONEVIEW_CHANGE,
};

// 세이브 포인트명은 공백포함 안됨
static const char* TransactionNames[] = {
										"Scenario_Create",
										"Scenario_Delete",
										"Scenario_Update",
										"Note_Insert",
										"Drag_Event_Note_Another_Signal",
										"Drag_Event_Note_This_Timeline_Signal",
										"Drag_Event_Note_Another_Timeline_Signal",
										"Drag_Event_Timeline_This_Signal",
										"Drag_Event_Timeline_Another_Signal",
										"Drag_Event_Timeline_This_Timeline_Signal",
										"Drag_Event_Timeline_Another_Timeline_Signal",
										"Note_Check_Delete",
										"Timeline_Oneview_Change",
										};