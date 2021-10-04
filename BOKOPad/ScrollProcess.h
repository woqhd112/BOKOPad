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
	int m_nPageCount;	// ��ũ�� ���� (wheel ������ ����)
	int m_nScrollProcessCount;

	int m_nAllPageSize;	// ���� ���̴� ȭ��
	int m_nScrollPos;
	int m_nWheelSize;	// ��ũ�� �̵� �� ���� (LINE_DOWN, LINE_UP)

};
