#pragma once


// BOKOTimelineOneViewDlg 대화 상자

class BOKOTimelineOneViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BOKOTimelineOneViewDlg)

public:
	BOKOTimelineOneViewDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOTimelineOneViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMELINE_ONE_VIEW };

	void SetTimelineText(ComplexString& strText);

public:

private:

	CFont m_editFont;
	CEdit m_edit_one_view;
	CButton m_btn_timeline_export;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTimelineExport();
	virtual BOOL OnInitDialog();
};
