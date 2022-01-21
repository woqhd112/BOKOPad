// BOKOScenarioDetailDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOScenarioDetailDlg.h"
#include "BOKOManager/UIManager/NoteUIManager.h"
#include "afxdialogex.h"


// BOKOScenarioDetailDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOScenarioDetailDlg, CDialogEx)
 
BOKOScenarioDetailDlg::BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_TIMELINE, pParent)
	, DlgInterface(this, true)
{
	CreateFrame(FDT_LIGHT_DLG);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_thisDataStruct = thisDataStruct;
	m_bDragModeCheck = true;
	m_bKeyDownEvent = false;
	Log_Manager->OnPutLog("BOKOScenarioDetailDlg 생성자 호출", LogType::LT_PROCESS);
}

BOKOScenarioDetailDlg::~BOKOScenarioDetailDlg()
{
	m_list_notePad.DestroyWindow();
	m_edit_note_input.DestroyWindow();
	m_setFont.DeleteObject();
	Log_Manager->OnPutLog("BOKOScenarioDetailDlg 소멸자 호출", LogType::LT_PROCESS);
}

void BOKOScenarioDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_NOTE_INPUT, m_edit_note_input);
	DDX_Control(pDX, IDC_BUTTON_NOTE_INPUT, m_btn_note_input);
	DDX_Control(pDX, IDC_STATIC_NOTE_LIMIT_SIZE, m_stt_note_limit_size);
	DDX_Control(pDX, IDC_CHECK_DRAG_MODE, m_btn_drag_mode);
	DDX_Control(pDX, IDC_BUTTON_NOTE_DELETE, m_btn_note_delete);
	DDX_Control(pDX, IDC_STATIC_TIMELINE_COUNT, m_stt_timeline_count);
	DDX_Control(pDX, IDC_PROGRESS_TIMELINE_COUNT, m_progress_timeline_count);
}


BEGIN_MESSAGE_MAP(BOKOScenarioDetailDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_NOTE_INPUT, &BOKOScenarioDetailDlg::OnEnChangeEditNoteInput)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_INPUT, &BOKOScenarioDetailDlg::OnBnClickedButtonNoteInput)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_DRAG_MODE, &BOKOScenarioDetailDlg::OnBnClickedCheckDragMode)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_DELETE, &BOKOScenarioDetailDlg::OnBnClickedButtonNoteDelete)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// BOKOScenarioDetailDlg 메시지 처리기


void BOKOScenarioDetailDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Log_Manager->OnPutLog("시나리오 화면 x 클릭", LogType::LT_EVENT);
	ScenarioManagerStruct scenarioStruct(m_thisDataStruct.scenarioData, m_thisDataStruct.scenarioIndex);
	Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);
	Scenario_UI_Manager->SendMessages(PM_DESTROY);
	Log_Manager->OnPutLog("시나리오 화면 종료", LogType::LT_OPERATE);
	//CDialogEx::OnClose();
}


BOOL BOKOScenarioDetailDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnClose();
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonNoteInput();
			return TRUE;
		}
		else if (pMsg->wParam == VK_CONTROL)
		{
			if (!m_bKeyDownEvent)
			{
				m_bKeyDownEvent = true;
				OnBnClickedCheckDragMode();
				return TRUE;
			}
		}
	}
	else if (WM_KEYUP == pMsg->message)
	{
		if (pMsg->wParam == VK_CONTROL)
		{
			if (m_bKeyDownEvent)
			{
				m_bKeyDownEvent = false;
				OnBnClickedCheckDragMode();
				return TRUE;
			}
		}
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		TitleBarActiveMove(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL BOKOScenarioDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//SetBackgroundColor(RGB(68, 68, 68));

	SetIcon(m_hIcon, TRUE);
	Initialize();

	m_edit_note_input.LimitText(500);

	GotoDlgCtrl(&m_edit_note_input);

	InitFrame(m_strTitleText);
	
	m_stt_timeline_count.SetWindowTextA("0 / 100");
	m_progress_timeline_count.SetPos(0);

	m_setFont.CreatePointFont(120, TEXT("굴림"));

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOScenarioDetailDlg::Initialize()
{
	m_strTitleText.Format("%s 타임라인", m_thisDataStruct.scenarioData.GetSceTITLE().GetBuffer());
	SetWindowTextA(m_strTitleText);
	m_list_notePad.Create(NoteListCtrl::IDD, this);
	m_edit_note_input.Create(CustomEditCtrl::IDD, this);
	Log_Manager->OnPutLog("노트 화면 생성 완료", LogType::LT_OPERATE);
	m_timeline.Create(Timeline::IDD, this);
	Log_Manager->OnPutLog("타임라인 화면 생성 완료", LogType::LT_OPERATE);
	m_timeline.AttachNoteManager(m_list_notePad.m_noteUIManager, m_list_notePad.m_noteDBManager);
	Log_Manager->OnPutLog("타임라인 UI 매니저 연결", LogType::LT_PROCESS);


	//m_timeline.SetBackgroundColor(DI_SUB_BK_COLOR);
	//m_list_notePad.SetBackgroundColor(DI_SUB_BK_COLOR);

	/*m_progress_timeline_count.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_stt_timeline_count.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_progress_timeline_count.BringWindowToTop();
	m_stt_timeline_count.BringWindowToTop();*/

	CRect thisRect;
	this->GetWindowRect(thisRect);
	m_progress_timeline_count.MoveWindow(CAST_INT(thisRect.Width() * 0.95 - 100), CAST_INT(thisRect.Height() * 0.07), 100, 20);
	m_stt_timeline_count.MoveWindow(CAST_INT(thisRect.Width() * 0.95 - 50), CAST_INT(thisRect.Height() * 0.07), 50, 20);
	m_timeline.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.1), CAST_INT(thisRect.Width() * 0.9), CAST_INT(thisRect.Height() * 0.13));
	m_list_notePad.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.28), CAST_INT(thisRect.Width() * 0.9), CAST_INT(thisRect.Height() * 0.37));
	m_edit_note_input.MoveWindows(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.7), CAST_INT(thisRect.Width() * 0.9), CAST_INT(thisRect.Height() * 0.15));
	m_stt_note_limit_size.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.85), 100, 30);
	m_btn_note_input.MoveWindow(CAST_INT(thisRect.Width() * 0.95 - 80), CAST_INT(thisRect.Height() * 0.85), 80, 25);
	m_btn_note_delete.MoveWindow(CAST_INT(thisRect.Width() * 0.95 - 80), CAST_INT(thisRect.Height() * 0.9), 80, 25);
	m_btn_drag_mode.SetWindowPos(NULL, CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.01), 0, 0, SWP_NOSIZE);
	m_list_notePad.ShowWindow(SW_SHOW);
	m_timeline.ShowWindow(SW_SHOW);
	m_btn_drag_mode.ShowWindow(SW_HIDE);
	m_btn_drag_mode.SetCheck(TRUE);
	//m_btn_note_delete.EnableWindow(FALSE);
	Log_Manager->OnPutLog("시나리오 화면 초기화", LogType::LT_PROCESS);

	m_edit_note_input.Initialize();
	m_list_notePad.Initialize();
	m_btn_note_input.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);
	m_btn_note_delete.Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);
	m_progress_timeline_count.SetBarColor(RGB(72, 205, 22));
	m_progress_timeline_count.SetBkColor(DI_SUB_BK_COLOR);
	m_edit_note_input.SetFont(&m_setFont);

	m_edit_note_input.ShowWindow(SW_SHOW);
	m_stt_timeline_count.ShowWindow(SW_HIDE);
}

void BOKOScenarioDetailDlg::SetCtrlByTimelineCount()
{
	int timelineCount = m_timeline.GetTimelineCount();
	CString strTimelineCount;
	strTimelineCount.Format("%d / %s", timelineCount, LIMIT_TIMELINE_COUNT_TEXT);
	m_stt_timeline_count.SetWindowTextA(strTimelineCount);

	m_progress_timeline_count.SetPos(timelineCount * 100 / LIMIT_TIMELINE_COUNT);
}

void BOKOScenarioDetailDlg::OnEnChangeEditNoteInput()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strInputText, strOutputText;
	m_edit_note_input.GetWindowTextA(strInputText);
	int textLength = strInputText.GetLength();

	strOutputText.Format(LIMIT_TEXT_FORMAT, textLength);
	m_stt_note_limit_size.SetWindowTextA(strOutputText);
}


void BOKOScenarioDetailDlg::OnBnClickedButtonNoteInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택모드일때 등록안되게
	if (m_bDragModeCheck == false)
		return;

	CString strInputText;
	m_edit_note_input.GetWindowTexts(strInputText);

	if (strInputText.IsEmpty())
		return;

	CURSOR_WAIT;
	Scenario_DB_Manager->StartTransaction(TransactionNames[TND_NOTE_INSERT]);
	
	if (m_list_notePad.InsertNote(strInputText.GetBuffer(), true))
	{
		m_edit_note_input.SetWindowTexts("");
		m_stt_note_limit_size.SetWindowTextA("0 / 500");
		m_edit_note_input.SetFocus();
		Scenario_DB_Manager->CommitTransaction();
		Log_Manager->OnPutLog("노트 입력 성공", LogType::LT_EVENT);
	}
	else
	{
		Scenario_DB_Manager->RollbackTransaction();
		Log_Manager->OnPutLog("노트 입력 실패로 인한 롤백 처리", LogType::LT_PROCESS);
		ScenarioManagerStruct scenarioStruct(m_thisDataStruct.scenarioData, m_thisDataStruct.scenarioIndex);
		Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);
		if (Scenario_UI_Manager->SendMessages(PM_NOTE_RELOAD) == false)
			MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");
	}

	CURSOR_ARROW;
}


BOOL BOKOScenarioDetailDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	

	return CDialogEx::OnCommand(wParam, lParam);
}

int BOKOScenarioDetailDlg::SignalGetTimelineCount()
{
	return m_timeline.GetTimelineCount();
}

bool BOKOScenarioDetailDlg::SignalLoadScenarioList()
{
	m_list_notePad.SetScenarioManagerStruct(m_thisDataStruct);
	if (m_timeline.SetScenarioManagerStruct(m_thisDataStruct) == false)
		return false;

	SetCtrlByTimelineCount();

	if (m_list_notePad.LoadNoteInformation() == false)
		return false;

	if (m_list_notePad.LoadDraggingNote() == false)
		return false;

	return true;
}

bool BOKOScenarioDetailDlg::SignalInsertNote(ComplexString strNoteContent, bool bNoteShow)
{
	bool result = m_list_notePad.InsertNote(strNoteContent, bNoteShow);

	if (result)
	{
		SetCtrlByTimelineCount();
	}

	return result;
}

bool BOKOScenarioDetailDlg::SignalDeleteNote(int notSEQ)
{
	bool result = m_list_notePad.DeleteNote(notSEQ);

	if (result)
	{
		SetCtrlByTimelineCount();
	}

	return result;
}

bool BOKOScenarioDetailDlg::SignalInsertTimeline(int notSEQ, POINT currentMPoint)
{
	bool result = m_timeline.InsertTimeline(notSEQ, currentMPoint);

	if (result)
	{
		SetCtrlByTimelineCount();
	}

	return result;
}

bool BOKOScenarioDetailDlg::SignalUpdateSetTIME(int notSEQ, bool setTIME)
{
	bool result = m_list_notePad.UpdateSetTIME(notSEQ, setTIME);

	if (result)
	{
		SetCtrlByTimelineCount();
	}

	return result;
}

bool BOKOScenarioDetailDlg::SignalReloadNoteList()
{
	if (m_list_notePad.DeleteAllItems() == false)
		return false;

	if (m_list_notePad.LoadNoteInformation() == false)
		return false;

	return true;
}

void BOKOScenarioDetailDlg::SignalTimelineOneViewRefresh()
{
	m_timeline.OneViewRefresh();
}

bool BOKOScenarioDetailDlg::SignalReloadTimeline()
{
	bool result = m_timeline.ReloadTimeline();

	if (result)
	{
		SetCtrlByTimelineCount();
	}

	return result;
}

void BOKOScenarioDetailDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_timeline.m_bDetailOpen)
	{
		m_timeline.HideTimelineDetail();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void BOKOScenarioDetailDlg::OnBnClickedCheckDragMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*if (m_bDragModeCheck)
	{
		m_btn_drag_mode.SetCheck(FALSE);
		m_bDragModeCheck = false;
		m_btn_note_delete.EnableWindow(TRUE);
		m_btn_note_input.EnableWindow(FALSE);
		if (m_list_notePad.UnloadDraggingNote() == false)
		{
			m_btn_drag_mode.SetCheck(TRUE);
			m_btn_note_delete.EnableWindow(FALSE);
			m_btn_note_input.EnableWindow(TRUE);
			m_bDragModeCheck = true;
			SignalReloadNoteList();
			Log_Manager->OnPutLog("클릭모드 정보 로드 오류로 인해 노트 갱신", LogType::LT_PROCESS);
			return;
		}
		Log_Manager->OnPutLog("클릭모드 On", LogType::LT_EVENT);
	}
	else
	{
		m_btn_drag_mode.SetCheck(TRUE);
		m_bDragModeCheck = true;
		m_btn_note_delete.EnableWindow(FALSE);
		m_btn_note_input.EnableWindow(TRUE);
		if (m_list_notePad.LoadDraggingNote() == false)
		{
			m_btn_drag_mode.SetCheck(FALSE);
			m_btn_note_delete.EnableWindow(TRUE);
			m_btn_note_input.EnableWindow(FALSE);
			m_bDragModeCheck = false;
			SignalReloadNoteList();
			Log_Manager->OnPutLog("클릭모드 정보 로드 오류로 인해 노트 갱신", LogType::LT_PROCESS);
			return;
		}
		Log_Manager->OnPutLog("드래그모드 On", LogType::LT_EVENT);
	}*/
}


void BOKOScenarioDetailDlg::OnBnClickedButtonNoteDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CURSOR_WAIT;
	Scenario_DB_Manager->StartTransaction(TransactionNames[TND_NOTE_CHECK_DELETE]);
	if (m_list_notePad.CheckDeleteNote() == false)
	{
		CURSOR_ARROW;
		Scenario_DB_Manager->RollbackTransaction();

		if (SignalReloadNoteList() == false)
			MessageBox("데이터 충돌이 났습니다. 재 접속 부탁드립니다.");

		return;
	}
	Log_Manager->OnPutLog("노트 삭제 완료", LogType::LT_EVENT);

	Scenario_DB_Manager->CommitTransaction();
	CURSOR_ARROW;
}


void BOKOScenarioDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}


HBRUSH BOKOScenarioDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	HBRUSH returnHBR = CtlColors(pDC, pWnd, nCtlColor);
	if (returnHBR != NULL)
		return returnHBR;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == m_stt_note_limit_size.GetDlgCtrlID())
		{
			pDC->SetBkColor(DI_LIGHT_COLOR);
			pDC->SetTextColor(DI_TEXT_COLOR);
			return m_staticLightBrush;
		}
		else if (pWnd->GetDlgCtrlID() == m_stt_timeline_count.GetDlgCtrlID())
		{
			pDC->SetBkMode(TRANSPARENT);
			CBrush B;
			B.CreateStockObject(NULL_BRUSH);
			return B;
		}
	}
	else if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (pWnd->GetDlgCtrlID() == m_edit_note_input.GetDlgCtrlID())
		{
			pDC->SetBkColor(DI_EDIT_COLOR);
			return m_editBrush;
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void BOKOScenarioDetailDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.
	DrawFrame(&dc);
}


bool BOKOScenarioDetailDlg::DragDown(MSG* pMsg)
{
	return false;
}

bool BOKOScenarioDetailDlg::DragMove(MSG* pMsg)
{
	return false;
}

bool BOKOScenarioDetailDlg::DragUp(MSG* pMsg)
{
	return false;
}