#pragma once
#include "ScrollProcess.h"
#include "NoteListInterface.h"
#include "ComplexThread.h"
#include "ComplexCondition.h"
#include "ComplexLock.h"

// OneViewList 대화 상자
class NoteDBManager;

class OneViewList : public CDialogEx, public NoteListInterface, public ComplexThread
{
	DECLARE_DYNAMIC(OneViewList)

public:
	OneViewList(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OneViewList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ONE_VIEW_LIST };

	void AttachManager(NoteDBManager* dbmanager);
	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);

	void InsertItem(ComplexString strText, int notSEQ);
	void SetItem(int index, ComplexString strText);
	void DeleteItem(int index);
	void DeleteAllItems();
	int GetItemCount() const;
	void ExpandAll(bool bExpand);

	void GetFullItemText(ComplexString* strText);


protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

	virtual void Run();

	ComplexLock m_timerLock;
	ComplexCondition m_cond;
	bool m_bDragTimer;
	int m_nDragTime;

private:

	enum MousePointRectPositionStatus
	{
		MPRPS_BUTTON_NONE = 0,
		MPRPS_BUTTON_TOP,
		MPRPS_BUTTON_TOP_MIDDLE,
		MPRPS_BUTTON_MIDDLE_BOTTOM,
		MPRPS_BUTTON_BOTTOM
	};

	struct OneViewListDataStruct
	{
		bool expandedEdit;
		int start_tagButton_pos_Y;
		int notSEQ;
		CButton* tagButton;
		CEdit* editButton;
	};

	NoteDBManager* m_noteDBManager;

	// 현재 타임라인이 사용중인 시나리오
	ScenarioManagerStruct m_thisDataStruct;

	// key : 인덱스
	// value : 리스트 ui컨트롤
	ComplexMap<int, OneViewListDataStruct> m_dataMap;
	ScrollProcess scroll;
	CFont m_buttonFont;
	CFont m_editFont;

	CButton* m_downButton;
	int m_downButtonKey;
	int m_focusButtonKey;
	MousePointRectPositionStatus m_mprps;

	int m_size;
	int m_prePos;
	int m_variableItemStart_Y;

	const int TAG_BUTTON_HEIGHT = 20;
	const int EXPAND_EDIT_HEIGHT = 60;

	void SortDataMapByMPRPStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
