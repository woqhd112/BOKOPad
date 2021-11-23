// CustomMenu.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomMenu.h"
#include "afxdialogex.h"

static int s_menuID = 25000;

// CustomMenu 대화 상자

IMPLEMENT_DYNAMIC(CustomMenu, CDialogEx)

CustomMenu::CustomMenu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MENU, pParent)
	, DlgInterface(this, false)
{
	CreateFrame();
	m_nMainMenuIndex = 0;
	m_bMenuApproach = false;
}

CustomMenu::~CustomMenu()
{
	ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.begin();

	while (iter != m_menuMap.end())
	{
		CustomMenuStruct* deleteStruct = iter->value.value;

		CustomButton* deleteMenu = deleteStruct->mainMenu;
		deleteMenu->DestroyWindow();
		delete deleteMenu;
		deleteMenu = nullptr;

		if (deleteStruct->bExistSubMenu)
		{
			CustomMenu* deleteSubMenuList = deleteStruct->subMenuList;
			deleteSubMenuList->DestroyWindow();
			delete deleteSubMenuList;
			deleteSubMenuList = nullptr;
		}

		delete deleteStruct;
		deleteStruct = nullptr;

		iter++;
	}
	m_menuMap.clear();
}

void CustomMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CustomMenu, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CustomMenu 메시지 처리기


void CustomMenu::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.
	DrawFrame(&dc);
}


BOOL CustomMenu::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CustomMenu::Initialize()
{
	InitFrame();
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE);
}

void CustomMenu::Init(bool bMainMenu)
{
	m_bMainMenu = bMainMenu;
}

void CustomMenu::Open(CPoint pt)
{
	CPoint point = pt;
	//GetParent()->ClientToScreen(&point);
	GetParent()->ClientToScreen(&point);
	
	SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE);
	ShowWindow(SW_SHOW);
}

void CustomMenu::Close()
{
	ShowWindow(SW_HIDE);
}

void CustomMenu::AddMenu(ComplexString menuName, EXECUTE_FUNC callback)
{
	CustomButton* menus = new CustomButton;

	menus->Create(menuName.GetBuffer(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(0, 0, 0, 0), this, s_menuID++);
	menus->Initialize(DI_BUTTON_COLOR, CMFCButton::FlatStyle::BUTTONSTYLE_NOBORDERS, _T("고딕"), 16, FW_BOLD);
	menus->m_nAlignStyle = CMFCButton::AlignStyle::ALIGN_LEFT;

	SetWindowPos(NULL, 0, 0, MENU_WITDH + 4, MENU_HEIGHT * (m_nMainMenuIndex + 1) + 2 * (m_nMainMenuIndex + 2), SWP_NOMOVE);
	menus->MoveWindow(2, MENU_HEIGHT * m_nMainMenuIndex + 2 * (m_nMainMenuIndex + 1), MENU_WITDH, MENU_HEIGHT);

	CustomMenuStruct* cms = new CustomMenuStruct;
	cms->bExistSubMenu = false;
	cms->mainMenu = menus;
	cms->subMenuList = nullptr;
	cms->executeCallbackFunc = callback;

	m_menuMap.insert(m_nMainMenuIndex++, cms);
}

CustomMenu* CustomMenu::GetSubMenu(int mainMenuIndex)
{
	ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.find(mainMenuIndex);
	if (iter == m_menuMap.end())
		return nullptr;

	if (iter->value.value->bExistSubMenu == false)
		return nullptr;

	return iter->value.value->subMenuList;
}

bool CustomMenu::AddSubMenu(int mainMenuIndex, ComplexString menuName, EXECUTE_FUNC callback)
{
	ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.find(mainMenuIndex);
	if (iter == m_menuMap.end())
		return false;

	if (iter->value.value->bExistSubMenu == false)
	{
		iter->value.value->bExistSubMenu = true;
		CustomMenu* newSubMenuList = new CustomMenu(false);
		newSubMenuList->Create(CustomMenu::IDD, this);
		newSubMenuList->ShowWindow(SW_HIDE);
		newSubMenuList->Init(false);
		iter->value.value->subMenuList = newSubMenuList;
		//iter->value.value->executeCallbackFunc = callback;

		CString strText, strChangeText;
		iter->value.value->mainMenu->GetWindowTextA(strText);

		strChangeText.Format(">> %s", strText);
		iter->value.value->mainMenu->SetWindowTextA(strChangeText);
	}

	iter->value.value->subMenuList->AddMenu(menuName, callback);

	return true;
}

void CustomMenu::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	switch (nState)
	{
		case WA_INACTIVE:	// 포커스 잃었을 때
		{
			ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.begin();
			bool bFind = false;
			while (iter != m_menuMap.end())
			{
				if (iter->value.value->bExistSubMenu)
				{
					if (iter->value.value->subMenuList->GetSafeHwnd() == pWndOther->GetSafeHwnd())
					{
						bFind = true;
						break;
					}
				}

				iter++;
			}

			if (bFind == false)
				this->ShowWindow(SW_HIDE);

			break;
		}
		case WA_ACTIVE:		// 포커스를 얻었을 때
		{
			/*ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.begin();
			bool bFind = false;
			while (iter != m_menuMap.end())
			{
				if (iter->value.value->bExistSubMenu)
				{
					iter->value.value->subMenuList->Close();
				}

				iter++;
			}*/

			break;
		}
		case WA_CLICKACTIVE:	// 클릭으로 포커스를 얻었을 떄
		{
			break;
		}

		default:
			break;
	}

}


BOOL CustomMenu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_MOUSEMOVE)
	{
		ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.begin();
		CRect rect;

		bool bFind = false;
		while (iter != m_menuMap.end())
		{
			iter->value.value->mainMenu->GetWindowRect(rect);
			if (PtInRect(rect, pMsg->pt))
			{
				if (m_bMenuApproach == false)
				{
					m_bMenuApproach = true;
					if (iter->value.value->bExistSubMenu)
					{
						CRect thisRect;
						iter->value.value->mainMenu->GetWindowRect(thisRect);
						iter->value.value->subMenuList->SetWindowPos(NULL, thisRect.right, thisRect.top - 2, 0, 0, SWP_NOSIZE);
						iter->value.value->subMenuList->ShowWindow(SW_SHOW);
					}
				}
				bFind = true;
				break;
			}
			else
			{
				/*if (iter->value.value->bExistSubMenu)
				{
					iter->value.value->subMenuList->ShowWindow(SW_HIDE);
				}*/
			}

			iter++;
		}

		if (bFind == false)
		{
			m_bMenuApproach = false;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}


BOOL CustomMenu::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ComplexMap<int, CustomMenuStruct*>::iterator iter = m_menuMap.begin();

	int ctlID = LOWORD(wParam);
	while (iter != m_menuMap.end())
	{
		int targetID = iter->value.value->mainMenu->GetDlgCtrlID();
		if (ctlID == targetID)
		{
			if (iter->value.value->bExistSubMenu)
			{
				CRect thisRect;
				iter->value.value->mainMenu->GetWindowRect(thisRect);
				iter->value.value->subMenuList->SetWindowPos(NULL, thisRect.right, thisRect.top - 2, 0, 0, SWP_NOSIZE);
				iter->value.value->subMenuList->ShowWindow(SW_SHOW);
			}
			else
			{
				// 함수 실행
				if (iter->value.value->executeCallbackFunc != NULL)
					iter->value.value->executeCallbackFunc();

				Close();
			}
			break;
		}

		iter++;
	}


	return __super::OnCommand(wParam, lParam);
}

bool CustomMenu::DragDown(MSG* pMsg)
{
	return false;
}

bool CustomMenu::DragMove(MSG* pMsg)
{
	return false;
}

bool CustomMenu::DragUp(MSG* pMsg)
{
	return false;
}