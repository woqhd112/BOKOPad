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

	bool Insert();
	bool DragMove();
	bool DragDown();
	bool DragUp();

private:

	int m_variableCtrlPos_x;
	int m_variableCtrlPos_y;
	const int EDIT_WIDTH = 80;
	const int EDIT_HEIGHT = 60;
	const int DRAG_DLG_WIDTH = 30;
	const int DRAG_DLG_HEIGHT = 30;

	ComplexMap<int, NotePadStruct> m_notePadManager;
	ComplexMap<int, int> m_noteSeqMap;

};

