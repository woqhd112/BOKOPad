// NoteListCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "NoteListCtrl.h"
#include "BOKOManager/UIManager/NoteUIManager.h"
#include "BOKOManager/DBManager/NoteDBManager.h"
#include "afxdialogex.h"

// NoteListCtrl 대화 상자\

IMPLEMENT_DYNAMIC(NoteListCtrl, CDialogEx)

NoteListCtrl::NoteListCtrl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NOTE_LIST_CTRL, pParent)
	, m_noteInformationContainer(new ComplexVector<NoteInformationVO>)
	, m_noteUIManager(new NoteUIManager)
	, m_noteDBManager(new NoteDBManager)
	, m_downButton(nullptr)
{
	m_bMainScrollFocus = true;
	m_viewNoteSize = 0;
	Log_Manager->OnPutLog("노트 UI 매니저 생성 완료", LogType::LT_PROCESS);
	Log_Manager->OnPutLog("노트 DB 매니저 생성 완료", LogType::LT_PROCESS);
	Log_Manager->OnPutLog("NoteListCtrl 생성자 호출", LogType::LT_PROCESS);
}

NoteListCtrl::~NoteListCtrl()
{
	if (m_noteUIManager)
	{
		delete m_noteUIManager;
		m_noteUIManager = nullptr;
	}

	if (m_noteDBManager)
	{
		delete m_noteDBManager;
		m_noteDBManager = nullptr;
	}

	if (m_noteInformationContainer)
	{
		delete m_noteInformationContainer;
		m_noteInformationContainer = nullptr;
	}
	Log_Manager->OnPutLog("NoteListCtrl 소멸자 호출", LogType::LT_PROCESS);
}

void NoteListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NoteListCtrl, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// NoteListCtrl 메시지 처리기


BOOL NoteListCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_noteUIManager->AttachManager(this);
	Log_Manager->OnPutLog("노트 UI 매니저 연결", LogType::LT_PROCESS);

	ScrollProcess::ScrollInfo info;
	info.scrollExecuteCtrl = this;
	info.wheelSize = EDIT_HEIGHT + 10;
	scroll.Init(info);
	scroll.ExecuteScroll(SCROLL_LINE_NOTHING);

	m_calculateItemPos.left = 0;
	m_calculateItemPos.top = 0;
	m_calculateItemPos.bottom = EDIT_HEIGHT;
	m_calculateItemPos.right = EDIT_WIDTH;

	GotoDlgCtrl(this);

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool NoteListCtrl::LoadDraggingNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);
			m_noteUIManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteUIManager->SendMessages(PM_LOAD_DRAGGING_MODE) == false)
				return false;
			bDetectedTimelineCnt++;
		}

		i++;
		iter++;
	}

	Log_Manager->OnPutLog("Dragging 모드 변경 성공", LogType::LT_PROCESS);

	return true;
}

bool NoteListCtrl::UnloadDraggingNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);
			m_noteUIManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteUIManager->SendMessages(PM_UNLOAD_DRAGGING_MODE) == false)
				return false;
			bDetectedTimelineCnt++;
		}

		i++;
		iter++;
	}

	Log_Manager->OnPutLog("UnDragging 모드 변경 성공", LogType::LT_PROCESS);

	return true;
}

bool NoteListCtrl::DeleteAllItems()
{
	if (m_noteUIManager->SendMessages(PM_NOTE_CLEAR) == false)
		return false;

	m_viewNoteSize = 0;
	scroll.ResetScroll();

	Log_Manager->OnPutLog("노트 UI 아이템 모두 삭제 완료", LogType::LT_PROCESS);

	return true;
}

bool NoteListCtrl::LoadNoteInformation()
{
	CURSOR_WAIT;
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), false, false, "");
	if (UpdateScenarioList(&inNote) == false)
		return false;

	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	int i = 0;
	int bDetectedTimelineCnt = 0;
	while (iter != m_noteInformationContainer->end())
	{
		CRect* itemRect = CalcNotePosition(bDetectedTimelineCnt);
		NoteManagerStruct noteManagerStruct(&iter->value, itemRect, i);

		m_noteUIManager->InputNoteStruct(&noteManagerStruct);
		if (m_noteUIManager->SendMessages(PM_NOTE_INSERT) == false)
			return false;

		if (iter->value.IsSetTIMELINE() == false)
		{
			bDetectedTimelineCnt++;
			ScrollExecute(true);
		}
		i++;
		iter++;
	}
	CURSOR_ARROW;

	Log_Manager->OnPutLog("노트정보 로드 완료", LogType::LT_PROCESS);

	return true;
}

bool NoteListCtrl::UpdateScenarioList(NoteInformationVO* noteInform)
{
	m_noteInformationContainer->clear();
	if (m_noteDBManager->SelectInSceSEQNoteInformation(noteInform->GetSceSEQ(), m_noteInformationContainer) == false)
		return false;
	
	Log_Manager->OnPutLog("시나리오 정보 갱신", LogType::LT_PROCESS);

	return true;
}

bool NoteListCtrl::UpdateSetTIME(int notSEQ)
{
	NoteInformationVO note;
	note.SetNotSEQ(notSEQ);
	note.SetSceSEQ(m_thisDataStruct.scenarioData.GetSceSEQ());
	if (m_noteDBManager->UpdateNoteInformationInSetTIMELINE(notSEQ, false) == false)
		return false;

	Log_Manager->OnPutLog("노트 DB 정보 타임라인으로 변경 완료", LogType::LT_PROCESS);

	if (UpdateScenarioList(&note) == false)
		return false;

	NoteManagerStruct noteManagerStruct(&note, NULL, notSEQ);

	m_noteUIManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteUIManager->SendMessages(PM_FIND_NOTE_INDEX) == false)
		return false;
	Log_Manager->OnPutLog("노트 인덱스 정보 호출 완료", LogType::LT_PROCESS);

	m_noteUIManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteUIManager->SendMessages(PM_NOTE_SHOW) == false)
		return false;
	Log_Manager->OnPutLog("해당 노트 화면 출력 완료", LogType::LT_PROCESS);
		
	// 타임라인 보이기 false로 업데이트 완료했으니 노트들 무빙시켜야함
	if (MoveNote() == false)
		return false;

	ScrollExecute(true, true);
	return true;
}

bool NoteListCtrl::InsertNote(ComplexString inpusString, bool bNoteShow)
{
	NoteInformationVO inNote(0, m_thisDataStruct.scenarioData.GetSceSEQ(), !bNoteShow, false, inpusString);
	if (m_noteDBManager->InsertNoteInformation(inNote) == false)
		return false;

	Log_Manager->OnPutLog("노트 DB 등록 성공", LogType::LT_PROCESS);

	if (UpdateScenarioList(&inNote) == false)
		return false;

	CRect* itemRect = CalcNotePosition(m_viewNoteSize);
	NoteManagerStruct noteManagerStruct(&m_noteInformationContainer->back(), itemRect, m_noteInformationContainer->size() - 1);

	m_noteUIManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteUIManager->SendMessages(PM_NOTE_INSERT) == false)
		return false;
	Log_Manager->OnPutLog("노트 컨트롤 생성 완료", LogType::LT_PROCESS);

	if (bNoteShow)
		ScrollExecute(true, true);

	return true;
}

bool NoteListCtrl::CheckDeleteNote()
{
	ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationContainer->begin();

	NoteManagerStruct noteManagerStruct(NULL, NULL, -1);
	int deleteCount = 0;
	while (iter != m_noteInformationContainer->end())
	{
		if (iter->value.IsSetTIMELINE() == false)
		{
			noteManagerStruct.noteData = &iter->value;
			m_noteUIManager->InputNoteStruct(&noteManagerStruct);
			// ui삭제안된건 냅두고 된거만 db삭제한다.
			if (m_noteUIManager->SendMessages(PM_NOTE_CHECK_DELETE))
			{
				if (m_noteDBManager->DeleteNoteInformation(noteManagerStruct.noteData->GetNotSEQ()) == false)
					return false;

				Log_Manager->OnPutLog("노트 DB 삭제 완료", LogType::LT_PROCESS);

				deleteCount++;
			}
		}
		iter++;
	}

	if (deleteCount > 0)
	{
		if (DeleteAllItems() == false)
			return false;

		if (LoadNoteInformation() == false)
			return false;

		Log_Manager->OnPutLog("선택한 노트 정보 삭제 완료", LogType::LT_PROCESS);
	}


	return true;
}

bool NoteListCtrl::DeleteNote(int notSEQ)
{
	NoteInformationVO inNote(notSEQ, m_thisDataStruct.scenarioData.GetSceSEQ(), false, false, "");
	if (m_noteDBManager->DeleteNoteInformation(notSEQ) == false)
		return false;

	Log_Manager->OnPutLog("노트 DB 삭제 완료", LogType::LT_PROCESS);

	if (UpdateScenarioList(&inNote) == false)
		return false;

	NoteManagerStruct noteManagerStruct(&inNote, NULL, -1);
	m_noteUIManager->InputNoteStruct(&noteManagerStruct);
	if (m_noteUIManager->SendMessages(PM_NOTE_DELETE) == false)
		return false;

	Log_Manager->OnPutLog("노트 컨트롤 삭제 완료", LogType::LT_PROCESS);

	// 삭제한 노트인덱스 부터 노트리스트의 마지막 인덱스까지 정렬
	if (MoveNote() == false)
		return false;

	ScrollExecute(false, true);


	return true;
}

bool NoteListCtrl::MoveNote()
{
	int noteSize = m_noteInformationContainer->size();

	int bDetectedTimelineCnt = 0;
	for (int i = 0; i < noteSize; i++)
	{
		NoteInformationVO moveVO = m_noteInformationContainer->at(i);
		if (moveVO.IsSetTIMELINE() == false)
		{
			CRect* itemRect = CalcNotePosition(i - bDetectedTimelineCnt);
			NoteManagerStruct noteManagerStruct(&moveVO, itemRect, i);
			m_noteUIManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteUIManager->SendMessages(PM_NOTE_MOVE) == false)
				return false;
		}
		else
			bDetectedTimelineCnt++;
	}

	Log_Manager->OnPutLog("노트 위치 정보 갱신", LogType::LT_PROCESS);
	return true;
}

void NoteListCtrl::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;
	m_defaultDragData.sceSEQ = thisDataStruct.scenarioData.GetSceSEQ();
	m_defaultDragData.sceIndex = thisDataStruct.scenarioIndex;
}

void NoteListCtrl::ScrollExecute(bool bAdd, bool bPosSwitch)
{
	if (bAdd)
	{
		if (m_viewNoteSize % 4 == 0)
		{
			scroll.ExecuteScroll(SCROLL_LINE_ADD);
		}
		++m_viewNoteSize;
	}
	else
	{
		if ((m_viewNoteSize - 1) % 4 == 0)
		{
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
		}
		--m_viewNoteSize;
	}

	if (bPosSwitch)
	{
		// 현재 스크롤 위치가 제일 마지막이 아니라면..
		if (scroll.GetScrollCount() != scroll.GetLineCount())
		{
			int executeScrollCount = scroll.GetLineCount() - scroll.GetScrollCount();
			// 노트를 추가했을 때만 스크롤 다운 이벤트
			if (bAdd)
			{
				for (int i = 0; i < executeScrollCount; i++)
					OnVScroll(SB_LINEDOWN, 0, GetScrollBarCtrl(SB_VERT));
			}
		}
		// 현재 스크롤이 마지막일 때
		else
		{
			// 노트가 삭제되면 페이지 업..
			if (m_viewNoteSize % 4 == 0)
			{
				if (!bAdd)
					OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
			}
		}
	}
}

void NoteListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	scroll.OperateScroll(nSBCode, nPos);

	//CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL NoteListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	OnVScroll(scroll.OperateWheel(zDelta), 0, GetScrollBarCtrl(SB_VERT));

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void NoteListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


CRect* NoteListCtrl::CalcNotePosition(int itemIndex)
{
	// 이부분이 조금 이상한듯? 그리고 타임라인에서 노트쪽으로 드래그할때 마우스포인트도 처리할것
	int nCurrentScrollIndex = scroll.GetScrollCount();

	m_calculateItemPos.left = (EDIT_WIDTH + 5) * (itemIndex % 4);
	m_calculateItemPos.top = ((EDIT_HEIGHT + 10) * ((itemIndex / 4) - nCurrentScrollIndex));// -scroll.GetScrollSize();
	m_calculateItemPos.right = m_calculateItemPos.left + EDIT_WIDTH;
	m_calculateItemPos.bottom = m_calculateItemPos.top + EDIT_HEIGHT;

	Log_Manager->OnPutLog("노트 위치 정보 로드 완료", LogType::LT_PROCESS);

	return &m_calculateItemPos;
}

bool NoteListCtrl::DragDown(MSG* pMsg)
{
	Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
	if (Scenario_UI_Manager->SendMessages(PM_IS_DRAGGING_MODE) == false)
		return false;

	UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
	if (nButtonStyle == BS_PUSHBUTTON || nButtonStyle == BS_DEFPUSHBUTTON)
	{
		m_downButton = (CButton*)FromHandle(pMsg->hwnd);
		m_defaultDragData.mousePos_X = pMsg->pt.x;
		m_defaultDragData.mousePos_Y = pMsg->pt.y;
		m_defaultDragData.buttonID = ::GetDlgCtrlID(pMsg->hwnd);
		m_noteUIManager->InputDragStruct(&m_defaultDragData);
		m_bDragProcessing = m_noteUIManager->SendMessages(PM_DRAG_DOWN);
		if (!m_bDragProcessing)
		{
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_OFF) == false)
				MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

			return false;
		}

		Log_Manager->OnPutLog("드래그 버튼 다운", LogType::LT_EVENT);
		return true;
	}
	return false;
}

bool NoteListCtrl::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	m_defaultDragData.mousePos_X = pMsg->pt.x;
	m_defaultDragData.mousePos_Y = pMsg->pt.y;
	m_noteUIManager->InputDragStruct(&m_defaultDragData);
	m_noteUIManager->SendMessages(PM_DRAG_MOVE);
	return true;
}

bool NoteListCtrl::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	if (m_downButton == nullptr)
	{
		m_bDragProcessing = false;
		return false;
	}

	Log_Manager->OnPutLog("드래그 버튼 업", LogType::LT_EVENT);

	m_defaultDragData.buttonID = m_downButton->GetDlgCtrlID();
	m_noteUIManager->InputDragStruct(&m_defaultDragData);
	if (m_noteUIManager->SendMessages(PM_DRAG_UP))
	{
		CURSOR_WAIT;
		DragUpState dus = m_noteUIManager->GetDragState();
		if (dus == DUS_NOTHING)
		{
			// 버그날일이 없을듯하다..
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_NOTHING) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				return false;
			}
			Log_Manager->OnPutLog("선택한 위치 : 없음", LogType::LT_PROCESS);
		}
		else if (dus == DUS_THIS)
		{
			// 버그날일이 없을듯하다..
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_THIS_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				return false;
			}
			Log_Manager->OnPutLog("선택한 위치 : 현재 시나리오", LogType::LT_PROCESS);
		}
		else if (dus == DUS_ANOTHER)
		{
			m_noteDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_SIGNAL]);
			// db처리는 다이얼로그에서.. 매니저는 ui처리만 할것
			
			NoteInformationVO note;
			if (m_noteDBManager->SelectOneNoteInformation(m_defaultDragData.noteSEQ, &note) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("노트 정보 DB 조회 실패로 인한 롤백 처리", LogType::LT_PROCESS);
				return false;
			}
			Log_Manager->OnPutLog("노트 정보 DB 조회 완료", LogType::LT_PROCESS);

			m_defaultDragData.noteCONTENT = note.GetNotCONTENT();
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_ANOTHER_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("선택한 다른 시나리오 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_ANOTHER_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			m_noteDBManager->CommitTransaction();
			Log_Manager->OnPutLog("선택한 위치 : 다른 시나리오", LogType::LT_PROCESS);
		}
		else if (dus == DUS_THIS_TIMELINE)
		{
			m_noteDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_NOTE_THIS_TIMELINE_SIGNAL]);
			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_THIS_TIMELINE_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("선택한 타임라인 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			Log_Manager->OnPutLog("선택한 위치 : 현재 타임라인", LogType::LT_PROCESS);

			// 노트의 settimeline값을 true로 주고 노트 ui를 hide 시키고 감춘 노트인덱스까지 모두 이동시킨다.
			NoteInformationVO inNote(m_defaultDragData.noteSEQ, m_thisDataStruct.scenarioData.GetSceSEQ(), true, false, "");
			if (m_noteDBManager->UpdateNoteInformationInSetTIMELINE(m_defaultDragData.noteSEQ, true) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			Log_Manager->OnPutLog("현재 노트의 타임라인 설정 정보 DB 처리 완료", LogType::LT_PROCESS);

			NoteManagerStruct noteManagerStruct(&inNote, NULL, m_defaultDragData.noteIndex);
			m_noteUIManager->InputNoteStruct(&noteManagerStruct);
			if (m_noteUIManager->SendMessages(PM_NOTE_HIDE) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("해당 노트 화면 감추기 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			Log_Manager->OnPutLog("해당 노트 화면 감추기", LogType::LT_PROCESS);

			if (UpdateScenarioList(&inNote) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("시나리오 갱신 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}

			// 스크롤 처리
			ScrollExecute(false, true);
			// 노트없앤 인덱스부터 에딧 이동
			if (MoveNote() == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("노트 위치 정보 갱신 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			
			Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
			Scenario_UI_Manager->SendMessages(PM_THIS_SCENARIO_ONE_VIEW_REFRESH);

			m_noteDBManager->CommitTransaction();
		}
		else if (dus == DUS_ANOTHER_TIMELINE)
		{
			m_noteDBManager->StartTransaction(TransactionNames[TND_DRAG_EVENT_NOTE_ANOTHER_TIMELINE_SIGNAL]);

			m_noteUIManager->InputDragStruct(&m_defaultDragData);
			if (m_noteUIManager->SendMessages(PM_DRAG_ANOTHER_TIMELINE_ATTACH) == false)
			{
				m_bDragProcessing = false;
				m_downButton = nullptr;
				CURSOR_ARROW;

				m_noteDBManager->RollbackTransaction();
				Log_Manager->OnPutLog("선택한 다른 타임라인 프로세스 처리 오류로 인한 롤백 처리", LogType::LT_PROCESS);
				m_noteUIManager->InputDragStruct(&m_defaultDragData);
				if (m_noteUIManager->SendMessages(PM_ROLLBACK_THIS_ANOTHER_TIMELINE_ATTACH) == false)
					MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

				return false;
			}
			Log_Manager->OnPutLog("선택한 위치 : 다른 타임라인", LogType::LT_PROCESS);

			Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
			Scenario_UI_Manager->SendMessages(PM_TARGET_SCENARIO_ONE_VIEW_REFRESH);

			m_noteDBManager->CommitTransaction();
		}
		CURSOR_ARROW;
	}
	else
	{
		m_bDragProcessing = false;
		m_downButton = nullptr;
		Log_Manager->OnPutLog("드래그 버튼 업 이벤트 실패", LogType::LT_PROCESS);
		return false;
	}

	m_bDragProcessing = false;
	m_downButton = nullptr;
	Log_Manager->OnPutLog("드래그 버튼 업 이벤트 완료", LogType::LT_PROCESS);

	return true;
}


BOOL NoteListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		if (m_bMainScrollFocus)
		{
			pMsg->hwnd = this->GetSafeHwnd();
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == this->GetSafeHwnd())
			m_bMainScrollFocus = true;
		else
		{
			m_bMainScrollFocus = false;
			if (DragDown(pMsg))
				return TRUE;
		}
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
	else if (WM_KEYUP == pMsg->message)
	{
		if (pMsg->wParam == VK_CONTROL)
		{
			MSG msg;
			msg.message = WM_KEYUP;
			msg.wParam = VK_CONTROL;
			GetParent()->PreTranslateMessage(&msg);
			GetParent()->SetFocus();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL NoteListCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (HIWORD(wParam) == BN_CLICKED)
	{
		// 노트매니저에 체크박스 클릭 이벤트 날리기
		m_defaultDragData.buttonID = LOWORD(wParam);
		m_noteUIManager->InputDragStruct(&m_defaultDragData);
		m_noteUIManager->SendMessages(PM_NOTE_CLICK);
		Log_Manager->OnPutLog("버튼 클릭 완료", LogType::LT_EVENT);
		return 1;
	}
	else if (HIWORD(wParam) == EN_CHANGE)
	{
		int editID = LOWORD(wParam);
		// 여기다가 에딧 변경작업 추가하기
		// 음 컨트롤러하나 상속받아서 스레드 추가해서 작업할까..
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
