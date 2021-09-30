#pragma once
#include "commonInclude.h"

struct DragData
{
	void Reset()
	{
		sceIndex = 0;
		noteSEQ = 0;
		sceSEQ = 0;
		noteCONTENT = "";
		mousePos_X = 0;
		mousePos_Y = 0;
	}

	int sceIndex;
	int noteSEQ;
	int sceSEQ;
	ComplexString noteCONTENT;

	int mousePos_X;
	int mousePos_Y;
};

class DragProcess
{
public:

	friend class StageManager;

	DragProcess();
	~DragProcess();

private:

	void CreateDragDlg(DragData* dragData);
	void DestroyDragDlg(DragData* dragData);
	void MovingDragDlg(DragData* dragData);

private:

};

