#pragma once
#include "DlgInterface.h"
#include "ComplexMap.h"


// ScenarioListCtrl 대화 상자


struct ScenarioItemStruct
{
	ScenarioListVO scenarioVO;
	CGdipButton* uiButton;
};


typedef ScenarioItemStruct ScenarioData;

class ScenarioListCtrl : public CDialogEx, public DlgInterface
{
	DECLARE_DYNAMIC(ScenarioListCtrl)

public:
	ScenarioListCtrl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ScenarioListCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCENARIO_LIST_CTRL };


public:

	bool LoadScenarioList();

	bool InsertItem(int index, ScenarioData scenario);
	bool DeleteItem(int index);
	void DeleteAllItems();

	bool UpdateItem(int index, ScenarioData scenario);

private:

	void* CreateItemUI(int tupleIndex, int recordIndex);
	CRect GetItemPosRect(int tupleIndex, int recordIndex);
	void LoadScenarioTimeline(HWND handle);
	UINT GetNoteUIImageResourceID(int recordIndex);
	void DeleteScenario(int tupleIndex, int recordIndex);

	// key : tuple index
	// value : m_ScenarioMap
	ComplexMap<int, ComplexMap<int, ScenarioData>> m_TupleDataMap;

	ComplexVector<ScenarioListVO> m_loadScenarioList;

	CGdipButton m_btn_add;

	const int ITEM_WITDH = 25;
	const int ITEM_HEIGHT = 70;
	const int ITEM_START_POS_X = 42;
	const int ITEM_START_POS_Y = 28;
	const int ITEM_BY_ITEM_MARGIN_X = 2;
	const int ITEM_BY_ITEM_MARGIN_Y = 52;

	const int FIVE_DIVIDE_MARGIN = 45;

protected:

	virtual bool DragDown(MSG* pMsg);
	virtual bool DragMove(MSG* pMsg);
	virtual bool DragUp(MSG* pMsg);
	virtual void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedMfcbuttonAddItem();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};