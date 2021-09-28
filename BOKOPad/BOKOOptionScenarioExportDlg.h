#pragma once


// BOKOOptionScenarioExportDlg 대화 상자

class BOKOOptionScenarioExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BOKOOptionScenarioExportDlg)

public:

	enum ImportAndExport
	{
		SCENARIO_EXPORT = 0,
		SCENARIO_IMPORT
	};

	BOKOOptionScenarioExportDlg(ImportAndExport bind, bool* bSelected, ComplexString* selectScenarioTitle, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOOptionScenarioExportDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OPTION_EXPORT };


private:

	CListCtrl m_list_export_scenario;

	ImportAndExport m_bind;
	bool* m_bSelected;
	ComplexString* m_selectScenarioTitle;
	ComplexVector<ScenarioListVO> m_loadScenarioList;

private:

	void Initialize();
	void InsertScenario(ComplexString title, ComplexString index);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListOptionSelectScenario(NMHDR *pNMHDR, LRESULT *pResult);
};
