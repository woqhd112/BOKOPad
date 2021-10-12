// OneViewList.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "OneViewList.h"
#include "TimelineUIManager.h"
#include "TimelineDBManager.h"
#include "afxdialogex.h"

static unsigned int g_oneViewID = 30000;
// OneViewList 대화 상자

IMPLEMENT_DYNAMIC(OneViewList, CDialogEx)

OneViewList::OneViewList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ONE_VIEW_LIST, pParent)
	, m_size(0)
	, m_variableItemStart_Y(0)
{

}

OneViewList::~OneViewList()
{
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

void OneViewList::ExpandAll(bool bExpand)
{
	ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();
	ComplexMap<int, OneViewListDataStruct>::iterator iter1;

	int i = 0;
	while (iter != m_dataMap.end())
	{
		iter1 = m_dataMap.begin();
		while (iter1 != m_dataMap.end())
		{
			if (bExpand)
			{
				iter1->value.value.expandedEdit = true;
				iter1->value.value.editButton->ShowWindow(SW_SHOW);
				if (iter1->value.key > i)
				{
					iter1->value.value.start_tagButton_pos_Y += EXPAND_EDIT_HEIGHT;
				}
			}
			else
			{
				iter1->value.value.expandedEdit = false;
				iter1->value.value.editButton->ShowWindow(SW_HIDE);
				if (iter1->value.key > i)
				{
					iter1->value.value.start_tagButton_pos_Y -= EXPAND_EDIT_HEIGHT;
				}
			}

			iter1->value.value.tagButton->SetWindowPos(NULL, 0, iter1->value.value.start_tagButton_pos_Y, 0, 0, SWP_NOSIZE);
			iter1->value.value.editButton->SetWindowPos(NULL, 0, iter1->value.value.start_tagButton_pos_Y + TAG_BUTTON_HEIGHT, 0, 0, SWP_NOSIZE);
			iter1++;
		}

		i++;
		iter++;
	}
}


void OneViewList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
		ComplexMap<int, OneViewListDataStruct>::iterator iter = m_dataMap.begin();

		bool bFind = false;
		bool bFindExpand = false;
		while (iter != m_dataMap.end())
		{
			if (bFind)
			{
				if (bFindExpand)
					iter->value.value.start_tagButton_pos_Y += (EXPAND_EDIT_HEIGHT - scroll.GetScrollCount() * 20);
				else
					// 스크롤시 동작하게.. 이상함
					iter->value.value.start_tagButton_pos_Y -= (EXPAND_EDIT_HEIGHT);

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
		
		return 1;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
