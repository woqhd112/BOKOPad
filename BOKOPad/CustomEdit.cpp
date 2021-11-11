// CustomEdit.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "CustomEdit.h"


// CustomEdit

IMPLEMENT_DYNAMIC(CustomEdit, CEdit)

CustomEdit::CustomEdit(int notSEQ)
{
	m_notSEQ = notSEQ;
	m_bSwapping = false;
}

CustomEdit::~CustomEdit()
{
	while (m_editQueue.emptyOutputQueue() == false && m_bSwapping == false)
	{
	}
}


BEGIN_MESSAGE_MAP(CustomEdit, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CustomEdit::OnEnChange)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CustomEdit 메시지 처리기

void CustomEdit::ExecuteTimer()
{
	SetTimer(NULL, 500, NULL);
}

void CustomEdit::OnEnChange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CEdit::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strText;
	GetWindowTextA(strText);
	// 텍스트가 비어있지 않을 때만 푸시
	if (strText.IsEmpty() == false)
	{
		m_editQueue.lock();
		m_editQueue.push(strText);
		m_editQueue.unlock();
		m_bSwapping = false;
	}
}


void CustomEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_editQueue.swapQueue();
	m_bSwapping = true;
	if (m_editQueue.emptyOutputQueue() == false)
	{
		CString lastText = m_editQueue.backOutputQueue();

		Scenario_DB_Manager->UpdateNoteInformationInNotContent(m_notSEQ, lastText.GetBuffer());
		ComplexString log;
		log.Format("%d SEQ 노트 변경 DB 갱신 완료", m_notSEQ);
		Log_Manager->OnPutLog(log, LogType::LT_PROCESS);
		m_editQueue.clearOutputQueue();
	}

	CEdit::OnTimer(nIDEvent);
}
