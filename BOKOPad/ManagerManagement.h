#pragma once
#include "ComplexMap.h"
#include "ScenarioListVO.h"
#include "NoteInformationVO.h"
#include "ComplexLock.h"
#include <memory>

using namespace ComplexLibrary;

class BOKOScenarioDetailDlg;
class NoteListCtrl;
class BOKODragDlg;

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
	PM_DRAG_MOVE,
	PM_DRAG_DOWN,
	PM_DRAG_UP,
	PM_DRAG_NOTHING,
	PM_DRAG_THIS_ATTACH,
	PM_DRAG_ANOTHER_ATTACH,
	PM_DRAG_THIS_TIMELINE_ATTACH,
	PM_DRAG_ANOTHER_TIMELINE_ATTACH,
	PM_TIMELINE_CONTACT_GRIDLINE,
	PM_TIMELINE_NOT_CONTACT_GRIDLINE,
	PM_NOTE_INSERT,
	PM_NOTE_DELETE,
	PM_NOTE_MOVE,
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
	ScenarioListVO scenarioData;	// ������ �ó����� ������
	int scenarioIndex;	// �ó����� ����Ʈ�� �ε��� ��ȣ
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
	int noteIndex;	// ��Ʈ ����Ʈ�� �ε��� ��ȣ
	NoteInformationVO* noteData;	// ������ ��Ʈ ������
	CRect* noteRect;
};

struct DragDataStruct
{
	DragDataStruct()
	{
	
	}
	DragDataStruct(int target_sceSEQ, int sceIndex, int noteSEQ, int sceSEQ, int noteIndex, int buttonID, ComplexString noteCONTENT, int mousePos_X, int mousePos_Y)
		: target_sceSEQ(target_sceSEQ)
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

	DragUpState GetDragState() const;

protected:

	struct NotePadStruct
	{
		CEdit* noteEdit;
		CButton* noteButton;
	};

	virtual bool SendMessages(PerformanceMessage message) = 0;
	virtual bool HelpInvoker(PerformanceMessage message) = 0;

	ScenarioManagerStruct* BringScenarioStruct() const;
	void ReleaseScenarioStruct();

	DragDataStruct* BringDragStruct() const;
	void ReleaseDragStruct();

	NoteManagerStruct* BringNoteStruct() const;
	void ReleaseNoteStruct();

	void SortScenarioManagerKey();
	void SortNoteManagerKey();


	bool m_bAttach;
	CWnd* m_mainDlg;
	BOKODragDlg* m_dragDlg;
	ComplexLock m_processLock;

	static ComplexMap<int, BOKOScenarioDetailDlg*> m_scenarioDlgManager;
	static ComplexMap<int, int> m_scenarioSeqMap;

	ComplexMap<int, NotePadStruct> m_notePadManager;
	ComplexMap<int, int> m_noteSeqMap;

	DragUpState m_dragState;

private:

	ScenarioManagerStruct* m_pPutScenarioStruct;
	NoteManagerStruct* m_pPutNoteStruct;
	DragDataStruct* m_pPutDragStruct;

};
