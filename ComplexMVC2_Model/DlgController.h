#pragma once
#include "MainDlgService.h"
#include "ScenarioListDlgService.h"
#include "NoteInformationDlgService.h"
#include "TimelineDlgService.h"


class COMPLEXMVC2_MODEL_DLL DlgController
{
public:

	DlgController();
	~DlgController();

	// pad option control
	// �ɼ����� �ε�
	bool SelectAllPadOption();
	// �ɼ����� ����
	bool UpdatePadOption();
	// �ó����� ����Ʈ �ڵ����� ������ ����
	bool UpdateScenarioListAutoIncrementSeq();
	// ��Ʈ ���� �ڵ����� ������ ����
	bool UpdateNoteInformationAutoIncrementSeq();

	// scenario list control
	// �ó����� ����Ʈ ���
	bool InsertScenarioList();
	// �ó����� ����Ʈ ��� �ε�
	bool SelectAllScenarioList();
	// Ư�� �ó����� ����Ʈ ����
	bool UpdateScenarioList();
	// Ư�� �ó����� ����Ʈ ����
	bool DeleteScenarioList();
	// Ư�� �ó����� ����Ʈ Ÿ��Ʋ���� ����
	bool UpdateScenarioListInSceTITLE();
	// Ư�� �ó����� ����Ʈ ��Ʈ���� ����
	bool UpdateScenarioListInNotCNT();
	// Ư�� �ó������� �ó����� Ÿ��Ʋ�� �ε�
	bool SelectInSceSEQScenarioListInSceTITLE();

	// note information control
	// �ó����� �������� ��Ʈ���� �ε�
	bool SelectInSceSEQNoteInformation();
	// ��Ʈ �������� �ϳ��� Ư�� ��Ʈ���� �ε�
	bool SelectOneNoteInformation();
	// ��Ʈ���� ���
	bool InsertNoteInformation();
	// ��Ʈ���� ����
	bool UpdateNoteInformation();
	// ��Ʈ���� ����
	bool DeleteNoteInformation();
	// ��Ʈ�������� ��Ʈ ���� ����
	bool UpdateNoteInformationInNotContent();
	// ��Ʈ�������� ��� ���� ����
	bool UpdateNoteInformationInNotLOCK();
	// ��Ʈ�������� �ó����� ������ ����
	bool UpdateNoteInformationInSceSEQ();
	// ��Ʈ�������� Ÿ�Ӷ��� ���� ����
	bool UpdateNoteInformationInSetTIMELINE();

	// time line control
	// �ó����� �������� Ÿ�Ӷ������� �ε�
	bool SelectInSceSEQTimeline();
	// Ÿ�Ӷ������� ���
	bool InsertTimeline();
	// Ÿ�Ӷ������� ����
	bool DeleteTimeline();
	// Ÿ�Ӷ������� timeIDX +1 ����
	bool UpdateTimelineInTimeIDXPlus();
	// Ÿ�Ӷ������� timeIDX -1 ����
	bool UpdateTimelineInTimeIDXMinus();
	// Ÿ�Ӷ������� timeIDX ����
	bool UpdateTimelineInTimeIDX();
	// ��Ʈ �������� Ÿ�Ӷ������� �ε�
	bool SelectInTimeIDXTimelineInNotSEQ();

private:

	MainDlgService* m_mainDlgService;
	ScenarioListDlgService* m_scenarioListDlgService;
	NoteInformationDlgService* m_noteInformationDlgService;
	TimelineDlgService* m_timeLineDlgService;

};

