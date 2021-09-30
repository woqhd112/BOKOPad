#pragma once
#include "ComplexMap.h"
#include "ScenarioListVO.h"
#include "NoteInformationVO.h"
#include "ProcessStage.h"

#ifdef _DEBUG
#pragma comment(lib, "ComplexScenarioProcessModule_dbg.lib")
#else
#pragma comment(lib, "ComplexScenarioProcessModule.lib")
#endif

using namespace ComplexLibrary;

class BOKOScenarioDetailDlg;
class NoteListCtrl;

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
	PM_DLG_ATTACH
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
	NoteInformationVO* noteData;	// 저장할 노트 데이터
	int noteIndex;	// 노트 리스트의 인덱스 번호
	CRect* noteRect;
};

class ManagerManagement
{
public:

	ManagerManagement();
	~ManagerManagement();

	void AttachManager(CWnd* dlgAttachPointer);
	CWnd* DetachManager();

	void InputScenarioStruct(ScenarioManagerStruct* scenarioDataStruct);
	void InputNoteStruct(NoteManagerStruct* noteDataStruct);

protected:

	virtual bool SendMessages(PerformanceMessage message) = 0;
	virtual bool HelpInvoker(PerformanceMessage message) = 0;


	bool m_bAttach;
	CWnd* m_mainDlg;

	ScenarioManagerStruct* m_pPutScenarioStruct;
	NoteManagerStruct* m_pPutNoteStruct;

	StageManager* m_pStageManager;

};

