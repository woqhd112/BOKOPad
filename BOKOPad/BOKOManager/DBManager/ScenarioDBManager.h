#pragma once
#include "DBManagerManagement.h"

class ScenarioDBManager : public DBManagerManagement
{
public:

	ScenarioDBManager();
	virtual ~ScenarioDBManager();

	bool SelectAllPadOption(PadOptionVO* out);
	bool SelectAllScenarioList(ComplexVector<ScenarioListVO>* out);
	bool InsertScenarioList(ComplexString in);
	bool DeleteScenarioList(int in);
	bool UpdateScenarioListAutoIncrementSeq();
	bool SelectInSceSEQScenarioListInSceTITLE(ComplexString in_sceTITLE, ScenarioListVO* out);
	bool SelectInSceSEQNoteInformation(int in_sceSEQ, ComplexVector<NoteInformationVO>* out);
	bool InsertNoteInformation(NoteInformationVO& in);
	bool UpdateNoteInformationInNotContent(int in_notSEQ, ComplexString in_notCONTENT);
	bool DeleteAllScenarioList();

private:


};

