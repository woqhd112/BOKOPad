// OneViewList.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "OneViewList.h"
#include "BOKOManager/DBManager/NoteDBManager.h"
#include "BOKOTimelineOneViewDlg.h"
#include "afxdialogex.h"

static unsigned int g_oneViewID = 30000;
// OneViewList 대화 상자

IMPLEMENT_DYNAMIC(OneViewList, CDialogEx)

OneViewList::OneViewList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ONE_VIEW_LIST, pParent)
	, DlgInterface(this, false)
	, m_size(0)
	, m_variableItemStart_Y(0)
	, m_downButton(nullptr)
	, m_downButtonKey(-1)
	, m_focusButtonKey(-1)
	, m_mprps(MPRPS_BUTTON_NONE)
	, m_prePos(0)
{
	Log_Manager->OnPutLog("OneViewList 생성자 호출", LogType::LT_PROCESS);
	this->Start();
	CreateFrame();
}

OneViewList::~OneViewList()
{
	scroll.DestroyWindow();
	DeleteAllItems();
	Log_Manager->OnPutLog("OneViewList 소멸자 호출", LogType::LT_PROCESS);
	this->Stop();
	m_cond.Destroy();
	this->Join();

}

void OneViewList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OneViewList, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// OneViewList 메시지 처리기


BOOL OneViewList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitFrame();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void OneViewList::Initialize()
{
	this->SetBackgroundColor(RGB(250, 250, 250));

	m_buttonFont.CreatePointFont(100, TEXT("굴림"));
	m_editFont.CreatePointFont(120, TEXT("굴림"));

	ScrollProcess::ScrollInfo info;
	info.scrollExecuteCtrl = this;
	info.wheelSize = 20;

	scroll.Create(CustomScroll::IDD, this);
	scroll.SetScrollInfo(info);
	scroll.ShowWindow(SW_HIDE);
}

void OneViewList::AttachManager(NoteDBManager* dbmanager)
{
	m_noteDBManager = dbmanager;
}

void OneViewList::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;
}

void OneViewList::InsertItem(ComplexString strText, int notSEQ)
{
	CRect rect;
	this->GetClientRect(rect);

	CustomButton* createButton = new CustomButton;
	CEdit* createEdit = new CEdit;

	ComplexString strTitle = strText.Left(10);

	OneViewListDataStruct itemData;

	createButton->Create(strTitle.GetBuffer(), WS_VISIBLE | BS_LEFT | BS_OWNERDRAW, CRect(0, 0, 0, 0), this, g_oneViewID++);
	createEdit->Create(WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_READONLY, CRect(0, 0, 0, 0), this, g_oneViewID++);

	createButton->Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);
	//createButton->SetFont(&m_buttonFont);
	createEdit->SetFont(&m_editFont);
	createButton->MoveWindow(1, m_variableItemStart_Y + 1, rect.Width() - 20, TAG_BUTTON_HEIGHT - 1);
	createEdit->MoveWindow(1, m_variableItemStart_Y + 1 + 20, rect.Width() - 20, EXPAND_EDIT_HEIGHT);
	createEdit->ShowWindow(SW_HIDE);
	createEdit->SetWindowTextA(strText.GetBuffer());

	createButton->m_nAlignStyle = CMFCButton::AlignStyle::ALIGN_LEFT;

	itemData.start_tagButton_pos_Y = m_variableItemStart_Y;
	itemData.tagButton = createButton;
	itemData.editButton = createEdit;
	itemData.expandedEdit = false;
	itemData.notSEQ = notSEQ;

	m_dataMap.insert(m_size, itemData);

	m_variableItemStart_Y += (TAG_BUTTON_HEIGHT);

	m_size++;

	if (m_size > 14)
		scroll.ExecuteScroll(SCROLL_LINE_ADD);

	Log_Manager->OnPutLog("OneViewList 아이템 등록 완료", LogType::LT_PROCESS);
}

void OneViewList::SetItem(int index, ComplexString strText)
{

}

void OneViewList::DeleteItem(int index)
{
	
}

void OneViewList::DeleteAllItems()
{
	ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();

	while (iter != m_dataMap.end())
	{
		CButton* deleteButton = iter->value.value.tagButton;
		CEdit* deleteEdit = iter->value.value.editButton;

		deleteButton->DestroyWindow();
		deleteEdit->DestroyWindow();
		delete deleteButton;
		delete deleteEdit;
		deleteButton = nullptr;
		deleteEdit = nullptr;

		iter++;
	}
	m_dataMap.clear();
	m_size = 0;
	m_variableItemStart_Y = 0;
	scroll.ResetScroll();

	Log_Manager->OnPutLog("OneViewList 아이템 모두 삭제 완료", LogType::LT_PROCESS);
}

int OneViewList::GetItemCount() const
{
	return m_size;
}

void OneViewList::GetFullItemText(ComplexString* strText)
{
	ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();

	CString strEditText;
	while (iter != m_dataMap.end())
	{
		iter->value.value.editButton->GetWindowTextA(strEditText);
		strText->AppendFormat("%s\r\n", strEditText.GetBuffer());
		iter++;
	}
}


void OneViewList::ExpandAll(bool bExpand)
{
	// 접기일땐 우선 스크롤을 맨 위로 올리고 작업한다.
	if (!bExpand)
	{
		for (int i = 0; i < scroll.GetLineCount(); i++)
		{
			OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
		}
	}

	ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
	ComplexMap<int, OneViewListDataStruct>::iterator iter1;

	int i = 0;
	while (iter != m_dataMap.end())
	{
		bool bAlreadyExpand = iter->value.value.expandedEdit;
		iter1 = m_dataMap.begin();
		while (iter1 != m_dataMap.end())
		{
			if (bExpand)
			{
				if (iter1->value.key > i && !bAlreadyExpand)
				{
					iter1->value.value.start_tagButton_pos_Y += EXPAND_EDIT_HEIGHT;
					m_variableItemStart_Y += EXPAND_EDIT_HEIGHT;
				}
			}
			else
			{
				if (iter1->value.key > i && bAlreadyExpand)
				{
					iter1->value.value.start_tagButton_pos_Y -= EXPAND_EDIT_HEIGHT;
					m_variableItemStart_Y -= EXPAND_EDIT_HEIGHT;
				}
			}

			iter1->value.value.tagButton->SetWindowPos(NULL, 1, iter1->value.value.start_tagButton_pos_Y, 0, 0, SWP_NOSIZE);
			iter1->value.value.editButton->SetWindowPos(NULL, 1, iter1->value.value.start_tagButton_pos_Y + TAG_BUTTON_HEIGHT, 0, 0, SWP_NOSIZE);
			iter1++;
		}


		if (bExpand)
		{
			iter->value.value.expandedEdit = true;
			iter->value.value.editButton->ShowWindow(SW_SHOW);
			if (!bAlreadyExpand)
			{
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				//m_prePos += 60;
			}
		}
		else
		{
			iter->value.value.expandedEdit = false;
			iter->value.value.editButton->ShowWindow(SW_HIDE);
			if (bAlreadyExpand)
			{
				scroll.ExecuteScroll(SCROLL_LINE_DELETE);
				scroll.ExecuteScroll(SCROLL_LINE_DELETE);
				scroll.ExecuteScroll(SCROLL_LINE_DELETE);
				//m_prePos -= 60;
			}
		}
		i++;
		iter++;
	}
	Log_Manager->OnPutLog("OneViewList 아이템 모두 펼치기 완료", LogType::LT_PROCESS);
}


void OneViewList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nSBCode == SB_ENDSCROLL)
		return;

	if (scroll.GetLineCount() == 0)
		return;

	int loopCount = 1;
	if (nSBCode == SB_THUMBTRACK)
	{
		int wheelSize = scroll.GetWheelSize();
		int currentPos = nPos / wheelSize;
		int prePos = m_prePos / wheelSize;

		if (prePos < currentPos)
		{
			nSBCode = SB_LINEDOWN;
		}
		else if (prePos > currentPos)
		{
			nSBCode = SB_LINEUP;
		}
		loopCount = abs(prePos - currentPos);
		if (loopCount == 0 && nPos < 20 && nPos > 0)
			loopCount = 1;
	}
	else if (nSBCode == SB_LINEDOWN || nSBCode == SB_PAGEDOWN)
	{
		nPos = scroll.GetScrollCount() * scroll.GetWheelSize() + scroll.GetWheelSize();
		if (int(nPos) >= scroll.GetLineCount() * scroll.GetWheelSize())
			nPos = scroll.GetLineCount() * scroll.GetWheelSize() - scroll.GetWheelSize();
	}
	else if (nSBCode == SB_LINEUP || nSBCode == SB_PAGEUP)
	{
		nPos = scroll.GetScrollCount() * scroll.GetWheelSize() - scroll.GetWheelSize();
		if (nPos < 0)
			nPos = 0;
	}

	for (int i = 0; i < loopCount; i++)
	{
		// 스크롤이 맨 위거나 맨 아래일경우는 처리안함 
		if (nSBCode == SB_LINEUP || nSBCode == SB_PAGEUP)
		{
			if (scroll.GetScrollCount() > 0)
			{
				ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
				while (iter != m_dataMap.end())
				{
					iter->value.value.start_tagButton_pos_Y += 20;
					iter++;
				}
				m_variableItemStart_Y += 20;
			}
		}
		else if (nSBCode == SB_LINEDOWN || nSBCode == SB_PAGEDOWN)
		{
			if (scroll.GetScrollCount() < (scroll.GetLineCount() - 1))
			{
				ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
				while (iter != m_dataMap.end())
				{
					iter->value.value.start_tagButton_pos_Y -= 20;
					iter++;
				}
				m_variableItemStart_Y -= 20;
			}
		}

		scroll.OperateScroll(nSBCode, nPos);
	}
	m_prePos = nPos;
	
	//CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL OneViewList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	OnVScroll(scroll.OperateWheel(zDelta), 0, GetScrollBarCtrl(SB_VERT));

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL OneViewList::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (HIWORD(wParam) == BN_CLICKED)
	{
		ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();

		bool bFind = false;
		bool bFindExpand = false;
		bool bLineEndClicked = false;
		while (iter != m_dataMap.end())
		{
			if (bFind)
			{
				if (bFindExpand)
				{
					iter->value.value.start_tagButton_pos_Y += EXPAND_EDIT_HEIGHT;
				}
				else
				{
					iter->value.value.start_tagButton_pos_Y -= EXPAND_EDIT_HEIGHT;
				}

				iter->value.value.tagButton->SetWindowPos(NULL, 1, iter->value.value.start_tagButton_pos_Y, 0, 0, SWP_NOSIZE);
				iter->value.value.editButton->SetWindowPos(NULL, 1, iter->value.value.start_tagButton_pos_Y + TAG_BUTTON_HEIGHT, 0, 0, SWP_NOSIZE);
			}

			if (iter->value.value.tagButton->GetDlgCtrlID() == LOWORD(wParam))
			{
				if (iter->value.value.expandedEdit == false)
				{
					iter->value.value.editButton->ShowWindow(SW_SHOW);
					iter->value.value.expandedEdit = true;
					bFindExpand = true;
				}
				else
				{
					iter->value.value.editButton->ShowWindow(SW_HIDE);
					iter->value.value.expandedEdit = false;
				}

				bFind = true;
			}
			iter++;
		}

		if (bFind)
		{
			if (bFindExpand)
			{
				m_variableItemStart_Y += EXPAND_EDIT_HEIGHT;
			}
			else
			{
				m_variableItemStart_Y -= EXPAND_EDIT_HEIGHT;
			}
		}

		// 클릭한 버튼이 맨 마지막에서 3칸이상 경우는 전부 3칸씩 아래로 내린다.
		if ((scroll.GetLineCount() - 1) == scroll.GetScrollCount() || (scroll.GetLineCount() - 2) == scroll.GetScrollCount() || (scroll.GetLineCount() - 3) == scroll.GetScrollCount())
		{
			bLineEndClicked = true;
		}
		// 클릭한 버튼이 맨마지막이고 에딧 접기일때만
		if (bLineEndClicked && !bFindExpand)
		{
			OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
			OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
			OnVScroll(SB_LINEUP, 0, GetScrollBarCtrl(SB_VERT));
		}

		if (bFindExpand)
		{
			if (m_variableItemStart_Y > 310)
			{
				if (scroll.GetLineCount() == 0)
				{
					scroll.ExecuteScroll(SCROLL_LINE_ADD);
					//m_prePos += 20;
				}

				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				//m_prePos += 20;
			}

			if (m_variableItemStart_Y > 330)
			{
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				//m_prePos += 20;
			}

			if (m_variableItemStart_Y > 350)
			{
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
				//m_prePos += 20;
			}
		}
		else
		{
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
			//m_prePos -= 60;
		}

		Log_Manager->OnPutLog("OneViewList 아이템 클릭", LogType::LT_EVENT);

		return 1;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL OneViewList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		DragDown(pMsg);
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

	return CDialogEx::PreTranslateMessage(pMsg);
}

void OneViewList::Run()
{
	while (this->IsRunning())
	{
		m_cond.Wait();
		m_nDragTime = 0;
		while (m_bDragTimer)
		{
			m_timerLock.Lock();
			m_nDragTime += 100;
			m_timerLock.UnLock();
			ComplexThread::Sleep(100);
			if (m_bDragTimer)
			{
				if (m_nDragTime >= 300)
				{
					if (m_downButton)
					{
						m_bDragProcessing = true;
						m_downButton->ShowWindow(SW_HIDE);
						
						SetCapture();

						Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
						Scenario_UI_Manager->SendMessages(PM_TIMELINE_ONEVIEW_EXPANDALL_CLOSE);
						Log_Manager->OnPutLog("드래그 버튼 다운", LogType::LT_EVENT);
					}

					m_bDragTimer = false;
					break;
				}
			}
		}
	}
}

bool OneViewList::DragDown(MSG* pMsg)
{
	UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
	if (nButtonStyle == BS_OWNERDRAW)
	{
		m_downButton = (CustomButton*)FromHandle(pMsg->hwnd);
		ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
		while (iter != m_dataMap.end())
		{
			if (iter->value.value.tagButton == m_downButton)
			{
				m_downButtonKey = iter->value.key;
				break;
			}
			iter++;
		}
		m_bDragTimer = true;
		m_cond.Signal();
	}

	return true;
}

bool OneViewList::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;

	if (m_downButton == nullptr)
		return false;

	CURSOR_CROSS;

	CRect rect;
	ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
	while (iter != m_dataMap.end())
	{
		if (iter->value.value.tagButton != m_downButton)
		{
			iter->value.value.tagButton->GetWindowRect(rect);
			if (PtInRect(rect, pMsg->pt))
			{
				// 버튼 포커싱 색 상태변경
				if (pMsg->pt.y >= (rect.top - 2) && pMsg->pt.y <= (rect.top - 2) + 4)
				{
					// 상단쪽 버튼과 버튼 사이
					m_mprps = MPRPS_BUTTON_TOP;
				}
				else if (pMsg->pt.y >= (rect.top - 2) + 4 && pMsg->pt.y <= (rect.top - 2) + ((rect.Height() + 2) / 2))
				{
					// 버튼의 상중쪽 
					m_mprps = MPRPS_BUTTON_TOP_MIDDLE;
				}
				else if (pMsg->pt.y >= (rect.top - 2) + ((rect.Height() + 2) / 2) && pMsg->pt.y <= rect.bottom - 4)
				{
					// 버튼의 중하쪽
					m_mprps = MPRPS_BUTTON_MIDDLE_BOTTOM;
				}
				else if (pMsg->pt.y >= rect.bottom - 4 && pMsg->pt.y <= rect.bottom)
				{
					// 하단쪽 버튼과 버튼 사이
					m_mprps = MPRPS_BUTTON_BOTTOM;
				}

				m_focusButtonKey = iter->value.key;
				Invalidate(FALSE);
				break;
			}

			Invalidate(FALSE);
		}
		else
			m_mprps = MPRPS_BUTTON_NONE;

		iter++;
	}

	return true;
}

bool OneViewList::DragUp(MSG* pMsg)
{
	m_bDragTimer = false;

	if (m_downButton == nullptr)
	{
		m_mprps = MPRPS_BUTTON_NONE;
		m_bDragProcessing = false;
		return false;
	}

	if (!m_bDragProcessing)
	{
		m_mprps = MPRPS_BUTTON_NONE;
		m_downButton = nullptr;
		m_downButtonKey = -1;
		return false;
	}

	m_downButton->ShowWindow(SW_SHOW);
	CURSOR_ARROW;
	ReleaseCapture();

	m_bDragProcessing = false;
	m_downButton = nullptr;
	Invalidate(FALSE);

	SortDataMapByMPRPStatus();

	m_mprps = MPRPS_BUTTON_NONE;
	m_downButtonKey = -1;
	m_focusButtonKey = -1;
	Log_Manager->OnPutLog("드래그 버튼 다운", LogType::LT_EVENT);

	return true;
}

void OneViewList::SortDataMapByMPRPStatus()
{
	// 마우스 상태값과 키값들로 현재 데이터맵을 갱신한다.

	if (m_focusButtonKey == -1)
		return;

	if (m_downButtonKey == -1)
		return;

	if (m_mprps == MPRPS_BUTTON_NONE)
		return;

	ComplexMap<int, OneViewListDataStruct>::iterator eventData = m_dataMap.find(m_focusButtonKey);
	ComplexMap<int, OneViewListDataStruct>::iterator downData = m_dataMap.find(m_downButtonKey);

	if (eventData == m_dataMap.end())
		return;

	if (downData == m_dataMap.end())
		return;

	CString baseButtonBuf, baseEditBuf, changeButtonBuf, changeEditBuf;

	eventData->value.value.tagButton->GetWindowTextA(baseButtonBuf);
	eventData->value.value.editButton->GetWindowTextA(baseEditBuf);
	downData->value.value.tagButton->GetWindowTextA(changeButtonBuf);
	downData->value.value.editButton->GetWindowTextA(changeEditBuf);

	m_noteDBManager->StartTransaction(TransactionNames[TND_TIMELINE_ONEVIEW_CHANGE]);

	// 노트디비 갱신
	if (m_noteDBManager->UpdateNoteInformationInNotContent(eventData->value.value.notSEQ, changeEditBuf.GetBuffer()) == false)
	{
		// 롤백 처리
		m_noteDBManager->RollbackTransaction();
		return;
	}

	if (m_noteDBManager->UpdateNoteInformationInNotContent(downData->value.value.notSEQ, baseEditBuf.GetBuffer()) == false)
	{
		// 롤백 처리
		m_noteDBManager->RollbackTransaction();
		return;
	}

	// 멤버 컨트롤러들 텍스트 변경
	eventData->value.value.tagButton->SetWindowTextA(changeButtonBuf);
	eventData->value.value.editButton->SetWindowTextA(changeEditBuf);
	downData->value.value.tagButton->SetWindowTextA(baseButtonBuf);
	downData->value.value.editButton->SetWindowTextA(baseEditBuf);

	// 타임라인 UI 갱신
	Scenario_UI_Manager->InputScenarioStruct(&m_thisDataStruct);
	if (Scenario_UI_Manager->SendMessages(PM_TIMELINE_RELOAD) == false)
	{
		// 롤백 처리
		m_noteDBManager->RollbackTransaction();
		return;
	}

	m_noteDBManager->CommitTransaction();
}

void OneViewList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);

	// 배경 깜빡임없이 그냥 싹 지우고 다시그리는 용도
	/*CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(&rect, RGB(255, 255, 255));*/

	if (m_focusButtonKey != -1)
	{
		CRect rect;
		this->GetClientRect(rect);
		int nScrollProcessingFocusKey = m_focusButtonKey - scroll.GetScrollCount();

		switch (m_mprps)
		{
			case MPRPS_BUTTON_TOP:
			{
				CPen* oldPen;
				CPen drawHoverPen;
				drawHoverPen.CreatePen(PS_SOLID, 3, HOVER_COLOR);
				oldPen = dc.SelectObject(&drawHoverPen);
				// 상단 가로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);

				// 상단 왼쪽 세로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey + (TAG_BUTTON_HEIGHT / 2));

				// 상단 오른쪽 세로축
				dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey + (TAG_BUTTON_HEIGHT / 2));

				if (nScrollProcessingFocusKey - 1 > -1)
				{
					oldPen = dc.SelectObject(&drawHoverPen);
					// 하단 가로축
					dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
					dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);

					// 하단 왼쪽 세로축
					dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
					dc.LineTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey - (TAG_BUTTON_HEIGHT / 2));

					// 하단 오른쪽 세로축
					dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
					dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey - (TAG_BUTTON_HEIGHT / 2));
				}

				dc.SelectObject(oldPen);

				drawHoverPen.DeleteObject();
				break;
			}

			case MPRPS_BUTTON_TOP_MIDDLE:
			{
				CPen* oldPen;
				CPen drawHoverPen;
				drawHoverPen.CreatePen(PS_SOLID, 3, HOVER_COLOR);

				oldPen = dc.SelectObject(&drawHoverPen);
				// 상단 가로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);

				// 상단 왼쪽 세로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(0, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey + (TAG_BUTTON_HEIGHT / 2));

				// 상단 오른쪽 세로축
				dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey);
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * nScrollProcessingFocusKey + (TAG_BUTTON_HEIGHT / 2));

				dc.SelectObject(oldPen);

				drawHoverPen.DeleteObject();
				break;
			}

			case MPRPS_BUTTON_MIDDLE_BOTTOM:
			{
				CPen* oldPen;
				CPen drawHoverPen;
				drawHoverPen.CreatePen(PS_SOLID, 3, HOVER_COLOR);

				oldPen = dc.SelectObject(&drawHoverPen);
				// 하단 가로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));

				// 하단 왼쪽 세로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) - (TAG_BUTTON_HEIGHT / 2));

				// 하단 오른쪽 세로축
				dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) - (TAG_BUTTON_HEIGHT / 2));

				dc.SelectObject(oldPen);

				drawHoverPen.DeleteObject();
				break;
			}

			case MPRPS_BUTTON_BOTTOM:
			{
				CPen* oldPen;
				CPen drawHoverPen;
				drawHoverPen.CreatePen(PS_SOLID, 3, HOVER_COLOR);

				oldPen = dc.SelectObject(&drawHoverPen);
				// 하단 가로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));

				// 하단 왼쪽 세로축
				dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) - (TAG_BUTTON_HEIGHT / 2));

				// 하단 오른쪽 세로축
				dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
				dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) - (TAG_BUTTON_HEIGHT / 2));

				if (nScrollProcessingFocusKey + 1 < (m_dataMap.size() - 1))
				{
					oldPen = dc.SelectObject(&drawHoverPen);
					// 하단 가로축
					dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
					dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));

					// 하단 왼쪽 세로축
					dc.MoveTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
					dc.LineTo(0, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) + (TAG_BUTTON_HEIGHT / 2));

					// 하단 오른쪽 세로축
					dc.MoveTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1));
					dc.LineTo(rect.right, TAG_BUTTON_HEIGHT * (nScrollProcessingFocusKey + 1) + (TAG_BUTTON_HEIGHT / 2));
				}

				drawHoverPen.DeleteObject();
				break;
			}

			default:
				break;
		}

	}
}


BOOL OneViewList::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
	//return __super::OnEraseBkgnd(pDC);
}


HBRUSH OneViewList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(DI_BLACK_COLOR);
		pDC->SetBkColor(DI_EDIT_COLOR);
		return m_editBrush;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
