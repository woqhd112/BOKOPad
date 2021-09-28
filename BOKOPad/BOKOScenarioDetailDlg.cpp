// BOKOScenarioDetailDlg.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "BOKOScenarioDetailDlg.h"
#include "afxdialogex.h"


// BOKOScenarioDetailDlg 대화 상자

IMPLEMENT_DYNAMIC(BOKOScenarioDetailDlg, CDialogEx)

BOKOScenarioDetailDlg::BOKOScenarioDetailDlg(int thisIndex, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_TIMELINE, pParent)
{
	m_scenarioIndex = thisIndex;
}

BOKOScenarioDetailDlg::~BOKOScenarioDetailDlg()
{
}

void BOKOScenarioDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BOKOScenarioDetailDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// BOKOScenarioDetailDlg 메시지 처리기


void BOKOScenarioDetailDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int a = 0;
	Scenario_Manager->SendMessages(SMP_DESTROY, m_scenarioIndex);


	//CDialogEx::OnClose();
}


BOOL BOKOScenarioDetailDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		OnClose();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
