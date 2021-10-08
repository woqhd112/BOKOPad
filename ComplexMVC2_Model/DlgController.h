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
	// 옵션정보 로드
	bool SelectAllPadOption();
	// 옵션정보 수정
	bool UpdatePadOption();
	// 시나리오 리스트 자동증가 시퀀스 리셋
	bool UpdateScenarioListAutoIncrementSeq();
	// 노트 정보 자동증가 시퀀스 리셋
	bool UpdateNoteInformationAutoIncrementSeq();

	// scenario list control
	// 시나리오 리스트 등록
	bool InsertScenarioList();
	// 시나리오 리스트 모두 로드
	bool SelectAllScenarioList();
	// 특정 시나리오 리스트 수정
	bool UpdateScenarioList();
	// 특정 시나리오 리스트 삭제
	bool DeleteScenarioList();
	// 특정 시나리오 리스트 타이틀정보 수정
	bool UpdateScenarioListInSceTITLE();
	// 특정 시나리오 리스트 노트개수 수정
	bool UpdateScenarioListInNotCNT();
	// 특정 시나리오를 시나리오 타이틀로 로드
	bool SelectInSceSEQScenarioListInSceTITLE();

	// note information control
	// 시나리오 시퀀스로 노트정보 로드
	bool SelectInSceSEQNoteInformation();
	// 노트 시퀀스로 하나의 특정 노트정보 로드
	bool SelectOneNoteInformation();
	// 노트정보 등록
	bool InsertNoteInformation();
	// 노트정보 수정
	bool UpdateNoteInformation();
	// 노트정보 삭제
	bool DeleteNoteInformation();
	// 노트정보에서 노트 내용 수정
	bool UpdateNoteInformationInNotContent();
	// 노트정보에서 잠금 내용 수정
	bool UpdateNoteInformationInNotLOCK();
	// 노트정보에서 시나리오 시퀀스 수정
	bool UpdateNoteInformationInSceSEQ();
	// 노트정보에서 타임라인 설정 수정
	bool UpdateNoteInformationInSetTIMELINE();

	// time line control
	// 시나리오 시퀀스로 타임라인정보 로드
	bool SelectInSceSEQTimeline();
	// 타임라인정보 등록
	bool InsertTimeline();
	// 타임라인정보 삭제
	bool DeleteTimeline();
	// 타임라인정보 timeIDX +1 수정
	bool UpdateTimelineInTimeIDXPlus();
	// 타임라인정보 timeIDX -1 수정
	bool UpdateTimelineInTimeIDXMinus();
	// 타임라인정보 timeIDX 변경
	bool UpdateTimelineInTimeIDX();
	// 노트 시퀀스로 타임라인정보 로드
	bool SelectInTimeIDXTimelineInNotSEQ();

private:

	MainDlgService* m_mainDlgService;
	ScenarioListDlgService* m_scenarioListDlgService;
	NoteInformationDlgService* m_noteInformationDlgService;
	TimelineDlgService* m_timeLineDlgService;

};

