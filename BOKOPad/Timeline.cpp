// Timeline.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "Timeline.h"
#include "BOKOManager/UIManager/TimelineUIManager.h"
#include "BOKOManager/UIManager/NoteUIManager.h"
#include "BOKOManager/DBManager/NoteDBManager.h"
#include "BOKOManager/DBManager/TimelineDBManager.h"
#include "afxdialogex.h"


// Timeline 대화 상자

IMPLEMENT_DYNAMIC(Timeline, CDialogEx)

Timeline::Timeline(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMELINE, pParent)
	, m_timeUIManager(new TimelineUIManager)
	, m_timeDBManager(new TimelineDBManager)
{
	m_bAttachManagerInit = false;
	m_bInit = false;
	m_nLineStartPoint_X = 0;
	m_nLineEndPoint_X = 0;
	m_nLinePoint_Y = 0;
	m_nDataVariableWidth = 0;
	m_nPointingTimeIDX = -1;
	m_bDetailOpen = false;
	m_timelineThickApprochState = TTA_NOTHING;
	Log_Manager->OnPutLog("타임라인 UI 매니저 생성 완료", LogType::LT_PROCESS);
	Log_Manager->OnPutLog("타임라인 DB 매니저 생성 완료", LogType::LT_PROCESS);
	Log_Manager->OnPutLog("Timeline 생성자 호출", LogType::LT_PROCESS);
}

Timeline::~Timeline()
{
	if (m_timeUIManager)
	{
		delete m_timeUIManager;
		m_timeUIManager = nullptr;
	}

	if (m_timeDBManager)
	{
		delete m_timeDBManager;
		m_timeDBManager = nullptr;
	}

	m_oneViewDlg.DestroyWindow();
	m_detailDlg.DestroyWindow();
	m_linePen.DeleteObject();
	m_drawPen.DeleteObject();
	m_drawHoverPen.DeleteObject();
	Log_Manager->OnPutLog("Timeline 소멸자 호출", LogType::LT_PROCESS);
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

	m_timeUIManager->AttachManager(this);
	Log_Manager->OnPutLog("Timeline UI 매니저 연결", LogType::LT_PROCESS);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_linePen.CreatePen(PS_SOLID, 3, LINE_COLOR);
	m_drawHoverPen.CreatePen(PS_SOLID, 2, LINE_COLOR);
	m_drawPen.CreatePen(PS_SOLID, 1, LINE_COLOR);

	m_detailDlg.Create(TimelineDetail::IDD, this);
	Log_Manager->OnPutLog("노트 상세정보 화면 생성 완료", LogType::LT_OPERATE);
	m_oneViewDlg.Create(BOKOTimelineOneViewDlg::IDD, this);
	Log_Manager->OnPutLog("한눈에 보기 화면 생성 완료", LogType::LT_OPERATE);
	m_detailDlg.MoveWindow(0, 0, 400, 200);
	m_detailDlg.ShowWindow(SW_HIDE);
	m_oneViewDlg.ShowWindow(SW_HIDE);

	Log_Manager->OnPutLog("한눈에보기 매니저 연결", LogType::LT_PROCESS);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void Timeline::AttachNoteManager(NoteUIManager* manager, NoteDBManager* dbmanager)
{
	m_bAttachManagerInit = true;
	m_noteUIManager = manager;
	m_oneViewDlg.AttachManager(dbmanager);
}

void Timeline::HideTimelineDetail()
{
	m_detailDlg.ShowWindow(SW_HIDE);
}

bool Timeline::ReloadTimeline()
{
	TimelineVO time;
	time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());

	m_timeLineContainer.clear();
	if (m_timeDBManager->SelectInSceSEQTimeline(m_thisDataStruct.scenarioData.GetSceSEQ(), &m_timeLineContainer) == false)
		return false;

	Log_Manager->OnPutLog("타임라인 정보 갱신", LogType::LT_PROCESS);

	Invalidate();

	return true;
}

int Timeline::ValidatePointToRectIDX(POINT pt)
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

bool Timeline::UpdateTimelineIDX(int startUpdateTimeIDX)
{
	// 업데이트를 오름차순으로 처리하면 같은값이 되서 내림차순으로 처리해야한다.
	TimelineVO time;
	//time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	for (int i = m_timeLineContainer.size() - 1; i >= startUpdateTimeIDX; i--)
	{
		if (m_timeDBManager->UpdateTimelineInTimeIDXPlus(m_timeLineContainer.at(i).GetNotSEQ(), i) == false)
			return false;
	}

	Log_Manager->OnPutLog("타임라인 시퀀스 정보 갱신", LogType::LT_PROCESS);

	return true;
}

bool Timeline::InsertTimeline(int notSEQ, POINT currentMPoint)
{
	int timeIDX = ValidatePointToRectIDX(currentMPoint);
	if (timeIDX == -1)
		timeIDX = m_timeLineContainer.size();

	if (UpdateTimelineIDX(timeIDX) == false)
		return false;

	TimelineVO time;
	time.SetNotSEQ(notSEQ);
	time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	time.SetTimeIDX(timeIDX);
	if (m_timeDBManager->InsertTimeline(time) == false)
		return false;

	Log_Manager->OnPutLog("타임라인 등록 성공", LogType::LT_PROCESS);

	return ReloadTimeline();
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


bool Timeline::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	if (!m_bAttachManagerInit)
		return false;

	m_thisDataStruct = thisDataStruct;
	m_defaultDragData.sceSEQ = thisDataStruct.scenarioData.GetSceSEQ();
	m_defaultDragData.sceIndex = thisDataStruct.scenarioIndex;

	m_oneViewDlg.SetScenarioManagerStruct(m_thisDataStruct);

	GetClientRect(m_thisRect);
	m_nLineStartPoint_X = m_thisRect.left + 5;
	m_nLineEndPoint_X = m_thisRect.Width() - m_nLineStartPoint_X;
	m_nLinePoint_Y = int(m_thisRect.Height() / 2);
	m_nDataVariableWidth = m_nLineEndPoint_X - m_nLineStartPoint_X;

	m_timeLineRect.left = m_nLineStartPoint_X;
	m_timeLineRect.right = m_nLineEndPoint_X;
	m_timeLineRect.top = m_nLinePoint_Y - LINE_HEIGHT;
	m_timeLineRect.bottom = m_nLinePoint_Y + LINE_HEIGHT;

	if (ReloadTimeline() == false)
		return false;

	m_bInit = true;

	return true;
}

bool Timeline::ThickEventTimeline(int notSEQ, POINT pts, TimelineThickApproch m_timelineThickApprochState)
{
	// thisApproch는 true일때 마우스가 아무 동작하지않을 때 접근
	// false일땐 드래그 이벤트 중에 접근했을 경우

	//// 현재 드래그가 다른시나리오쪽에서 작동중일경우 처리x
	//if (Scenario_Manager->SendMessages(PM_DRAG_PROCESS_LOCK) == true)
	//	return false;

	// 

	if (m_timeLineContainer.empty())
		return false;

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

		// 마우스 커서가 노트있는 곳을 발견했을 때
		if (bFind)
		{
			m_nPointingTimeIDX = iter->value.key;
			// 우선 마우스를 핸드로 변경
			CURSOR_HAND;
			m_detailDlg.SetWindowPos(NULL, pts.x - 400 - 5, pts.y - 200 - 5, 0, 0, SWP_NOSIZE);
			m_detailDlg.ShowWindow(SW_SHOW);
			m_bDetailOpen = true;
			
			if (m_timelineThickApprochState == TTA_WELL_APPROCH)
			{
				Log_Manager->OnPutLog("마우스와 타임라인 접촉 이벤트", LogType::LT_EVENT);

				if (m_nPointingTimeIDX > m_timeLineContainer.size() - 1)
					return false;

				int thisNoteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
				NoteInformationVO note;
				if (m_timeDBManager->SelectOneNoteInformation(thisNoteSEQ, &note) == false)
					return false;

				Log_Manager->OnPutLog("집은 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

				// this쪽에만 입력
				m_detailDlg.SetText(note.GetNotCONTENT(), "");
			}
			else if (m_timelineThickApprochState == TTA_NOTE_BY_TIMELINE_DRAG_EVENT_APPROCH)
			{
				Log_Manager->OnPutLog("노트와 타임라인 접촉 이벤트", LogType::LT_EVENT);

				if (m_nPointingTimeIDX > m_timeLineContainer.size() - 1)
					return false;

				if (notSEQ < 0)
					return false;

				int thisNoteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
				NoteInformationVO note1, note2;
				if (m_timeDBManager->SelectOneNoteInformation(thisNoteSEQ, &note1) == false)
					return false;

				Log_Manager->OnPutLog("집은 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

				if (m_timeDBManager->SelectOneNoteInformation(notSEQ, &note2) == false)
					return false;

				Log_Manager->OnPutLog("갖다 댄 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

				// 둘다 입력 (첫번째는 드래그해서 온 노트가 this가 된다.)
				m_detailDlg.SetText(note2.GetNotCONTENT(), note1.GetNotCONTENT());
			}
			else if (m_timelineThickApprochState == TTA_TIMELINE_BY_TIMELINE_DRAG_EVENT_APPROCH)
			{
				// 마우스 다운이벤트때 timeIDX 정렬을 안하고 컨테이너 삭제만 했기때문에 
				// m_nPointingTimeIDX값이 정렬안된 상태의 인덱스임. 그래서 따로 구분함..
				Log_Manager->OnPutLog("타임라인과 타임라인 접촉 이벤트", LogType::LT_EVENT);

				if (notSEQ < 0)
					return false;

				// 해당 timeIDX값 뽑아오는데 db 조회를 해야하는건가? 일단 생각해두자
				TimelineVO time;
				if (m_timeDBManager->SelectInTimeIDXTimelineInNotSEQ(notSEQ, &time) == false)
					return false;
				Log_Manager->OnPutLog("노트 시퀀스로 타임라인정보 DB 조회 성공", LogType::LT_PROCESS);

				// 현재 잡고있는 타임라인 idx값보다 마우스로 댄 타임라인 idx값이 더 클경우는 -1 처리 
				if (time.GetTimeIDX() < m_nPointingTimeIDX)
					m_nPointingTimeIDX -= 1;
				else if (time.GetTimeIDX() == m_nPointingTimeIDX)
					return false;

				int thisNoteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
				NoteInformationVO note1, note2;
				note2.SetNotSEQ(notSEQ);
				if (m_timeDBManager->SelectOneNoteInformation(thisNoteSEQ, &note1) == false)
					return false;

				Log_Manager->OnPutLog("집은 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

				if (m_timeDBManager->SelectOneNoteInformation(notSEQ, &note2) == false)
					return false;

				Log_Manager->OnPutLog("갖다 댄 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

				// 둘다 입력 (첫번째는 드래그해서 온 노트가 this가 된다.)
				m_detailDlg.SetText(note2.GetNotCONTENT(), note1.GetNotCONTENT());
			}
		}
		// 발견하지 못했을 땐
		else
		{
			m_nPointingTimeIDX = -1;

			m_bDetailOpen = false;
			m_detailDlg.ShowWindow(SW_HIDE);
			if (m_timelineThickApprochState == TTA_WELL_APPROCH)
				CURSOR_ARROW;	// 드래그이벤트로 접근하지 않았을 경우 화살표로 변경
			else
				CURSOR_CROSS;	// 드래그이벤트로 접근했을 땐 마우스가 크로스로 접근했기 때문에 크로스로 되돌림

		}
		Invalidate();
	}
	else
	{
		m_bDetailOpen = false;
		m_detailDlg.ShowWindow(SW_HIDE);
	}

	return true;
}

void Timeline::SetExpandCloseEvent()
{
	m_oneViewDlg.SetExpandCloseEvent();
}

BOOL Timeline::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		POINT pt = pMsg->pt;
		ScreenToClient(&pt);

		if (PtInRect(m_timeLineRect, pt))
			return FALSE;

		CRect rect;
		GetParent()->GetWindowRect(rect);
		if (PtInRect(m_thisRect, pt))
		{
			if (GetSystemMetrics(SM_CXSCREEN) - 500 < rect.right)
				m_oneViewDlg.SetWindowPos(NULL, rect.left - rect.Width(), rect.top, 0, 0, SWP_NOSIZE);
			else
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
		if (!m_bDragProcessing)
			ThickEventTimeline(-1, pMsg->pt, TTA_WELL_APPROCH);

		if (DragMove(pMsg))
			return FALSE;
	}

	return __super::PreTranslateMessage(pMsg);
}

bool Timeline::TimelineOneViewProcess()
{
	CURSOR_WAIT;
	ComplexVector<TimelineVO>::iterator iter = m_timeLineContainer.begin();

	ComplexString oneViewTimelineText;

	m_oneViewDlg.Clear();
	while (iter != m_timeLineContainer.end())
	{
		NoteInformationVO note;
		if (m_timeDBManager->SelectOneNoteInformation(iter->value.GetNotSEQ(), &note) == false)
			return false;
		Log_Manager->OnPutLog("노트 정보 DB 조회 완료", LogType::LT_PROCESS);

		m_oneViewDlg.SetTimelineText(note.GetNotCONTENT(), note.GetNotSEQ());

		iter++;
	}

	Log_Manager->OnPutLog("한눈에 보기 화면 출력 완료", LogType::LT_EVENT);

	CURSOR_CROSS;

	return true;
}

void Timeline::OneViewRefresh()
{
	if (m_oneViewDlg.IsWindowVisible())
		TimelineOneViewProcess();

	Log_Manager->OnPutLog("한눈에 보기 화면 갱신 완료", LogType::LT_PROCESS);
}

bool Timeline::DragDown(MSG* pMsg)
{
	Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
	if (Scenario_UI_Manager->SendMessages(PM_IS_DRAGGING_MODE) == false)
		return false;

	POINT pt = pMsg->pt;
	ScreenToClient(&pt);
	if (PtInRect(m_timeLineRect, pt) == false)
		return false;

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

	if (!bFind)
		return false;

	if (m_nPointingTimeIDX > m_timeLineContainer.size() - 1)
		return false;

	if (m_nPointingTimeIDX < 0)
		return false;

	if (m_bDetailOpen)
	{
		m_bDetailOpen = false;
		m_detailDlg.ShowWindow(SW_HIDE);
	}
	
	// 드래그 다이얼로그 띄우기
	m_defaultDragData.mousePos_X = pMsg->pt.x;
	m_defaultDragData.mousePos_Y = pMsg->pt.y;
	m_defaultDragData.noteSEQ = m_timeLineContainer.at(m_nPointingTimeIDX).GetNotSEQ();
	m_timeUIManager->InputDragStruct(&m_defaultDragData);
	m_bDragProcessing = m_timeUIManager->SendMessages(PM_TIMELINE_DRAG_DOWN);

	if (!m_bDragProcessing)
		return false;
	
	// 타임라인 맵에서 해당 데이터 삭제
	// 우선 컨테이너에서만 삭제시킨다.
	ComplexVector<TimelineVO>::iterator iter2 = m_timeLineContainer.begin();

	while (iter2 != m_timeLineContainer.end())
	{
		if (iter2->value.GetTimeIDX() == m_nPointingTimeIDX)
		{
			m_timeLineContainer.erase(iter2);
			Invalidate();
			break;
		}
		iter2++;
	}

	Log_Manager->OnPutLog("드래그 버튼 다운", LogType::LT_EVENT);

	return true;
}

bool Timeline::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	m_defaultDragData.mousePos_X = pMsg->pt.x;
	m_defaultDragData.mousePos_Y = pMsg->pt.y;
	m_timeUIManager->InputDragStruct(&m_defaultDragData);
	m_timeUIManager->SendMessages(PM_TIMELINE_DRAG_MOVE);

	return true;
}

bool Timeline::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	Log_Manager->OnPutLog("드래그 버튼 업", LogType::LT_EVENT);

	m_defaultDragData.mousePos_X = pMsg->pt.x;
	m_defaultDragData.mousePos_Y = pMsg->pt.y;
	m_timeUIManager->InputDragStruct(&m_defaultDragData);
	if (m_timeUIManager->SendMessages(PM_TIMELINE_DRAG_UP))
	{
		CURSOR_WAIT;
		DragUpState dus = m_timeUIManager->GetDragState();
		if (dus == DUS_NOTHING)
		{
			if (ReloadTimeline() == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;
				return false;
			}
			Log_Manager->OnPutLog("선택한 위치 : 없음", LogType::LT_PROCESS);
		}
		else if (dus == DUS_THIS)
		{
			m_timeDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_TIMELINE_THIS_SIGNAL]);

			m_timeUIManager->InputDragStruct(&m_defaultDragData);
			if (m_timeUIManager->SendMessages(PM_NOTE_INSERT) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 노트 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				if (ReloadTimeline() == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			if (m_timeDBManager->DeleteTimeline(m_defaultDragData.noteSEQ, m_thisDataStruct.scenarioData.GetSceSEQ()) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 타임라인 DB 삭제 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				if (ReloadTimeline() == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			Log_Manager->OnPutLog("타임라인 정보 DB 삭제 완료", LogType::LT_PROCESS);

			if (ReloadTimeline() == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 시나리오 갱신 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");
				return false;
			}

			Log_Manager->OnPutLog("선택한 위치 : 현재 시나리오", LogType::LT_PROCESS);
			m_timeDBManager->CommitTransaction();
		}
		else if (dus == DUS_ANOTHER)
		{
			m_timeDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_TIMELINE_ANOTHER_SIGNAL]);

			// 현재 다이얼로그에서 노트정보 삭제 (timeline <-> noteinformation cascade 관계라 note만 지워도 됨)
			NoteInformationVO inNote(m_defaultDragData.noteSEQ, 0, false, false, "");
			// 해당 notSEQ로 notCONTENT 뽑아오기
			if (m_timeDBManager->SelectOneNoteInformation(m_defaultDragData.noteSEQ, &inNote) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 노트 정보 DB 조회 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				return false;
			}
			Log_Manager->OnPutLog("노트 정보 DB 조회 완료", LogType::LT_PROCESS);
			m_defaultDragData.noteCONTENT = inNote.GetNotCONTENT();

			// 노트 삭제
			if (m_timeDBManager->DeleteNoteInformation(inNote.GetNotSEQ()) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 노트 정보 DB 삭제 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				if (ReloadTimeline() == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			Log_Manager->OnPutLog("노트 정보 DB 삭제 완료", LogType::LT_PROCESS);

			// 타임라인정보 갱신
			if (ReloadTimeline() == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 시나리오 갱신 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");
				return false;
			}

			
			// 노트매니저와 노트컨트롤쪽 컨테이너, 맵 삭제하고 타겟 다이얼로그에 노트 등록
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_ANOTHER_ATTACH) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 타임라인 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				// 타임라인 -> 타겟다이얼로그 롤백 메시지 구현
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_TIMELINE_ANOTHER_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			
			Log_Manager->OnPutLog("선택한 위치 : 다른 시나리오", LogType::LT_PROCESS);
			m_timeDBManager->CommitTransaction();
		}
		else if (dus == DUS_THIS_TIMELINE)
		{
			m_timeDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_TIMELINE_THIS_TIMELINE_SIGNAL]);

			// 마우스 다운이벤트때 m_timeLineContainer에서만 해당 타임라인을 삭제시켜서 
			// ValidatePointToRect() 함수로 가져온 timeIDX값을 가지고 모두 업데이트 시킨다.
			// 매니저 호출 필요없이 db에서 timeIDX값 갱신시키고 select 갱신 시킨 후 invalidate 하면 된다.
			int timeIDX = ValidatePointToRectIDX(pMsg->pt);	// 마우스 포인트가 가르킨 timeIDX 이거나 사잇값일떈 뒤쪽 timeIDX
			if (timeIDX == -1)
				timeIDX = m_timeLineContainer.size() + 1;	// 다운이벤트때 타임라인 컨테이너에서 삭제가되어서 임시로 +1 시킨것
			
			TimelineVO time;
			time.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
			time.SetNotSEQ(m_defaultDragData.noteSEQ);

			// 집어든 notSEQ값으로 timeIDX값을 가져온다.
			if (m_timeDBManager->SelectInTimeIDXTimelineInNotSEQ(m_defaultDragData.noteSEQ, &time) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("노트 시퀀스로 타임라인정보 DB 조회 실패로 인한 롤백 처리", LogType::LT_PROCESS);
				if (ReloadTimeline() == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			Log_Manager->OnPutLog("노트 시퀀스로 타임라인정보 DB 조회 성공", LogType::LT_PROCESS);

			int dragEventingTimeIDX = time.GetTimeIDX();

			// 집어든 idx가 발견한 idx보다 클때 
			if (dragEventingTimeIDX + 1 < timeIDX)
			{
				// 집어든 timeIDX값을 발견한 timeIDX - 1값으로 갱신
				if (m_timeDBManager->UpdateTimelineInTimeIDX(m_defaultDragData.noteSEQ, dragEventingTimeIDX, timeIDX - 1) == false)
				{
					m_bDragProcessing = false;
					CURSOR_ARROW;

					m_timeDBManager->RollbackTransaction();
					Log_Manager->OnPutLog("타임라인 정보 DB 갱신 실패로 인한 롤백 처리", LogType::LT_PROCESS);
					if (ReloadTimeline() == false)
						MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

					return false;
				}
				Log_Manager->OnPutLog("타임라인 정보 DB 갱신 성공", LogType::LT_PROCESS);

				// 발견한 timeIDX값보다 낮고 집어든 timeIDX값보다 높은 timeIDX는 전부 -1 처리 (이미 타임라인 컨테이너는 삭제가되있어서 잡고있는 타임라인 idx부터 처리해야함)
				for (int i = dragEventingTimeIDX; i < timeIDX - 1; i++)
				{
					if (m_timeDBManager->UpdateTimelineInTimeIDXMinus(m_timeLineContainer.at(i).GetNotSEQ(), i + 1) == false)
					{
						m_bDragProcessing = false;
						CURSOR_ARROW;

						m_timeDBManager->RollbackTransaction();
						Log_Manager->OnPutLog("타임라인 정보 DB 갱신 실패로 인한 롤백 처리", LogType::LT_PROCESS);
						if (ReloadTimeline() == false)
							MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

						return false;
					}
				}
				Log_Manager->OnPutLog("타임라인 정보 DB 갱신 성공", LogType::LT_PROCESS);
			}
			// 집어든 idx가 그대로일때
			else if (dragEventingTimeIDX + 1 == timeIDX)
			{
				// do nothing
			}
			// 집어든 idx가 발견한 idx보다 작을때
			else if (dragEventingTimeIDX > timeIDX)
			{
				// 집어든 timeIDX값을 발견한 timeIDX값으로 갱신
				if (m_timeDBManager->UpdateTimelineInTimeIDX(m_defaultDragData.noteSEQ, dragEventingTimeIDX, timeIDX) == false)
				{
					m_bDragProcessing = false;
					CURSOR_ARROW;

					m_timeDBManager->RollbackTransaction();
					Log_Manager->OnPutLog("타임라인 정보 DB 갱신 실패로 인한 롤백 처리", LogType::LT_PROCESS);
					if (ReloadTimeline() == false)
						MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

					return false;
				}
				Log_Manager->OnPutLog("타임라인 정보 DB 갱신 성공", LogType::LT_PROCESS);

				// 발견한 timeIDX값보다 높거나 같고 집어든 timeIDX값보다 낮은 timeIDX는 전부 +1 처리
				for (int i = timeIDX; i < dragEventingTimeIDX; i++)
				{
					if (m_timeDBManager->UpdateTimelineInTimeIDXPlus(m_timeLineContainer.at(i).GetNotSEQ(), i) == false)
					{
						m_bDragProcessing = false;
						CURSOR_ARROW;

						m_timeDBManager->RollbackTransaction();
						Log_Manager->OnPutLog("타임라인 정보 DB 갱신 실패로 인한 롤백 처리", LogType::LT_PROCESS);
						if (ReloadTimeline() == false)
							MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

						return false;
					}
				}
				Log_Manager->OnPutLog("타임라인 정보 DB 갱신 성공", LogType::LT_PROCESS);
			}
			// 같을때는 m_timeLineContainer가 하나밖에없는 상태에서 드래그처리했을 시
			else
			{
				// do nothing;
			}

			if (ReloadTimeline() == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 시나리오 갱신 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			OneViewRefresh();

			Log_Manager->OnPutLog("선택한 위치 : 현재 타임라인", LogType::LT_PROCESS);
			m_timeDBManager->CommitTransaction();
		}
		else if (dus == DUS_ANOTHER_TIMELINE)
		{
			m_timeDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_TIMELINE_ANOTHER_TIMELINE_SIGNAL]);

			// 집어든 notSEQ값으로 timeIDX값을 가져온다.
			TimelineVO time;
			if (m_timeDBManager->SelectInTimeIDXTimelineInNotSEQ(m_defaultDragData.noteSEQ, &time) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("노트 시퀀스로 타임라인정보 DB 조회 실패로 인한 롤백 처리", LogType::LT_PROCESS);
				if (ReloadTimeline() == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			Log_Manager->OnPutLog("노트 시퀀스로 타임라인정보 DB 조회 성공", LogType::LT_PROCESS);

			int dragEventingTimeIDX = time.GetTimeIDX();


			// 클릭한 타임라인의 notSEQ값으로 내용을 가져온다.
			NoteInformationVO note;
			if (m_timeDBManager->SelectOneNoteInformation(m_defaultDragData.noteSEQ, &note) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("선택한 노트 정보 DB 조회 실패로 인한 롤백 처리", LogType::LT_PROCESS);
				return false;
			}
			Log_Manager->OnPutLog("선택한 노트 정보 DB 조회 성공", LogType::LT_PROCESS);

			m_defaultDragData.noteCONTENT = note.GetNotCONTENT();
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_ANOTHER_TIMELINE_ATTACH) == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("다른 타임라인 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_ANOTHER_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}


			// 집어든 노트 timeIDX보다 뒤쪽의 IDX를 정렬한다.
			for (int i = 0; i < m_timeLineContainer.size(); i++)
			{
				if (dragEventingTimeIDX < m_timeLineContainer.at(i).GetTimeIDX())
				{
					if (m_timeDBManager->UpdateTimelineInTimeIDXMinus(m_timeLineContainer.at(i).GetNotSEQ(), m_timeLineContainer.at(i).GetTimeIDX()) == false)
					{
						m_bDragProcessing = false;
						CURSOR_ARROW;

						m_timeDBManager->RollbackTransaction();
						Log_Manager->OnPutLog("타임라인 정보 DB 갱신 실패로 인한 롤백 처리", LogType::LT_PROCESS);
						if (ReloadTimeline() == false)
							MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");
						
						return false;
					}
				}
			}

			Log_Manager->OnPutLog("타임라인 정보 DB 갱신 성공", LogType::LT_PROCESS);

			if (ReloadTimeline() == false)
			{
				m_bDragProcessing = false;
				CURSOR_ARROW;

				m_timeDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("현재 시나리오 갱신 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			OneViewRefresh();
			Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
			Scenario_UI_Manager->SendMessages(PM_TARGET_SCENARIO_ONE_VIEW_REFRESH);

			Log_Manager->OnPutLog("선택한 위치 : 다른 타임라인", LogType::LT_PROCESS);
			m_timeDBManager->CommitTransaction();
		}

		CURSOR_ARROW;
	}
	else
	{
		CURSOR_ARROW;
		m_bDragProcessing = false;
		Log_Manager->OnPutLog("드래그 버튼 업 이벤트 실패", LogType::LT_PROCESS);
		return false;
	}

	Log_Manager->OnPutLog("드래그 버튼 업 이벤트 완료", LogType::LT_PROCESS);
	m_bDragProcessing = false;
	return true;
}
