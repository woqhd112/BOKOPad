#pragma once
#include <WinUser.h>

class NoteListInterface
{
public:

	NoteListInterface()
		: m_bDragProcessing(false)
	{
	
	}

	~NoteListInterface()
	{
	
	}

protected:

	virtual bool DragDown(MSG* pMsg) = 0;
	virtual bool DragMove(MSG* pMsg) = 0;
	virtual bool DragUp(MSG* pMsg) = 0;

	bool m_bDragProcessing;

private:


};
