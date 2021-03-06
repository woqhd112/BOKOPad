#pragma once
#include "ComplexMap.h"
#include "Model/VO/ScenarioListVO.h"
#include "Model/VO/NoteInformationVO.h"
#include "Model/VO/TimelineVO.h"
#include "..\..\CustomEdit.h"
#include "..\..\CustomEditCtrl.h"
#include "..\..\CustomButton.h"
//#include <memory>

using namespace ComplexLibrary;

class BOKOScenarioDetailDlg;
class NoteListCtrl;
class BOKODragDlg;

#define CURSOR_HAND SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND))
#define CURSOR_ARROW SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW))
#define CURSOR_CROSS SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS))
#define CURSOR_HAND SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND))
#define CURSOR_WAIT SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT))
#define EDIT_WIDTH 120
#define EDIT_HEIGHT 100


enum DragUpState
{
	DUS_NOTHING = 0,
	DUS_THIS,
	DUS_ANOTHER,
	DUS_THIS_TIMELINE,
	DUS_ANOTHER_TIMELINE
};

enum PerformanceMessage
{
	PM_CREATE = 0,
	PM_DESTROY,
	PM_SHOW,
	PM_HIDE,
	PM_EXIST,
	PM_SCENARIO_CLEAR,
	PM_THIS_SCENARIO_ONE_VIEW_REFRESH,
	PM_TARGET_SCENARIO_ONE_VIEW_REFRESH,
	PM_DRAG_PROCESS_LOCK,
	PM_IS_DRAGGING_MODE,
	PM_LOAD_DRAGGING_MODE,
	PM_UNLOAD_DRAGGING_MODE,
	PM_NOTE_RELOAD,
	PM_DRAG_MOVE,
	PM_DRAG_DOWN,
	PM_DRAG_UP,
	PM_DRAG_OFF,
	PM_DRAG_NOTHING,
	PM_DRAG_THIS_ATTACH,
	PM_DRAG_ANOTHER_ATTACH,
	PM_DRAG_THIS_TIMELINE_ATTACH,
	PM_DRAG_ANOTHER_TIMELINE_ATTACH,
	PM_INSERT_NOTE_VIEW_UPDATE,
	PM_SEQ_NOTE_VIEW_UPDATE,
	PM_NOTE_CLICK,
	PM_NOTE_CLICK_RELEASE,
	PM_NOTE_INSERT,
	PM_NOTE_DELETE,
	PM_NOTE_MOVE,
	PM_NOTE_CHECK_DELETE,
	PM_TIMELINE_DRAG_DOWN,
	PM_TIMELINE_DRAG_MOVE,
	PM_TIMELINE_DRAG_UP,
	PM_FIND_NOTE_INDEX,
	PM_NOTE_SHOW,
	PM_NOTE_HIDE,
	PM_NOTE_CLEAR,
	PM_ROLLBACK_ANOTHER_ATTACH,
	PM_ROLLBACK_THIS_TIMELINE_ATTACH,
	PM_ROLLBACK_TIMELINE_ANOTHER_ATTACH,
	PM_ROLLBACK_THIS_ANOTHER_TIMELINE_ATTACH,
	PM_TIMELINE_RELOAD,
	PM_TIMELINE_ONEVIEW_EXPANDALL_CLOSE,
};

struct ScenarioManagerStruct
{
	ScenarioManagerStruct()
	{

	}
	ScenarioManagerStruct(ScenarioListVO scenarioData, int scenarioIndex = -1)
		: scenarioData(scenarioData)
		, scenarioIndex(scenarioIndex)
	{

	}
	ScenarioListVO scenarioData;	// ?????? ???????? ??????
	int scenarioIndex;	// ???????? ???????? ?????? ????
};

struct NoteManagerStruct
{
	NoteManagerStruct()
	{

	}
	NoteManagerStruct(NoteInformationVO* noteData, CRect* noteRect, bool noteView, int noteIndex = -1)
		: noteData(noteData)
		, noteView(noteView)
		, noteIndex(noteIndex)
		, noteRect(noteRect)
	{

	}
	bool noteView;
	int noteIndex;	// ???? ???????? ?????? ????
	NoteInformationVO* noteData;	// ?????? ???? ??????
	CRect* noteRect;
};

struct DragDataStruct
{
	DragDataStruct()
	{
	
	}
	DragDataStruct(bool pushShift, bool pushCtrl, int target_sceSEQ, int sceIndex, int noteSEQ, int sceSEQ, int noteIndex, int buttonID, ComplexString noteCONTENT, int mousePos_X, int mousePos_Y)
		: pushShiftButton(pushShift)
		, pushCtrlButton(pushCtrl)
		, target_sceSEQ(target_sceSEQ)
		, sceIndex(sceIndex)
		, noteSEQ(noteSEQ)
		, sceSEQ(sceSEQ)
		, noteIndex(noteIndex)
		, noteCONTENT(noteCONTENT)
		, buttonID(buttonID)
		, mousePos_X(mousePos_X)
		, mousePos_Y(mousePos_Y)
	{

	}

	bool pushShiftButton = false;
	bool pushCtrlButton = false;
	int target_sceSEQ = -1;
	int sceSEQ = -1;
	int sceIndex = -1;
	int noteSEQ = -1;
	int noteIndex = -1;

	int buttonID = -1;
	int mousePos_X = -1;
	int mousePos_Y = -1;

	ComplexString noteCONTENT;
};

class UIManagerManagement
{
public:

	UIManagerManagement();
	~UIManagerManagement();

	void AttachManager(CWnd* dlgAttachPointer);
	CWnd* DetachManager();

	void InputDragStruct(DragDataStruct* dragDataStruct);
	void InputScenarioStruct(ScenarioManagerStruct* scenarioDataStruct);
	void InputNoteStruct(NoteManagerStruct* noteDataStruct);

	DragUpState GetDragState() const;

protected:

	struct NotePadStruct
	{
		bool bOpenView;
		CustomEditCtrl* noteEdit;
		CustomButton* noteButton;
		CButton* noteCheckBox;
	};

	virtual bool SendMessages(PerformanceMessage message) = 0;
	virtual bool InvokeHelper(PerformanceMessage message) = 0;

	ScenarioManagerStruct* BringScenarioStruct() const;
	void ReleaseScenarioStruct();

	DragDataStruct* BringDragStruct() const;
	void ReleaseDragStruct();

	NoteManagerStruct* BringNoteStruct() const;
	void ReleaseNoteStruct();

	void SortScenarioManagerKey();
	void SortNoteManagerKey();

	void CursorCountRestore(int setCursorCnt);

	bool m_bAttach;
	CWnd* m_mainDlg;
	BOKODragDlg* m_dragDlg;

	static ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
	static ComplexMap<int, int> m_scenarioSeqMap;
	static DragUpState m_dragState;
	static int m_cursorEventCnt;
	static bool m_bIsDragging;

	ComplexMap<int, NotePadStruct> m_notePadManager;
	ComplexMap<int, int> m_noteSeqMap;

	CFont m_setFont;
	const int DRAG_DLG_WIDTH = 30;
	const int DRAG_DLG_HEIGHT = 30;

private:

	ScenarioManagerStruct* m_pPutScenarioStruct;
	NoteManagerStruct* m_pPutNoteStruct;
	DragDataStruct* m_pPutDragStruct;

};

