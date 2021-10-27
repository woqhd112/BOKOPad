// OneViewList.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "OneViewList.h"
#include "TimelineUIManager.h"
#include "TimelineDBManager.h"
#include "BOKOTimelineOneViewDlg.h"
#include "afxdialogex.h"

static unsigned int g_oneViewID = 30000;
// OneViewList 대화 상자

IMPLEMENT_DYNAMIC(OneViewList, CDialogEx)

OneViewList::OneViewList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ONE_VIEW_LIST, pParent)
	, m_size(0)
	, m_variableItemStart_Y(0)
	, m_bClickEvent(true)
{
	Log_Manager->OnPutLog("OneViewList 생성자 호출", LogType::LT_PROCESS);
}

OneViewList::~OneViewList()
{
	DeleteAllItems();
	Log_Manager->OnPutLog("OneViewList 소멸자 호출", LogType::LT_PROCESS);
}

void OneViewList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OneViewList, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// OneViewList 메시지 처리기


BOOL OneViewList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ScrollProcess::ScrollInfo info;
	info.scrollExecuteCtrl = this;
	info.wheelSize = 20;
	scroll.Init(info);
	scroll.ExecuteScroll(SCROLL_LINE_NOTHING);

	this->SetBackgroundColor(RGB(250, 250, 250));

	m_buttonFont.CreatePointFont(100, TEXT("굴림"));
	m_editFont.CreatePointFont(120, TEXT("굴림"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void OneViewList::AttachManager(TimelineUIManager* manager, TimelineDBManager* dbmanager)
{
	m_timelineUIManager = manager;
	m_timelineDBManager = dbmanager;
}

void OneViewList::SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct)
{
	m_thisDataStruct = thisDataStruct;
}

void OneViewList::InsertItem(ComplexString strText)
{
	CRect rect;
	this->GetClientRect(rect);

	CButton* createButton = new CButton;
	CEdit* createEdit = new CEdit;

	ComplexString strTitle = strText.Left(10);

	OneViewListDataStruct itemData;

	createButton->Create(strTitle.GetBuffer(), WS_VISIBLE | BS_PUSHBUTTON | BS_LEFT, CRect(0, 0, 0, 0), this, g_oneViewID++);
	createEdit->Create(WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER | ES_MULTILINE | WS_DISABLED, CRect(0, 0, 0, 0), this, g_oneViewID++);

	createButton->SetFont(&m_buttonFont);
	createEdit->SetFont(&m_editFont);
	createButton->MoveWindow(0, m_variableItemStart_Y, rect.Width(), TAG_BUTTON_HEIGHT);
	createEdit->MoveWindow(0, m_variableItemStart_Y + 20, rect.Width(), EXPAND_EDIT_HEIGHT);
	createEdit->ShowWindow(SW_HIDE);
	createEdit->SetWindowTextA(strText.GetBuffer());

	itemData.start_tagButton_pos_Y = m_variableItemStart_Y;
	itemData.tagButton = createButton;
	itemData.editButton = createEdit;
	itemData.expandedEdit = false;

	m_dataMap.insert(m_size, itemData);

	m_variableItemStart_Y += (TAG_BUTTON_HEIGHT);

	m_size++;

	if (m_size > 14)
		scroll.ExecuteScroll(SCROLL_LINE_ADD);
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

			iter1->value.value.tagButton->SetWindowPos(NULL, 0, iter1->value.value.start_tagButton_pos_Y, 0, 0, SWP_NOSIZE);
			iter1->value.value.editButton->SetWindowPos(NULL, 0, iter1->value.value.start_tagButton_pos_Y + TAG_BUTTON_HEIGHT, 0, 0, SWP_NOSIZE);
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
			}
		}
		i++;
		iter++;
	}
}


void OneViewList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (scroll.GetLineCount() == 0)
		return;

	// 스크롤이 맨 위거나 맨 아래일경우는 처리안함 
	if (nSBCode == SB_LINEUP)
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
	else if (nSBCode == SB_LINEDOWN)
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
		if (m_bClickEvent == false)
			return 0;

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

				iter->value.value.tagButton->SetWindowPos(NULL, 0, iter->value.value.start_tagButton_pos_Y, 0, 0, SWP_NOSIZE);
				iter->value.value.editButton->SetWindowPos(NULL, 0, iter->value.value.start_tagButton_pos_Y + TAG_BUTTON_HEIGHT, 0, 0, SWP_NOSIZE);
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
					scroll.ExecuteScroll(SCROLL_LINE_ADD);

				scroll.ExecuteScroll(SCROLL_LINE_ADD);
			}

			if (m_variableItemStart_Y > 330)
				scroll.ExecuteScroll(SCROLL_LINE_ADD);

			if (m_variableItemStart_Y > 350)
				scroll.ExecuteScroll(SCROLL_LINE_ADD);
		}
		else
		{
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
			scroll.ExecuteScroll(SCROLL_LINE_DELETE);
		}

		return 1;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL OneViewList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_KEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_CONTROL)
		{
			BOKOTimelineOneViewDlg* parent = (BOKOTimelineOneViewDlg*)GetParent();
			if (m_bClickEvent)
			{
				parent->SetBackgroundColor(DRAG_BK_COLOR);
				m_bClickEvent = false;
			}
			else
			{
				parent->SetBackgroundColor(BASE_BK_COLOR);
				m_bClickEvent = true;
			}
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (!m_bClickEvent)
		{
			if (DragDown(pMsg))
				return TRUE;
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (!m_bClickEvent)
		{
			if (DragUp(pMsg))
				return TRUE;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (!m_bClickEvent)
		{
			if (DragMove(pMsg))
				return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


bool OneViewList::DragDown(MSG* pMsg)
{
	UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
	if (nButtonStyle == BS_PUSHBUTTON || nButtonStyle == BS_DEFPUSHBUTTON)
	{
		CButton* downButton = (CButton*)FromHandle(pMsg->hwnd);
	}
}

bool OneViewList::DragMove(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;
}

bool OneViewList::DragUp(MSG* pMsg)
{
	if (!m_bDragProcessing)
		return false;
}