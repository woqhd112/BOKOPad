﻿#pragma once
#include "resource.h"
#include "NoteListCtrl.h"
#include "Timeline.h"

// BOKOScenarioDetailDlg 대화 상자

#define LIMIT_TEXT_FORMAT "%d / 500"
#define CAST_INT(x) int((x)) 

class BOKOScenarioDetailDlg : public CDialogEx
{
	// 타임라인 접근권한
	friend class TimelineManager;
	// 타임라인 접근권한
	friend class NoteManager;
	// 드래그모드 접근권한
	friend class ScenarioManager;

	DECLARE_DYNAMIC(BOKOScenarioDetailDlg)

public:
	BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOScenarioDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_TIMELINE };

public:

	bool SignalLoadScenarioList();

	bool SignalInsertTimeline(int notSEQ, POINT currentMPoint);
	bool SignalInsertNote(ComplexString& strNoteContent, bool bNoteShow);
	bool SignalDeleteNote(int notSEQ);
	bool SignalUpdateSetTIME(int notSEQ);
	bool SignalReloadNoteList();
	bool SignalReloadTimeline();

private:

	void Initialize();

private:

	CEdit m_edit_note_input;
	CButton m_btn_note_input;
	CButton m_btn_drag_mode;
	CButton m_btn_note_delete;
	CStatic m_stt_note_limit_size;
	NoteListCtrl m_list_notePad;
	Timeline m_timeline;

	bool m_bDragModeCheck;
	bool m_bKeyDownEvent;

	ScenarioManagerStruct m_thisDataStruct;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditNoteInput();
	afx_msg void OnBnClickedButtonNoteInput();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckDragMode();
	afx_msg void OnBnClickedButtonNoteDelete();
};
