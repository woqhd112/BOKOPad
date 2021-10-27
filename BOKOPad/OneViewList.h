#pragma once
#include "ScrollProcess.h"
#include "NoteListInterface.h"

// OneViewList 대화 상자
class TimelineUIManager;
class TimelineDBManager;

#define BASE_BK_COLOR RGB(240, 240, 240)
#define DRAG_BK_COLOR RGB(227, 214, 231)

class OneViewList : public CDialogEx, public NoteListInterface
{
	// ctrl 버튼 이벤트 참조
	friend class BOKOTimelineOneViewDlg;
	DECLARE_DYNAMIC(OneViewList)

public:
	OneViewList(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OneViewList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ONE_VIEW_LIST };

	void AttachManager(TimelineUIManager* uimanager, TimelineDBManager* dbmanager);
	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);

	void InsertItem(ComplexString strText);
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

private:

	struct OneViewListDataStruct
	{
		bool expandedEdit;
		int start_tagButton_pos_Y;
		CButton* tagButton;
		CEdit* editButton;
	};

	TimelineUIManager* m_timelineUIManager;

	TimelineDBManager* m_timelineDBManager;

	// 현재 타임라인이 사용중인 시나리오
	ScenarioManagerStruct m_thisDataStruct;

	// key : 인덱스
	// value : 리스트 ui컨트롤
	ComplexMap<int, OneViewListDataStruct> m_dataMap;
	ScrollProcess scroll;
	CFont m_buttonFont;
	CFont m_editFont;

	int m_size;

	int m_variableItemStart_Y;

	bool m_bClickEvent;

	const int TAG_BUTTON_HEIGHT = 20;
	const int EXPAND_EDIT_HEIGHT = 60;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
