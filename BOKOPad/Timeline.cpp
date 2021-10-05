// Timeline.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "Timeline.h"
#include "NoteManager.h"
#include "afxdialogex.h"


// Timeline 대화 상자

IMPLEMENT_DYNAMIC(Timeline, CDialogEx)

Timeline::Timeline(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE, pParent)
{
	m_bInit = false;
	m_nLineStartPoint_X = 0;
	m_nLineEndPoint_X = 0;
	m_nLinePoint_Y = 0;
	m_nDataVariableWidth = 0;
	m_nTimelineCount = 0;
	m_nPointingTimeIDX = -1;
	bDetailOpen = false;
}

Timeline::~Timeline()
{
	m_linePen.DeleteObject();
	m_drawPen.DeleteObject();
	m_drawHoverPen.DeleteObject();
}

void Timeline::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Timeline, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Timeline 메시지 처리기


BOOL Timeline::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_linePen.CreatePen(PS_SOLID, 3, LINE_COLOR);
	m_drawHoverPen.CreatePen(PS_SOLID, 2, LINE_COLOR);
	m_drawPen.CreatePen(PS_SOLID, 1, LINE_COLOR);

	m_detailDlg.Create(TimelineDetail::IDD, this);
	m_oneViewDlg.Create(BOKOTimelineOneViewDlg::IDD, this);
	m_detailDlg.MoveWindow(0, 0, 400, 200);
	m_detailDlg.ShowWindow(SW_HIDE);
	m_oneViewDlg.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void Timeline::AttachNoteManager(NoteManager* manager)
{
	m_noteManager = manager;
}

void Timeline::HideTimelineDetail()
{
	m_detailDlg.ShowWindow(SW_HIDE);
}

int Timeline::ValidatePointToRect(POINT pt)
{
	// 현재 커서위치가 타임라인 안에 존재할 때 진행.
	POINT point = pt;
	ScreenToClient(&point);
	if (PtInRect(m_timeLineRect, point) == FALSE)
		return -1;

	CRect prev(0, 0, 0, 0);
	ComplexMap<int, CRect>::iterator iter = m_timeLinePointMap.begin();
	while (iter != m_timeLinePointMap.end())
	{
		CRect rect = iter->value.value;

		// 커서위치가 개별 타임라인에 접근했을 때 그 인덱스를 리턴한다.
		if (PtInRect(rect, point))
		{
			return iter->value.key;
		}

		// 커서위치가 개별 타임라인 사이에 존재하면 사이의 뒷값 인덱스를 리턴한다.
		if ((prev.left < point.x) && (rect.left > point.x))
			return iter->value.key;

		prev = rect;
		iter++;
	}

	return -1;
}

void Timeline::UpdateTimelineIDX(int startUpdateTimeIDX)
{
	for (int i = startUpdateTimeIDX; i < m_timeLineContainer.size(); i++)
	{
		TimelineVO time;
		time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
		time.SetTimeIDX(i);
		RequestScope->SetRequestAttributes(time);
		if (MVC_Controller->UpdateTimelineInTimeIDX() == false)
			return;
	}
}

void Timeline::InsertTimeline(int notSEQ, POINT currentMPoint)
{
	int timeIDX = ValidatePointToRect(currentMPoint);
	if (timeIDX == -1)
		timeIDX = m_nTimelineCount;

	UpdateTimelineIDX(timeIDX);

	TimelineVO time;
	time.SetNotSEQ(notSEQ);
	time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	time.SetTimeIDX(timeIDX);
	RequestScope->SetRequestAttributes(time);
	if (MVC_Controller->InsertTimeline())
	{
		RequestScope->SetRequestAttributes(time);
		if (MVC_Controller->SelectInSceSEQTimeline())
		{
			m_timeLineContainer.clear();
			RequestScope->GetRequestAttributes(&m_timeLineContainer);
		}
		m_nTimelineCount = m_timeLineContainer.size();
		Invalidate();
	}
}

void Timeline::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	// 그리기 작업
	CPen* oldPen;

	// 막대라인 그리기
	oldPen = dc.SelectObject(&m_linePen);
	dc.MoveTo(m_nLineStartPoint_X, m_nLinePoint_Y);
	dc.LineTo(m_nLineEndPoint_X, m_nLinePoint_Y);

	dc.MoveTo(m_nLineStartPoint_X, m_nLinePoint_Y - LINE_HEIGHT);
	dc.LineTo(m_nLineStartPoint_X, m_nLinePoint_Y + LINE_HEIGHT);

	dc.MoveTo(m_nLineEndPoint_X, m_nLinePoint_Y - LINE_HEIGHT);
	dc.LineTo(m_nLineEndPoint_X, m_nLinePoint_Y + LINE_HEIGHT);

	dc.SelectObject(oldPen);

	if (m_bInit)
	{
		// 데이터라인 그리기
		int dataSize = m_timeLineContainer.size();

		// 100개이상은 못그림
		if (dataSize > 100 || dataSize <= 0)
			return;

		int dataInterval = int(m_nDataVariableWidth / (dataSize + 1));
		m_timeLinePointMap.clear();
		for (int i = 0; i < dataSize; i++)
		{
			if (m_nPointingTimeIDX == i)
				oldPen = dc.SelectObject(&m_drawHoverPen);
			else
				oldPen = dc.SelectObject(&m_drawPen);

			dc.MoveTo(m_nLineStartPoint_X + (dataInterval * (i + 1)), m_nLinePoint_Y - DATA_HEIGHT);
			dc.LineTo(m_nLineStartPoint_X + (dataInterval * (i + 1)), m_nLinePoint_Y + DATA_HEIGHT);
			CRect rect;
			rect.left = m_nLineStartPoint_X + (dataInterval * (i + 1)) - 2;
			rect.top = m_nLinePoint_Y - DATA_HEIGHT;
			rect.right = m_nLineStartPoint_X + (dataInterval * (i + 1)) + 2;
			rect.bottom = m_nLinePoint_Y + DATA_HEIGHT;
			int timeIDX = m_timeLineContainer.at(i).GetTimeIDX();
			m_timeLinePointMap.insert(timeIDX, rect);

			dc.SelectObject(oldPen);
		}

	}
}


void Timeline::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;

	GetClientRect(m_thisRect);
	m_nLineStartPoint_X = m_thisRect.left + 5;
	m_nLineEndPoint_X = m_thisRect.Width() - m_nLineStartPoint_X;
	m_nLinePoint_Y = int(m_thisRect.Height() / 2);
	m_nDataVariableWidth = m_nLineEndPoint_X - m_nLineStartPoint_X;

	m_timeLineRect.left = m_nLineStartPoint_X;
	m_timeLineRect.right = m_nLineEndPoint_X;
	m_timeLineRect.top = m_nLinePoint_Y - LINE_HEIGHT;
	m_timeLineRect.bottom = m_nLinePoint_Y + LINE_HEIGHT;


	TimelineVO time;
	time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	RequestScope->SetRequestAttributes(time);
	if (MVC_Controller->SelectInSceSEQTimeline())
	{
		m_bInit = true;
		RequestScope->GetRequestAttributes(&m_timeLineContainer);

	}
	m_nTimelineCount = m_timeLineContainer.size();
	Invalidate();
}

void Timeline::ThickEventTimeline(int notSEQ, POINT pts, bool thisApproch)
{
	// thisApproch는 true일때 마우스가 아무 동작하지않을 때 접근
	// false일땐 드래그 이벤트 중에 접근했을 경우

	POINT pt = pts;
	ScreenToClient(&pt);
	if (PtInRect(m_timeLineRect, pt))
	{
		ComplexMap<int, CRect>::iterator iter = m_timeLinePointMap.begin();

		bool bFind = false;
		while (iter != m_timeLinePointMap.end())
		{
			if (PtInRect(iter->value.value, pt))
			{
				bFind = true;
				break;
			}
			iter++;
		}

		if (bFind)
		{
			m_nPointingTimeIDX = iter->value.key;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

			m_detailDlg.SetWindowPos(NULL, pts.x - 400 - 5, pts.y - 200 - 5, 0, 0, SWP_NOSIZE);
			m_detailDlg.ShowWindow(SW_SHOW);
			bDetailOpen = true;
			if (thisApproch)
			{
				if (m_nPointingTimeIDX > m_timeLineContainer.size() - 1)
					return;

				int thisNoteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
				NoteInformationVO note;
				note.SetNotSEQ(thisNoteSEQ);
				RequestScope->SetRequestAttributes(note);
				if (MVC_Controller->SelectOneNoteInformation())
				{
					RequestScope->GetRequestAttributes(&note);
					// this쪽에만 입력
					m_detailDlg.SetText(note.GetNotCONTENT(), "");
				}

			}
			else
			{
				if (m_nPointingTimeIDX > m_timeLineContainer.size() - 1)
					return;

				if (notSEQ < 0)
					return;

				int thisNoteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
				NoteInformationVO note1, note2;
				note1.SetNotSEQ(thisNoteSEQ);
				note2.SetNotSEQ(notSEQ);
				RequestScope->SetRequestAttributes(note1);
				if (MVC_Controller->SelectOneNoteInformation() == false)
					return;

				RequestScope->GetRequestAttributes(&note1);

				RequestScope->SetRequestAttributes(note2);
				if (MVC_Controller->SelectOneNoteInformation() == false)
					return;

				RequestScope->GetRequestAttributes(&note2);
				// 둘다 입력 (첫번째는 드래그해서 온 노트가 this가 된다.)
				m_detailDlg.SetText(note2.GetNotCONTENT(), note1.GetNotCONTENT());

			}
		}
		else
		{
			m_nPointingTimeIDX = -1;

			bDetailOpen = false;
			m_detailDlg.ShowWindow(SW_HIDE);
			if (thisApproch)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			else
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

		}
		Invalidate();
	}
	else
	{
		bDetailOpen = false;
		m_detailDlg.ShowWindow(SW_HIDE);
	}
}

BOOL Timeline::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_MOUSEMOVE)
	{
		ThickEventTimeline(-1, pMsg->pt);
	}
	else if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		POINT pt = pMsg->pt;
		ScreenToClient(&pt);

		if (PtInRect(m_timeLineRect, pt))
			return FALSE;

		CRect rect;
		GetParent()->GetWindowRect(rect);
		if (PtInRect(m_thisRect, pt))
		{
			m_oneViewDlg.SetWindowPos(NULL, rect.right, rect.top, 0, 0, SWP_NOSIZE);
			m_oneViewDlg.ShowWindow(SW_SHOW);
			TimelineOneViewProcess();
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (DragDown(pMsg))
			return TRUE;
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (DragUp(pMsg))
			return TRUE;
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (DragMove(pMsg))
			return FALSE;
	}

	return __super::PreTranslateMessage(pMsg);
}

void Timeline::TimelineOneViewProcess()
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	ComplexVector<TimelineVO>::iterator iter = m_timeLineContainer.begin();

	ComplexString oneViewTimelineText;
	while (iter != m_timeLineContainer.end())
	{
		NoteInformationVO note;
		note.SetNotSEQ(iter->value.GetNotSEQ());
		RequestScope->SetRequestAttributes(note);
		if (MVC_Controller->SelectOneNoteInformation() == false)
			return;

		RequestScope->GetRequestAttributes(&note);
		if (oneViewTimelineText.IsEmpty())
			oneViewTimelineText.AppendFormat("%s", note.GetNotCONTENT().GetBuffer());
		else
			oneViewTimelineText.AppendFormat("\r\n%s", note.GetNotCONTENT().GetBuffer());


		iter++;
	}

	m_oneViewDlg.SetTimelineText(oneViewTimelineText);

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}


bool Timeline::DragDown(MSG* pMsg)
{
	
	return true;
}

bool Timeline::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	
	return true;
}

bool Timeline::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;


	return true;
}
