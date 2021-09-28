#pragma once


// BOKOOptionDlg 대화 상자

class BOKOOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BOKOOptionDlg)

public:
	BOKOOptionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OPTION };


private:

	CButton m_btn_export_file;
	CButton m_btn_import_file;
	CButton m_radio_test1;
	CButton m_radio_test2;

	PadOptionVO m_mainOptionData;
	ComplexString m_workPath;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButtonExportFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonImportFile();
};
