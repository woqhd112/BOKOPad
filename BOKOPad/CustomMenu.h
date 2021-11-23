#pragma once
#include "DlgInterface.h"

// CustomMenu 대화 상자

class CustomMenu;


typedef void (*EXECUTE_FUNC)();

struct CustomMenuStruct
{
	bool bExistSubMenu;
	CustomButton* mainMenu;
	CustomMenu* subMenuList;
	EXECUTE_FUNC executeCallbackFunc;
};

class CustomMenu : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(CustomMenu)

public:
	CustomMenu(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CustomMenu();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MENU };


public:

	void Init(bool bMainMenu);

	void Open(CPoint pt);

	void Close();

	CustomMenu* GetSubMenu(int mainMenuIndex);

	void AddMenu(ComplexString menuName, EXECUTE_FUNC callback = NULL);

	bool AddSubMenu(int mainMenuIndex, ComplexString menuName, EXECUTE_FUNC callback = NULL);

private:

	ComplexMap<int, CustomMenuStruct*> m_menuMap;

	int m_nMainMenuIndex;

	const int MENU_WITDH = 150;
	const int MENU_HEIGHT = 20;

	bool m_bMenuApproach;

	bool m_bMainMenu;

protected:

	virtual void Initialize();
	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
