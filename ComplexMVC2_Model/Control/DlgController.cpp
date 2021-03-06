#include "pch.h"
#include "DlgController.h"


DlgController::DlgController()
	: m_mainDlgService(new MainDlgService)
	, m_scenarioListDlgService(new ScenarioListDlgService)
	, m_noteInformationDlgService(new NoteInformationDlgService)
	, m_timeLineDlgService(new TimelineDlgService)
{

}

DlgController::~DlgController()
{
	if (m_mainDlgService)
	{
		delete m_mainDlgService;
		m_mainDlgService = nullptr;
	}

	if (m_scenarioListDlgService)
	{
		delete m_scenarioListDlgService;
		m_scenarioListDlgService = nullptr;
	}

	if (m_noteInformationDlgService)
	{
		delete m_noteInformationDlgService;
		m_noteInformationDlgService = nullptr;
	}

	if (m_timeLineDlgService)
	{
		delete m_timeLineDlgService;
		m_timeLineDlgService = nullptr;
	}
}

bool DlgController::SelectAllPadOption()
{
	return m_mainDlgService->SelectAllPadOption();
}

bool DlgController::UpdatePadOption()
{
	PadOptionVO option;
	RequestScope->GetRequestAttributes(&option);
	return m_mainDlgService->UpdatePadOption(option.GetTest1(), option.GetTest2());
}

bool DlgController::UpdateScenarioListAutoIncrementSeq()
{
	return m_mainDlgService->UpdateScenarioListAutoIncrementSeq();
}

bool DlgController::UpdateNoteInformationAutoIncrementSeq()
{
	return m_mainDlgService->UpdateNoteInformationAutoIncrementSeq();
}

bool DlgController::InsertScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->InsertScenarioList(scenario.GetSceTITLE());
}

bool DlgController::SelectAllScenarioList()
{
	return m_scenarioListDlgService->SelectAllScenarioList();
}

bool DlgController::UpdateScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioList(scenario.GetSceTITLE(), scenario.GetNotCNT(), scenario.GetSceSEQ());
}

bool DlgController::DeleteScenarioList()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->DeleteScenarioList(scenario.GetSceSEQ());
}

bool DlgController::UpdateScenarioListInSceTITLE()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioListInSceTITLE(scenario.GetSceTITLE(), scenario.GetSceSEQ());
}

bool DlgController::UpdateScenarioListInNotCNT()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->UpdateScenarioListInNotCNT(scenario.GetNotCNT(), scenario.GetSceSEQ());
}

bool DlgController::SelectInSceSEQScenarioListInSceTITLE()
{
	ScenarioListVO scenario;
	RequestScope->GetRequestAttributes(&scenario);
	return m_scenarioListDlgService->SelectInSceSEQScenarioListInSceTITLE(scenario.GetSceTITLE());
}

bool DlgController::DeleteAllScenarioList()
{
	return m_scenarioListDlgService->DeleteAllScenarioList();
}

bool DlgController::SelectInSceSEQNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->SelectInSceSEQNoteInformation(note.GetSceSEQ());
}

bool DlgController::SelectOneNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->SelectOneNoteInformation(note.GetNotSEQ());
}

bool DlgController::InsertNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->InsertNoteInformation(note.GetNotCONTENT(), note.IsSetTIMELINE(), note.IsNotLOCK(), note.GetSceSEQ());
}

bool DlgController::UpdateNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformation(note.GetNotCONTENT(), note.IsSetTIMELINE(), note.IsNotLOCK(), note.GetNotSEQ());
}

bool DlgController::DeleteNoteInformation()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->DeleteNoteInformation(note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInNotContent()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInNotContent(note.GetNotCONTENT(), note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInNotLOCK()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInNotLOCK(note.IsNotLOCK(), note.GetNotSEQ());
}

bool DlgController::UpdateNoteInformationInSceSEQ()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInSceSEQ(note.GetNotSEQ(), note.GetSceSEQ());
}

bool DlgController::UpdateNoteInformationInSetTIMELINE()
{
	NoteInformationVO note;
	RequestScope->GetRequestAttributes(&note);
	return m_noteInformationDlgService->UpdateNoteInformationInSetTIMELINE(note.GetNotSEQ(), note.IsSetTIMELINE());
}

bool DlgController::SelectInSceSEQTimeline()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->SelectInSceSEQTimeline(timeline.GetSceSEQ());
}

bool DlgController::InsertTimeline()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->InsertTimeline(timeline.GetTimeIDX(), timeline.GetNotSEQ(), timeline.GetSceSEQ());
}

bool DlgController::DeleteTimeline()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->DeleteTimeline(timeline.GetNotSEQ(), timeline.GetSceSEQ());
}

bool DlgController::UpdateTimelineInTimeIDXPlus()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->UpdateTimelineInTimeIDXPlus(timeline.GetNotSEQ(), timeline.GetTimeIDX());
}

bool DlgController::UpdateTimelineInTimeIDXMinus()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->UpdateTimelineInTimeIDXMinus(timeline.GetNotSEQ(), timeline.GetTimeIDX());
}

bool DlgController::UpdateTimelineInTimeIDX()
{
	int updateTimeIDX;
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	RequestScope->GetRequestInt(&updateTimeIDX);
	return m_timeLineDlgService->UpdateTimelineInTimeIDX(timeline.GetNotSEQ(), timeline.GetTimeIDX(), updateTimeIDX);
}

bool DlgController::SelectInTimeIDXTimelineInNotSEQ()
{
	TimelineVO timeline;
	RequestScope->GetRequestAttributes(&timeline);
	return m_timeLineDlgService->SelectInTimeIDXTimelineInNotSEQ(timeline.GetNotSEQ());
}