#pragma once
#include "Resource.h"
#include "TimelineVO.h"
#include "TimelineDetail.h"
#include "BOKOTimelineOneViewDlg.h"
#include "NoteListInterface.h"

class NoteUIManager;
class NoteDBManager;
class TimelineDBManager;
class TimelineUIManager;
// Timeline 대화 상자

#define LINE_COLOR RGB(0, 0, 0)

class Timeline : public CDialogEx, public NoteListInterface
{
	friend class TimelineUIManager;
	friend class NoteUIManager;
	friend class BOKOScenarioDetailDlg;
	DECLARE_DYNAMIC(Timeline)

public:
	Timeline(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Timeline();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMELINE };

public:

	enum TimelineThickApproch
	{
		TTA_NOTHING = 0,
		TTA_WELL_APPROCH,
		TTA_NOTE_BY_TIMELINE_DRAG_EVENT_APPROCH,
		TTA_TIMELINE_BY_TIMELINE_DRAG_EVENT_APPROCH
	};
	
	void SetExpandCloseEvent();

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

private:

	void AttachNoteManager(NoteUIManager* manager, NoteDBManager* dbmanager);

	// 시그널 함수
	bool SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);
	bool InsertTimeline(int notSEQ, POINT currentMPoint);
	bool ThickEventTimeline(int notSEQ, POINT pt, TimelineThickApproch timelineThickApprochState);
	bool ReloadTimeline();
	void HideTimelineDetail();

	// 내부 함수
	bool TimelineOneViewProcess();
	int ValidatePointToRectIDX(POINT pt);
	bool UpdateTimelineIDX(int startUpdateTimeIDX);
	void OneViewRefresh();

	bool m_bDetailOpen;
	bool m_bAttachManagerInit;
	bool m_bInit;
	// 현재 타임라인이 사용중인 시나리오
	ScenarioManagerStruct m_thisDataStruct;

	// 현재 타임라인 다이얼로그의 사각형
	CRect m_thisRect;
	// 타임라인 메인 선의 사각형 
	CRect m_timeLineRect;
	// 타임라인 메인 선을 그릴때 사용되는 펜
	CPen m_linePen;
	// 일반 타임라인 데이터를 그릴때 사용되는 펜
	CPen m_drawPen;
	// 마우스 댔을 때 타임라인 데이터를 그릴때 사용되는 펜
	CPen m_drawHoverPen;

	TimelineThickApproch m_timelineThickApprochState;

	// 드래그 처리할때 사용되는 데이터
	DragDataStruct m_defaultDragData;

	// 타임라인 상세보기 다이얼로그 (마우스 포인트로 타임라인 데이터쪽에 다가갔을 때 생성되는 다이얼로그)
	TimelineDetail m_detailDlg;
	// 한눈에보기 다이얼로그
	BOKOTimelineOneViewDlg m_oneViewDlg;
	// 타임라인 UI매니저
	TimelineUIManager* m_timeUIManager;
	// 노트 UI매니저
	NoteUIManager* m_noteUIManager;
	// 타임라인 DB매니저
	TimelineDBManager* m_timeDBManager;

	// 타임라인 x축 선의 시작지점
	int m_nLineStartPoint_X;
	// 타임라인 x축 선의 끝지점
	int m_nLineEndPoint_X;
	// 타임라인의 길이
	int m_nDataVariableWidth;
	// 타임라인의 y축 지점
	int m_nLinePoint_Y;

	// 마우스 커서가 가르키고있는 타임라인 선 지점의 timeIDX값
	int m_nPointingTimeIDX;

	// 타임라인의 양 끝지점의 세로 선 길이(각각)
	const int LINE_HEIGHT = 8;
	// 타임라인의 데이터가 들어간 세로 선 길이
	const int DATA_HEIGHT = 6;

	// 타임라인 정보를 담고있는 컨테이너
	ComplexVector<TimelineVO> m_timeLineContainer;

	// 타임라인의 데이터 세로선의 rect 값을 담고있는 컨테이너
	// key : timeIDX, value : rect
	ComplexMap<int, CRect> m_timeLinePointMap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
