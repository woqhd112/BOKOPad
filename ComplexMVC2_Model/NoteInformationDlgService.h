#pragma once
#include "NoteInformationDlgModelImp.h"

class NoteInformationDlgService
{
public:

	NoteInformationDlgService();
	~NoteInformationDlgService();

	bool SelectInSceSEQNoteInformation(int in_sceSEQ);
	bool SelectOneNoteInformation(int in_notSEQ);
	bool InsertNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_sceSEQ);
	bool UpdateNoteInformation(ComplexString in_notCONTENT, bool in_notLOCK, int in_notSEQ);
	bool DeleteNoteInformation(int in_notSEQ);
	bool UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ);
	bool UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ);
	bool UpdateNoteInformationInSceSEQ(int in_notSEQ, int in_sceSEQ);

private:

	NoteInformationDlgModelImp* m_noteInformationDlgModel;

};

