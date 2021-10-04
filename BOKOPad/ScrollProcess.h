#pragma once

#define SCROLL_LINE_ADD 1
#define SCROLL_LINE_DELETE 0

class ScrollProcess
{
public:

	ScrollProcess();
	~ScrollProcess();

	struct ScrollInfo
	{
		CWnd* scrollExecuteCtrl;
		int wheelSize;
	};

	void Init(ScrollInfo& info);
	void ExecuteScroll(int scrollLineFlag);
	bool OperateScroll(int nSBCode, int nPos);
	UINT OperateWheel(short zDelta);

	int GetLineCount() const;
	int GetScrollCount() const;
	int GetScrollSize() const;

private:

	void ProcessScrollCount(int nSBCode);

	bool m_bInit;
	CWnd* m_pProcessDlg;
	int m_nPageCount;	// 스크롤 개수 (wheel 단위로 계산됨)
	int m_nScrollProcessCount;

	int m_nAllPageSize;	// 현재 보이는 화면
	int m_nScrollPos;
	int m_nWheelSize;	// 스크롤 이동 휠 단위 (LINE_DOWN, LINE_UP)

};
