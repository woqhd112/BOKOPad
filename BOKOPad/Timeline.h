#pragma once
#include "ComplexThread.h"
#include "ComplexCondition.h"
#include "ComplexLock.h"
#include "TimelineVO.h"
#include "TimelineDetail.h"

// Timeline 대화 상자

#define LINE_COLOR RGB(0, 0, 0)

class Timeline : public CDialogEx, public ComplexThread
{
	DECLARE_DYNAMIC(Timeline)

public:
	Timeline(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Timeline();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMELINE };

public:

	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);

	void InsertTimeline(int notSEQ, POINT currentMPoint);
	void ThickEventTimeline(int notSEQ, POINT pt, bool thisApproch = true);

protected:

	virtual void Run();

private:

	int ValidatePointToRect(POINT pt);
	void UpdateTimelineIDX(int startUpdateTimeIDX);

	bool m_bInit;
	ComplexCondition m_cond;

	ScenarioManagerStruct m_thisDataStruct;

	CRect m_thisRect;
	CRect m_timeLineRect;
	CPen m_linePen;
	CPen m_drawPen;
	CPen m_drawHoverPen;

	TimelineDetail m_detailDlg;

	int m_nLineStartPoint_X;
	int m_nLineEndPoint_X;
	int m_nDataVariableWidth;
	int m_nLinePoint_Y;

	int m_nTimelineCount;

	int m_nPointingTimeIDX;

	const int LINE_HEIGHT = 8;
	const int DATA_HEIGHT = 6;

	ComplexVector<TimelineVO> m_timeLineContainer;

	ComplexMap<int, CRect> m_timeLinePointMap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
