#pragma once
#include "../Model/DAO/NoteInformationDlgModelImp.h"

class NoteInformationDlgService
{
public:

	NoteInformationDlgService();
	~NoteInformationDlgService();

	bool SelectInSceSEQNoteInformation(int in_sceSEQ);
	bool SelectOneNoteInformation(int in_notSEQ);
	bool InsertNoteInformation(ComplexString in_notCONTENT, bool in_setTIMELINE, bool in_notLOCK, int in_sceSEQ);
	bool UpdateNoteInformation(ComplexString in_notCONTENT, bool in_setTIMELINE, bool in_notLOCK, int in_notSEQ);
	bool DeleteNoteInformation(int in_notSEQ);
	bool UpdateNoteInformationInNotContent(ComplexString in_notCONTENT, int in_notSEQ);
	bool UpdateNoteInformationInNotLOCK(bool in_notLOCK, int in_notSEQ);
	bool UpdateNoteInformationInSceSEQ(int in_notSEQ, int in_sceSEQ);
	bool UpdateNoteInformationInSetTIMELINE(int in_notSEQ, bool in_setTIMELINE);

private:

	NoteInformationDlgModelImp* m_noteInformationDlgModel;

};

