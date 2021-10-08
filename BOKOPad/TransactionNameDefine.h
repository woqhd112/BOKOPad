#pragma once

enum TRANSACTION_NAME_DEFINE
{
	TND_SCENARIO_CREATE = 0,
	TND_SCENARIO_DELETE,
	TND_SCENARIO_UPDATE,
	TND_NOTE_INSERT,
	TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL,
};

// 세이브 포인트명은 공백포함 안됨
static const char* TransactionNames[] = {
										"Scenario_Create",
										"Scenario_Delete",
										"Scenario_Update",
										"Note_Insert",
										"Drag_Event_Note_Another_Signal",
										};