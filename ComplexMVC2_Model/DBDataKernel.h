#pragma once
#include "NoteInformationVO.h"
#include "ScenarioListVO.h"
#include "PadOptionVO.h"
#include "TimelineVO.h"

#ifdef COMPLEXMVC2_MODEL_EXPORTS
#define COMPLEXMVC2_MODEL_DLL __declspec(dllexport)
#else
#define COMPLEXMVC2_MODEL_DLL __declspec(dllimport)
#endif

class DBDataKernel
{
public:

	DBDataKernel();
	~DBDataKernel();

	void SetInt(int in_int);
	void SetString(ComplexString in_string);

	void SetNoteInformation(NoteInformationVO& in);
	void SetScenarioList(ScenarioListVO& in);
	void SetPadOption(PadOptionVO& in);
	void SetTimeline(TimelineVO& in);
	void SetNoteInformation(int notSEQ, int sceSEQ, bool setTIMELINE, bool notLOCK, ComplexString notCONTENT);
	void SetScenarioList(int sceSEQ, ComplexString sceTITLE, int notCNT);
	void SetPadOption(int test1, int test2);
	void SetTimeline(int timeIDX, int notSEQ, int sceSEQ);

	void GetInt(int* out_int);
	void GetString(ComplexString* out_string);

	void GetNoteInformation(NoteInformationVO* out, int get_row_index = 0);
	void GetScenarioList(ScenarioListVO* out, int get_row_index = 0);
	void GetPadOption(PadOptionVO* out, int get_row_index = 0);
	void GetTimeline(TimelineVO* out, int get_row_index = 0);
	void GetNoteInformation(int* out_notSEQ, int* out_sceSEQ, bool* out_setTIMELINE, bool* out_notLOCK, ComplexString* out_notCONTENT, int get_row_index = 0);
	void GetScenarioList(int* out_sceSEQ, int* out_notCNT, ComplexString* out_sceTITLE, int get_row_index = 0);
	void GetPadOption(int* out_test1, int* out_test2, int get_row_index = 0);
	void GetTimeline(int* out_timeIDX, int* out_notSEQ, int* out_sceSEQ, int get_row_index = 0);
	void GetNoteInformationContainer(ComplexVector<NoteInformationVO>* out_noteInformationVOContainer);
	void GetScenarioListContainer(ComplexVector<ScenarioListVO>* out_scenarioListVOContainer);
	void GetPadOptionContainer(ComplexVector<PadOptionVO>* out_padOptionVOContainer);
	void GetTimelineContainer(ComplexVector<TimelineVO>* out_timeLineVOContainer);

	void ResetInt();
	void ResetString();

	void ResetNoteInformation();
	void ResetScenarioList();
	void ResetPadOption();
	void ResetTimeline();

private:

	ComplexString m_string;
	int m_int;
	ComplexVector<NoteInformationVO> m_noteInformationVOContainer;
	ComplexVector<ScenarioListVO> m_scenarioListVOContainer;
	ComplexVector<PadOptionVO> m_padOptionVOContainer;
	ComplexVector<TimelineVO> m_timeLineContainer;

};

