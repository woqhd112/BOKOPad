#pragma once
#include "ComplexMap.h"
#include "ScenarioListVO.h"
#include "NoteInformationVO.h"
#include "DragProcess.h"
#include <memory>

using namespace ComplexLibrary;

class BOKOScenarioDetailDlg;
class NoteListCtrl;
class BOKODragDlg;

// 0 ~ 4 -> scenario ddl event message
// 5 ~ 11 -> scenario dlg event message
// 12 ~ -> note event message
enum PerformanceMessage
{
	PM_CREATE = 0,
	PM_DESTROY,
	PM_SHOW,
	PM_HIDE,
	PM_EXIST,
	PM_DRAG_MOVE,
	PM_DRAG_DOWN,
	PM_DRAG_UP,
	PM_TIMELINE_ATTACH,
	PM_TIMELINE_DETACH,
	PM_TIMELINE_CONTACT_GRIDLINE,
	PM_TIMELINE_NOT_CONTACT_GRIDLINE,
	PM_NOTE_INSERT,
	PM_NOTE_DELETE,
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
	ScenarioListVO scenarioData;	// 저장할 시나리오 데이터
	int scenarioIndex;	// 시나리오 리스트의 인덱스 번호
};

struct NoteManagerStruct
{
	NoteManagerStruct()
	{

	}
	NoteManagerStruct(NoteInformationVO* noteData, CRect* noteRect, int noteIndex = -1)
		: noteData(noteData)
		, noteIndex(noteIndex)
		, noteRect(noteRect)
	{

	}
	int noteIndex;	// 노트 리스트의 인덱스 번호
	NoteInformationVO* noteData;	// 저장할 노트 데이터
	CRect* noteRect;
};

struct DragDataStruct
{
	DragDataStruct()
	{
	
	}
	DragDataStruct(int sceIndex, int noteSEQ, int sceSEQ, int noteIndex, int buttonID, ComplexString noteCONTENT, int mousePos_X, int mousePos_Y)
		: sceIndex(sceIndex)
		, noteSEQ(noteSEQ)
		, sceSEQ(sceSEQ)
		, noteIndex(noteIndex)
		, noteCONTENT(noteCONTENT)
		, buttonID(buttonID)
		, mousePos_X(mousePos_X)
		, mousePos_Y(mousePos_Y)
	{

	}

	int sceSEQ;
	int sceIndex;
	int noteSEQ;
	int noteIndex;

	int buttonID;
	int mousePos_X;
	int mousePos_Y;

	ComplexString noteCONTENT;
};

class ManagerManagement
{
public:

	ManagerManagement();
	~ManagerManagement();

	void AttachManager(CWnd* dlgAttachPointer);
	CWnd* DetachManager();

	void InputDragStruct(DragDataStruct* dragDataStruct);
	void InputScenarioStruct(ScenarioManagerStruct* scenarioDataStruct);
	void InputNoteStruct(NoteManagerStruct* noteDataStruct);


protected:

	virtual bool SendMessages(PerformanceMessage message) = 0;
	virtual bool HelpInvoker(PerformanceMessage message) = 0;

	ScenarioManagerStruct* BringScenarioStruct() const;
	void ReleaseScenarioStruct();

	DragDataStruct* BringDragStruct() const;
	void ReleaseDragStruct();

	NoteManagerStruct* BringNoteStruct() const;
	void ReleaseNoteStruct();


	bool m_bAttach;
	CWnd* m_mainDlg;
	BOKODragDlg* m_dragDlg;
	ComplexLock m_processLock;
	static std::shared_ptr<DragProcess> m_dragProc;

private:

	ScenarioManagerStruct* m_pPutScenarioStruct;
	NoteManagerStruct* m_pPutNoteStruct;
	DragDataStruct* m_pPutDragStruct;

};

