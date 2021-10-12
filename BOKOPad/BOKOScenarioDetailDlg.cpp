// BOKOScenarioDetailDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOScenarioDetailDlg.h"
#include "NoteUIManager.h"
#include "afxdialogex.h"


// BOKOScenarioDetailDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOScenarioDetailDlg, CDialogEx)
 
BOKOScenarioDetailDlg::BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_TIMELINE, pParent)
{
	m_thisDataStruct = thisDataStruct;
	m_bDragModeCheck = true;
	m_bKeyDownEvent = false;
}

BOKOScenarioDetailDlg::~BOKOScenarioDetailDlg()
{
	m_list_notePad.DestroyWindow();
}

void BOKOScenarioDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NOTE_INPUT, m_edit_note_input);
	DDX_Control(pDX, IDC_BUTTON_NOTE_INPUT, m_btn_note_input);
	DDX_Control(pDX, IDC_STATIC_NOTE_LIMIT_SIZE, m_stt_note_limit_size);
	DDX_Control(pDX, IDC_CHECK_DRAG_MODE, m_btn_drag_mode);
	DDX_Control(pDX, IDC_BUTTON_NOTE_DELETE, m_btn_note_delete);
}


BEGIN_MESSAGE_MAP(BOKOScenarioDetailDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_NOTE_INPUT, &BOKOScenarioDetailDlg::OnEnChangeEditNoteInput)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_INPUT, &BOKOScenarioDetailDlg::OnBnClickedButtonNoteInput)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_DRAG_MODE, &BOKOScenarioDetailDlg::OnBnClickedCheckDragMode)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_DELETE, &BOKOScenarioDetailDlg::OnBnClickedButtonNoteDelete)
END_MESSAGE_MAP()


// BOKOScenarioDetailDlg 메시지 처리기


void BOKOScenarioDetailDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScenarioManagerStruct scenarioStruct(m_thisDataStruct.scenarioData, m_thisDataStruct.scenarioIndex);
	Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);
	Scenario_UI_Manager->SendMessages(PM_DESTROY);

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

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL BOKOScenarioDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	m_edit_note_input.LimitText(500);

	GotoDlgCtrl(&m_edit_note_input);
	
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BOKOScenarioDetailDlg::Initialize()
{
	SetWindowTextA(m_thisDataStruct.scenarioData.GetSceTITLE());
	m_list_notePad.Create(NoteListCtrl::IDD, this);
	m_timeline.Create(Timeline::IDD, this);
	m_timeline.AttachNoteManager(m_list_notePad.m_noteUIManager);

	CRect thisRect;
	this->GetWindowRect(thisRect);
	m_timeline.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.05), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.2));
	m_list_notePad.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.3), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.3));
	m_edit_note_input.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.65), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.15));
	m_stt_note_limit_size.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.8), 100, 30);
	m_btn_note_input.MoveWindow(CAST_INT(thisRect.Width() * 0.9 - 80), CAST_INT(thisRect.Height() * 0.8), 80, 25);
	m_btn_note_delete.MoveWindow(CAST_INT(thisRect.Width() * 0.9 - 80), CAST_INT(thisRect.Height() * 0.85), 80, 25);
	m_btn_drag_mode.SetWindowPos(NULL, CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.01), 0, 0, SWP_NOSIZE);
	m_list_notePad.ShowWindow(SW_SHOW);
	m_timeline.ShowWindow(SW_SHOW);
	m_btn_drag_mode.ShowWindow(SW_HIDE);
	m_btn_drag_mode.SetCheck(TRUE);
	m_btn_note_delete.EnableWindow(FALSE);
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
	m_edit_note_input.GetWindowTextA(strInputText);

	if (strInputText.IsEmpty())
		return;

	CURSOR_WAIT;
	Scenario_DB_Manager->StartTransaction(TransactionNames[TND_NOTE_INSERT]);
	
	if (m_list_notePad.InsertNote(strInputText.GetBuffer(), true))
	{
		m_edit_note_input.SetWindowTextA("");
		m_stt_note_limit_size.SetWindowTextA("0 / 500");
		m_edit_note_input.SetFocus();
		Scenario_DB_Manager->CommitTransaction();
	}
	else
	{
		Scenario_DB_Manager->RollbackTransaction();
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

bool BOKOScenarioDetailDlg::SignalLoadScenarioList()
{
	m_list_notePad.SetScenarioManagerStruct(m_thisDataStruct);
	if (m_timeline.SetScenarioManagerStruct(m_thisDataStruct) == false)
		return false;

	if (m_list_notePad.LoadNoteInformation() == false)
		return false;

	if (m_list_notePad.LoadDraggingNote() == false)
		return false;

	return true;
}

bool BOKOScenarioDetailDlg::SignalInsertNote(ComplexString& strNoteContent, bool bNoteShow)
{
	return m_list_notePad.InsertNote(strNoteContent, bNoteShow);
}

bool BOKOScenarioDetailDlg::SignalDeleteNote(int notSEQ)
{
	return m_list_notePad.DeleteNote(notSEQ);
}

bool BOKOScenarioDetailDlg::SignalInsertTimeline(int notSEQ, POINT currentMPoint)
{
	return m_timeline.InsertTimeline(notSEQ, currentMPoint);
}

bool BOKOScenarioDetailDlg::SignalUpdateSetTIME(int notSEQ)
{
	return m_list_notePad.UpdateSetTIME(notSEQ);
}

bool BOKOScenarioDetailDlg::SignalReloadNoteList()
{
	if (m_list_notePad.DeleteAllItems() == false)
		return false;

	if (m_list_notePad.LoadNoteInformation() == false)
		return false;

	return true;
}

bool BOKOScenarioDetailDlg::SignalReloadTimeline()
{
	return m_timeline.ReloadTimeline();
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
	if (m_bDragModeCheck)
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
		}
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
		}
	}
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

	Scenario_DB_Manager->CommitTransaction();
	CURSOR_ARROW;
}
