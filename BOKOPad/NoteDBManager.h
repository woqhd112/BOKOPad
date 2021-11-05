#pragma once
#include "DBManagerManagement.h"

class NoteDBManager : public DBManagerManagement
{
public:

	NoteDBManager();
	virtual ~NoteDBManager();

	bool SelectInSceSEQNoteInformation(int in_sceSEQ, ComplexVector<NoteInformationVO>* out);
	bool UpdateNoteInformationInSetTIMELINE(int in_notSEQ, bool in_setTIMELINE);
	bool InsertNoteInformation(NoteInformationVO& note);
	bool DeleteNoteInformation(int in_notSEQ);
	bool SelectOneNoteInformation(int in_notSEQ, NoteInformationVO* out);
	bool UpdateNoteInformationInNotContent(int in_notSEQ, ComplexString in_notCONTENT);

private:



};

