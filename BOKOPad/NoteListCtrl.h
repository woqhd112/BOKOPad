#pragma once
#include "ComplexMap.h"
#include "ScrollProcess.h"
#include "NoteListInterface.h"

class NoteManager;
// NoteListCtrl 대화 상자

#define BUTTON_COLOR RGB(68, 68, 68)

class NoteListCtrl : public CDialogEx, public NoteListInterface
{
	DECLARE_DYNAMIC(NoteListCtrl)

public:

	NoteListCtrl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NoteListCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NOTE_LIST_CTRL };

	void LoadNoteInformation();

	void ScrollExecute(bool bAdd, bool bPosSwitch = false);
	CRect* CalcNotePosition(int itemIndex);
	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);
	bool InsertNote(ComplexString inpusString);
	bool UpdateSetTIME(int notSEQ, int noteIndex);
	bool DeleteNote(int notSEQ);
	bool MoveNote(int startMoveIndex, int endMoveIndex);

private:

	bool UpdateScenarioList(NoteInformationVO* noteInform);

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

public:

	ScrollProcess scroll;
	
private:

	ComplexVector<NoteInformationVO>* m_noteInformationContainer;
	NoteManager* m_noteManager;
	bool m_bMainScrollFocus;

	CRect m_calculateItemPos;

	const int EDIT_WIDTH = 80;
	const int EDIT_HEIGHT = 60;

	int m_noteSize;
	ScenarioManagerStruct m_thisDataStruct;
	DragDataStruct m_defaultDragData;

	CButton* m_downButton;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
