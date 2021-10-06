// BOKOScenarioDetailDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOScenarioDetailDlg.h"
#include "afxdialogex.h"


// BOKOScenarioDetailDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOScenarioDetailDlg, CDialogEx)

BOKOScenarioDetailDlg::BOKOScenarioDetailDlg(ScenarioManagerStruct thisDataStruct, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_TIMELINE, pParent)
{
	m_thisDataStruct = thisDataStruct;
	m_bAddOnButton = true;
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
}


BEGIN_MESSAGE_MAP(BOKOScenarioDetailDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_NOTE_INPUT, &BOKOScenarioDetailDlg::OnEnChangeEditNoteInput)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_INPUT, &BOKOScenarioDetailDlg::OnBnClickedButtonNoteInput)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// BOKOScenarioDetailDlg 메시지 처리기


void BOKOScenarioDetailDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScenarioManagerStruct scenarioStruct(m_thisDataStruct.scenarioData, m_thisDataStruct.scenarioIndex);
	Scenario_Manager->InputScenarioStruct(&scenarioStruct);
	Scenario_Manager->SendMessages(PM_DESTROY);

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

	CRect thisRect;
	this->GetWindowRect(thisRect);
	m_timeline.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.05), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.2));
	m_list_notePad.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.3), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.3));
	m_edit_note_input.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.65), CAST_INT(thisRect.Width() * 0.85), CAST_INT(thisRect.Height() * 0.15));
	m_stt_note_limit_size.MoveWindow(CAST_INT(thisRect.Width() * 0.05), CAST_INT(thisRect.Height() * 0.8), 100, 30);
	m_btn_note_input.MoveWindow(CAST_INT(thisRect.Width() * 0.9 - 80), CAST_INT(thisRect.Height() * 0.8), 80, 25);
	m_list_notePad.ShowWindow(SW_SHOW);
	m_timeline.ShowWindow(SW_SHOW);
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

	CString strInputText;
	m_edit_note_input.GetWindowTextA(strInputText);

	if (strInputText.IsEmpty())
		return;

	CURSOR_WAIT;
	if (m_list_notePad.InsertNote(strInputText.GetBuffer()))
	{
		m_edit_note_input.SetWindowTextA("");
		m_stt_note_limit_size.SetWindowTextA("0 / 500");
		m_edit_note_input.SetFocus();
	}
	CURSOR_ARROW;
}


BOOL BOKOScenarioDetailDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::OnCommand(wParam, lParam);
}

void BOKOScenarioDetailDlg::SignalLoadScenarioList()
{
	m_list_notePad.SetScenarioManagerStruct(m_thisDataStruct);
	m_timeline.SetScenarioManagerStruct(m_thisDataStruct);
	m_list_notePad.LoadNoteInformation();
}

void BOKOScenarioDetailDlg::SignalInsertNote(ComplexString& strNoteContent)
{
	m_list_notePad.InsertNote(strNoteContent);
}

void BOKOScenarioDetailDlg::SignalDeleteNote(int notSEQ)
{
	m_list_notePad.DeleteNote(notSEQ);
}

void BOKOScenarioDetailDlg::SignalInsertTimeline(int notSEQ, POINT currentMPoint)
{
	m_timeline.InsertTimeline(notSEQ, currentMPoint);
}

void BOKOScenarioDetailDlg::SignalUpdateSetTIME(int notSEQ)
{
	m_list_notePad.UpdateSetTIME(notSEQ);
}

void BOKOScenarioDetailDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_timeline.bDetailOpen)
	{
		m_timeline.HideTimelineDetail();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

