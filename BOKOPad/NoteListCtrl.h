#pragma once
#include "ComplexMap.h"
#include "NoteListInterface.h"
#include "DlgInterface.h"

class NoteDBManager;
class NoteUIManager;
// NoteListCtrl 대화 상자

#define BUTTON_COLOR RGB(68, 68, 68)

class NoteListCtrl : public CDialogEx, public NoteListInterface, public DlgInterface
{
	friend class NoteUIManager;
	friend class BOKOScenarioDetailDlg;
	DECLARE_DYNAMIC(NoteListCtrl)

public:

	NoteListCtrl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NoteListCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NOTE_LIST_CTRL };


private:

	// 시그널 함수
	bool LoadNoteInformation();
	CRect* CalcNotePosition(int itemIndex);
	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);
	bool InsertNote(ComplexString inpusString, bool bNoteShow);
	bool UpdateSetTIME(int notSEQ);
	bool DeleteNote(int notSEQ);
	bool MoveNote();
	bool DeleteAllItems();
	bool LoadDraggingNote();
	bool UnloadDraggingNote();
	bool CheckDeleteNote();

	// 내부 함수
	void ScrollExecute(bool bAdd, bool bPosSwitch = false);
	bool UpdateScenarioList(NoteInformationVO* noteInform);

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

public:

	ScrollProcess scroll;
	
private:

	ComplexVector<NoteInformationVO>* m_noteInformationContainer;
	NoteUIManager* m_noteUIManager;
	NoteDBManager* m_noteDBManager;
	bool m_bMainScrollFocus;

	CRect m_calculateItemPos;

	int m_viewNoteSize;
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
