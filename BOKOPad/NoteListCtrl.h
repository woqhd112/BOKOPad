#pragma once
#include "ComplexMap.h"
#include "ScrollProcess.h"
#include "NoteManager.h"

// NoteListCtrl 대화 상자

#define BUTTON_COLOR RGB(68, 68, 68)

class NoteListCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(NoteListCtrl)

public:

	NoteListCtrl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NoteListCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NOTE_LIST_CTRL };

	void LoadNoteInformation();

	void SignalNoteInput(bool bPosSwitch = false);
	CRect* CalcNotePosition();
	void SetScenarioManagerStruct(ScenarioManagerStruct m_thisDataStruct);
	bool InsertNote(ComplexString inpusString);

private:

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

	bool m_bDragProcessing;


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
