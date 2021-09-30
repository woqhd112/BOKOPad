#pragma once
#include "ManagerManagement.h"

class NoteManager : public ManagerManagement
{
public:

	NoteManager();
	virtual ~NoteManager();

	virtual bool SendMessages(PerformanceMessage message);

private:

	struct NotePadStruct
	{
		CEdit* noteEdit;
		CButton* noteButton;
	};

	virtual bool HelpInvoker(PerformanceMessage message);
	bool BringNoteStruct();
	void ReleaseNoteStruct();

	bool Insert();


private:

	int m_variableCtrlPos_x;
	int m_variableCtrlPos_y;
	const int EDIT_WIDTH = 80;
	const int EDIT_HEIGHT = 60;

	NoteManagerStruct* m_GetNoteStruct;
	ComplexMap<int, NotePadStruct> m_notePadManager;

};

