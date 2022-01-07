// ScenarioListCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "BOKOPad.h"
#include "ScenarioListCtrl.h"
#include "BOKOScenarioAddDlg.h"
#include "afxdialogex.h"

static int g_scenarioItemID = 15000;
// ScenarioListCtrl 대화 상자

IMPLEMENT_DYNAMIC(ScenarioListCtrl, CDialogEx)

ScenarioListCtrl::ScenarioListCtrl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCENARIO_LIST_CTRL, pParent)
	, DlgInterface(this, false)
{
	CreateFrame(FDT_MAIN_DLG);
}

ScenarioListCtrl::~ScenarioListCtrl()
{
	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.begin();
	while (tupleIter != m_TupleDataMap.end())
	{
		ComplexMap<int, ScenarioData>::iterator recordIter = tupleIter->value.value.begin();
		while (recordIter != tupleIter->value.value.end())
		{
			// UI삭제 
			CGdipButton* deleteButton = recordIter->value.value.uiButton;
			deleteButton->DestroyWindow();
			delete deleteButton;
			deleteButton = nullptr;

			recordIter++;
		}
		tupleIter->value.value.clear();
		tupleIter++;
	}
	m_TupleDataMap.clear();
}

void ScenarioListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_ADD_ITEM, m_btn_add);
}


BEGIN_MESSAGE_MAP(ScenarioListCtrl, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_ADD_ITEM, &ScenarioListCtrl::OnBnClickedMfcbuttonAddItem)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ScenarioListCtrl 메시지 처리기


BOOL ScenarioListCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool ScenarioListCtrl::LoadScenarioList()
{
	CURSOR_WAIT;

	// 시나리오 리스트 로드
	if (Scenario_DB_Manager->SelectAllScenarioList(&m_loadScenarioList))
	{
		ComplexVector<ScenarioListVO>::iterator iter = m_loadScenarioList.begin();

		int i = 0;
		while (iter != m_loadScenarioList.end())
		{
			ComplexString index = ComplexConvert::IntToString(iter->value.GetSceSEQ());
			ComplexString title = iter->value.GetSceTITLE();

			ScenarioItemStruct item;
			item.scenarioVO = iter->value;
			InsertItem(i, item);
			iter++;
			i++;
		}
		Log_Manager->OnPutLog("시나리오 정보 로드", LogType::LT_PROCESS);
		CURSOR_ARROW;
		return true;
	}

	CURSOR_ARROW;
	return false;
}


BOOL ScenarioListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_MOUSEMOVE)
	{
		UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
		if (nButtonStyle == BS_OWNERDRAW)
		{
			CURSOR_HAND;
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		UINT nButtonStyle = GetWindowLongA(pMsg->hwnd, GWL_STYLE) & 0x0000000F;
		if (nButtonStyle == BS_OWNERDRAW)
		{
			CURSOR_HAND;
		}
	}
	else if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		LoadScenarioTimeline(pMsg->hwnd);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void ScenarioListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	DrawFrame(&dc);
}


HBRUSH ScenarioListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

bool ScenarioListCtrl::DragDown(MSG* pMsg)
{
	return false;
}

bool ScenarioListCtrl::DragMove(MSG* pMsg)
{
	return false;
}

bool ScenarioListCtrl::DragUp(MSG* pMsg)
{
	return false;
}

void ScenarioListCtrl::Initialize()
{
	InitFrame();
	m_btn_add.LoadStdImage(IDB_PNG_NOTE_ADD, "PNG");
	m_btn_add.LoadHovImage(IDB_PNG_NOTE_ADD, "PNG");
	m_btn_add.LoadAltImage(IDB_PNG_NOTE_ADD, "PNG");
	m_btn_add.MoveWindow(GetItemPosRect(0, 0));
}

bool ScenarioListCtrl::InsertItem(int index, ScenarioData scenario)
{
	int recordIndex = index % ITEM_LIMIT_RECORD_COUNT;
	int tupleIndex = index / ITEM_LIMIT_RECORD_COUNT;

	// 아이템 총 갯수 초과 시 
	if (index > ITEM_LIMIT_TOTAL_COUNT)
		return false;

	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.find(tupleIndex);
	if (tupleIter == m_TupleDataMap.end())
	{
		// 튜플을 새로 등록해야 하는 경우
		ComplexMap<int, ScenarioData> recordScenarioMap;

		scenario.uiButton = (CGdipButton*)CreateItemUI(tupleIndex, recordIndex);

		recordScenarioMap.insert(recordIndex, scenario);
		m_TupleDataMap.insert(tupleIndex, recordScenarioMap);
	}
	else
	{
		// 존재하는 튜플에 등록해야 하는 경우
		ComplexMap<int, ScenarioData>::iterator recordIter = tupleIter->value.value.find(recordIndex);
		if (recordIter == tupleIter->value.value.end())
		{
			// 레코드를 새로 등록해야 하는 경우
			scenario.uiButton = (CGdipButton*)CreateItemUI(tupleIndex, recordIndex);
			tupleIter->value.value.insert(recordIndex, scenario);
		}
		else
			return false;	// 레코드가 존재 할 시 (이 조건은 안타겠지만 처리함)
	}

	// 아이템 등록이 끝나면 추가버튼은 그 다음으로 이동한다.
	int nextRecordIndex = (index + 1) % ITEM_LIMIT_RECORD_COUNT;
	int nextTupleIndex = (index + 1) / ITEM_LIMIT_RECORD_COUNT;

	if (nextTupleIndex > ITEM_LIMIT_TUPLE_COUNT)
	{
		m_btn_add.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btn_add.MoveWindow(GetItemPosRect(nextTupleIndex, nextRecordIndex));
	}


	return true;
}

bool ScenarioListCtrl::DeleteItem(int index)
{
	int recordIndex = index % ITEM_LIMIT_RECORD_COUNT;
	int tupleIndex = index / ITEM_LIMIT_RECORD_COUNT;

	// 아이템 총 갯수 초과 시 
	if (index > ITEM_LIMIT_TOTAL_COUNT)
		return false;

	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.find(tupleIndex);
	// 튜플이 존재하지 않을 시
	if (tupleIter == m_TupleDataMap.end())
		return false;

	ComplexMap<int, ScenarioData>::iterator recordIter = tupleIter->value.value.find(recordIndex);
	// 레코드가 존재하지 않을 시
	if (recordIter == tupleIter->value.value.end())
		return false;

	CGdipButton* deleteButton = recordIter->value.value.uiButton;
	deleteButton->DestroyWindow();
	delete deleteButton;
	deleteButton = nullptr;
	tupleIter->value.value.erase(recordIndex);

	DeleteScenario(tupleIndex, recordIndex);

	// 아이템들 위치 이동 및 맵의 키 갱신

	// 지우면 무조건 추가버튼 show한다.
	m_btn_add.ShowWindow(SW_SHOW);

	return true;
}

void ScenarioListCtrl::DeleteAllItems()
{
	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.begin();
	while (tupleIter != m_TupleDataMap.end())
	{
		ComplexMap<int, ScenarioData>::iterator recordIter = tupleIter->value.value.begin();
		while (recordIter != tupleIter->value.value.end())
		{
			// UI삭제 
			CGdipButton* deleteButton = recordIter->value.value.uiButton;
			deleteButton->DestroyWindow();
			delete deleteButton;
			deleteButton = nullptr;

			recordIter++;
		}
		tupleIter->value.value.clear();
		tupleIter++;
	}
	m_TupleDataMap.clear();

	m_loadScenarioList.clear();
	Scenario_DB_Manager->DeleteAllScenarioList();
}


bool ScenarioListCtrl::UpdateItem(int index, ScenarioData scenario)
{
	int recordIndex = index % ITEM_LIMIT_RECORD_COUNT;
	int tupleIndex = index / ITEM_LIMIT_RECORD_COUNT;

	// 아이템 총 갯수 초과 시 
	if (index > ITEM_LIMIT_TOTAL_COUNT)
		return false;

	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.find(tupleIndex);
	// 튜플이 존재하지 않을 시
	if (tupleIter == m_TupleDataMap.end())
		return false;

	ComplexMap<int, ScenarioData>::iterator recordIter = tupleIter->value.value.find(recordIndex);
	// 레코드가 존재하지 않을 시
	if (recordIter == tupleIter->value.value.end())
		return false;

	tupleIter->value.value.erase(recordIndex);
	tupleIter->value.value.insert(recordIndex, scenario);

	return true;
}

void* ScenarioListCtrl::CreateItemUI(int tupleIndex, int recordIndex)
{
	CRect itemPosRect = GetItemPosRect(tupleIndex, recordIndex);

	CGdipButton* newScenaioButton = new CGdipButton;
	newScenaioButton->Create("", WS_VISIBLE | BS_CENTER | BS_OWNERDRAW, itemPosRect, this, g_scenarioItemID++);

	// 이미지 로드
	UINT noteResourceID = GetNoteUIImageResourceID(recordIndex);
	newScenaioButton->LoadStdImage(noteResourceID, "PNG");
	newScenaioButton->LoadHovImage(noteResourceID, "PNG");
	newScenaioButton->LoadAltImage(noteResourceID, "PNG");

	return newScenaioButton;
}

UINT ScenarioListCtrl::GetNoteUIImageResourceID(int recordIndex)
{
	UINT returnID = IDB_PNG_NOTE_BLACK;
	switch (recordIndex)
	{
		case 0:
		{
			returnID = IDB_PNG_NOTE_BLACK;
			break;
		}
		case 1:
		{
			returnID = IDB_PNG_NOTE_BLUE;
			break;
		}
		case 2:
		{
			returnID = IDB_PNG_NOTE_BROWN;
			break;
		}
		case 3:
		{
			returnID = IDB_PNG_NOTE_GREEN;
			break;
		}
		case 4:
		{
			returnID = IDB_PNG_NOTE_ORANGE;
			break;
		}
		case 5:
		{
			returnID = IDB_PNG_NOTE_PINK;
			break;
		}
		case 6:
		{
			returnID = IDB_PNG_NOTE_PURPLE;
			break;
		}
		case 7:
		{
			returnID = IDB_PNG_NOTE_RED;
			break;
		}
		case 8:
		{
			returnID = IDB_PNG_NOTE_SKY;
			break;
		}
		case 9:
		{
			returnID = IDB_PNG_NOTE_YELLOW;
			break;
		}

		default:
		{
			returnID = IDB_PNG_NOTE_BLACK;
			break;
		}
	}

	return returnID;
}

CRect ScenarioListCtrl::GetItemPosRect(int tupleIndex, int recordIndex)
{
	// 인덱스에 따라서 아이템UI의 Rect를 리턴
	CRect returnRect;

	returnRect.left = ITEM_START_POS_X + (recordIndex * (ITEM_WITDH + ITEM_BY_ITEM_MARGIN_X)) + (recordIndex > 4 ? FIVE_DIVIDE_MARGIN : 0);
	returnRect.top = ITEM_START_POS_Y + (tupleIndex * (ITEM_HEIGHT + ITEM_BY_ITEM_MARGIN_Y));
	returnRect.right = returnRect.left + ITEM_WITDH;
	returnRect.bottom = returnRect.top + ITEM_HEIGHT;

	return returnRect;
}

void ScenarioListCtrl::LoadScenarioTimeline(HWND handle)
{
	CGdipButton* dbClickButton = (CGdipButton*)FromHandle(handle);

	ComplexMap<int, ComplexMap<int, ScenarioData>>::iterator tupleIter = m_TupleDataMap.begin();
	ComplexMap<int, ScenarioData>::iterator recordIter;
	bool bFind = false;
	while (tupleIter != m_TupleDataMap.end())
	{
		recordIter = tupleIter->value.value.begin();
		
		while (recordIter != tupleIter->value.value.end())
		{
			CGdipButton* deleteButton = recordIter->value.value.uiButton;

			if (deleteButton == dbClickButton)
			{
				bFind = true;
				break;
			}
			recordIter++;
		}

		if (bFind)
			break;

		tupleIter++;
	}

	if (bFind)
	{
		int scenarioIndex = tupleIter->value.key * ITEM_LIMIT_RECORD_COUNT + recordIter->value.key;

		// 시나리오가 이미 존재할 시
		ScenarioManagerStruct scenarioStruct(recordIter->value.value.scenarioVO, scenarioIndex);
		Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);

		if (Scenario_UI_Manager->SendMessages(PM_EXIST) == true)
			return;

		CURSOR_WAIT;
		Scenario_DB_Manager->StartTransaction(TransactionNames[TND_SCENARIO_CREATE]);

		Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);
		if (Scenario_UI_Manager->SendMessages(PM_CREATE) == false)
		{
			Scenario_DB_Manager->RollbackTransaction();
			Log_Manager->OnPutLog("타임라인 화면 정보 로드 오류", LogType::LT_PROCESS);
		}
		else
		{
			Scenario_DB_Manager->CommitTransaction();
			Scenario_UI_Manager->InputScenarioStruct(&scenarioStruct);
			Scenario_UI_Manager->SendMessages(PM_SHOW);
		}

		ComplexString logMsg;
		logMsg.Format("%d", scenarioIndex + 1);
		Log_Manager->OnPutLog(logMsg + "번째 시나리오 화면 정보 출력 성공", LogType::LT_OPERATE);
		Log_Manager->OnPutLog(logMsg + "번째 시나리오 더블클릭", LogType::LT_EVENT);

		CURSOR_ARROW;
	}
}

void ScenarioListCtrl::OnBnClickedMfcbuttonAddItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ComplexString inputText;
	BOKOScenarioAddDlg addDlg(&inputText);
	addDlg.DoModal();

	if (inputText.IsEmpty() == false)
	{
		ComplexString logMsg = inputText + " 시나리오 DB 등록 성공";
		CURSOR_WAIT;
		if (Scenario_DB_Manager->InsertScenarioList(inputText))
		{
			Log_Manager->OnPutLog(inputText + " 시나리오 DB 등록 성공", LogType::LT_PROCESS);

			m_loadScenarioList.clear();
			if (Scenario_DB_Manager->SelectAllScenarioList(&m_loadScenarioList))
				Log_Manager->OnPutLog("시나리오 정보 갱신", LogType::LT_PROCESS);

			ScenarioData scenarioData;
			ScenarioListVO scenario = m_loadScenarioList.at(m_loadScenarioList.size() - 1);
			scenarioData.scenarioVO = scenario;

			InsertItem(m_loadScenarioList.size() - 1, scenarioData);

			Log_Manager->OnPutLog(inputText + " 시나리오 등록 성공", LogType::LT_EVENT);
		}
		CURSOR_ARROW;
	}
}

void ScenarioListCtrl::DeleteScenario(int tupleIndex, int recordIndex)
{
	if (m_loadScenarioList.empty())
		return;

	CURSOR_WAIT;
	Scenario_DB_Manager->StartTransaction(TransactionNames[TND_SCENARIO_DELETE]);

	ScenarioListVO selectedScenario = m_loadScenarioList.at(tupleIndex * ITEM_LIMIT_RECORD_COUNT + recordIndex);
	if (Scenario_DB_Manager->DeleteScenarioList(selectedScenario.GetSceSEQ()) == false)
	{
		Scenario_DB_Manager->RollbackTransaction();
		m_loadScenarioList.clear();
		if (Scenario_DB_Manager->SelectAllScenarioList(&m_loadScenarioList))
			Log_Manager->OnPutLog("시나리오 정보 갱신", LogType::LT_PROCESS);

		CURSOR_ARROW;
		return;
	}

	Log_Manager->OnPutLog("시나리오 DB 삭제 성공", LogType::LT_PROCESS);
	Log_Manager->OnPutLog("시나리오 삭제 성공", LogType::LT_EVENT);

	m_loadScenarioList.clear();
	if (Scenario_DB_Manager->SelectAllScenarioList(&m_loadScenarioList))
		Log_Manager->OnPutLog("시나리오 정보 갱신", LogType::LT_PROCESS);

	if (m_loadScenarioList.empty())
	{
		Scenario_DB_Manager->UpdateScenarioListAutoIncrementSeq();
		Log_Manager->OnPutLog("시나리오 미존재로 인한 시퀀스번호 갱신", LogType::LT_PROCESS);
	}
	Scenario_DB_Manager->CommitTransaction();

	CURSOR_ARROW;
}

void ScenarioListCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Sizing(nType);
}
