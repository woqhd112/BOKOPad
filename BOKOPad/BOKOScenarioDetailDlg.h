#pragma once
#include "resource.h"
#include "NoteListCtrl.h"
#include "Timeline.h"

// BOKOScenarioDetailDlg 대화 상자

#define LIMIT_TEXT_FORMAT "%d / 500"
#define CAST_INT(x) int((x)) 

class BOKOScenarioDetailDlg : public CDialogEx
{
	// 타임라인 접근권한
	friend class NoteManager;

	DECLARE_DYNAMIC(BOKOScenarioDetailDlg)

public:
	BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOScenarioDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_TIMELINE };

public:

	void SignalLoadScenarioList();

	void SignalInsertTimeline(int notSEQ, POINT currentMPoint);
	void SignalInsertNote(ComplexString& strNoteContent);
	void SignalDeleteNote(int notSEQ);
	void SignalUpdateSetTIME(int notSEQ, int noteIndex);

private:

	void Initialize();

private:

	CEdit m_edit_note_input;
	CButton m_btn_note_input;
	CStatic m_stt_note_limit_size;
	NoteListCtrl m_list_notePad;
	Timeline m_timeline;

	bool m_bAddOnButton;

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
};
