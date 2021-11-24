#pragma once
#include "resource.h"
#include "NoteListCtrl.h"
#include "Timeline.h"
#include "DlgInterface.h"

// BOKOScenarioDetailDlg 대화 상자

class BOKOScenarioDetailDlg : public CDialogEx, public DlgInterface
{
	// 타임라인 접근권한
	friend class TimelineUIManager;
	// 타임라인 접근권한
	friend class NoteUIManager;
	// 드래그모드 접근권한
	friend class ScenarioUIManager;

	DECLARE_DYNAMIC(BOKOScenarioDetailDlg)

public:
	BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOScenarioDetailDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_TIMELINE };

public:

	bool SignalLoadScenarioList();

	bool SignalInsertTimeline(int notSEQ, POINT currentMPoint);
	bool SignalInsertNote(ComplexString strNoteContent, bool bNoteShow);
	bool SignalDeleteNote(int notSEQ);
	bool SignalUpdateSetTIME(int notSEQ, bool setTIME);
	bool SignalReloadNoteList();
	bool SignalReloadTimeline();
	void SignalTimelineOneViewRefresh();
	int SignalGetTimelineCount();

private:

	void SetCtrlByTimelineCount();

protected:

	virtual void Initialize();
	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

private:

	//CEdit m_edit_note_input;
	CustomEditCtrl m_edit_note_input;

	CustomButton m_btn_note_input;
	CButton m_btn_drag_mode;
	CustomButton m_btn_note_delete;
	CStatic m_stt_note_limit_size;
	NoteListCtrl m_list_notePad;
	Timeline m_timeline;
	CStatic m_stt_timeline_count;
	CProgressCtrl m_progress_timeline_count;

	bool m_bDragModeCheck;
	bool m_bKeyDownEvent;

	ScenarioManagerStruct m_thisDataStruct;
	CString m_strTitleText;

	CFont m_setFont;

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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
