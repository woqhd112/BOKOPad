#pragma once
#include "OneViewList.h"

// BOKOTimelineOneViewDlg 대화 상자
class NoteDBManager;

class BOKOTimelineOneViewDlg : public CDialogEx
{
	friend class OneViewList;
	DECLARE_DYNAMIC(BOKOTimelineOneViewDlg)

public:
	BOKOTimelineOneViewDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BOKOTimelineOneViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMELINE_ONE_VIEW };

	void AttachManager(NoteDBManager* dbmanager);
	void SetScenarioManagerStruct(ScenarioManagerStruct thisDataStruct);

	void Clear();
	void SetTimelineText(ComplexString& strText, int noteSEQ);
	void SetExpandCloseEvent();

public:

private:

	CFont m_editFont;
	CEdit m_edit_one_view;
	CButton m_btn_timeline_export;
	CButton m_btn_expand_all;
	CButton m_btn_preview;

	OneViewList m_list_one_view;

	bool m_bExpandedProcess;
	bool m_bPreviewProcess;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTimelineExport();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonExpandAll();
	afx_msg void OnBnClickedButtonOneView();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
