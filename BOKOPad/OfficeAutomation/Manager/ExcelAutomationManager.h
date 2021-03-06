#pragma once
  
#include "..\CExcel\CApplication.h"
#include "..\CExcel\CRange.h"
#include "..\CExcel\CWorkbook.h"
#include "..\CExcel\CWorkbooks.h"
#include "..\CExcel\CWorksheet.h"
#include "..\CExcel\CWorksheets.h"
#include "..\CExcel\CFont0.h"
#include "..\CExcel\CBorder.h"
#include "..\CExcel\CBorders.h"
#include "..\CExcel\CCellFormat.h"
#include "..\CExcel\Cnterior.h"
#include "..\CExcel\CChart.h"
#include "..\CExcel\CCharts.h"
#include "..\CExcel\CChartTitle.h"
#include "..\CExcel\CChartColorFormat.h"	
#include "..\CExcel\CChartFillFormat.h"	
#include "..\CExcel\CLegend.h"
#include "..\CExcel\CLegendEntry.h"
#include "..\CExcel\CLegendKey.h"
#include "..\CExcel\CHPageBreaks.h"
#include "..\CExcel\CHPageBreak.h"
#include "..\CExcel\CVPageBreaks.h"
#include "..\CExcel\CVPageBreak.h"
#include "..\CExcel\CWindows.h"
#include "..\CExcel\CWindow0.h"
#include "..\CExcel\CSeries.h"
#include "..\CExcel\CPoint0.h"
#include <vector> 
#include <map>
#include "OfficeAutomationInterface.h"

#define AUTO_COLUMN				0
#define AUTO_ROW				1
#define COMMON_COLUMN_VALUE		18
#define COMMON_ROW_VALUE		16.5
#define HEADER_ROW_VALUE		28
#define TITLE_ROW_VALUE			24


//https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.excel._chart?view=excel-pia	ms excel api (.net ver)
//https://docs.microsoft.com/en-us/office/vba/api/excel.workbooks.add	ms excel api (vba ver)
//https://gusrealworld.tistory.com/47 vba colorindex 표

// ms office의 엑셀 클래스를 사용하기 편하게 처리하는 클래스
class ExcelAutomationManager : public OAInterface
{
public:

	// 생성자
	ExcelAutomationManager();

	// 소멸자
	virtual ~ExcelAutomationManager();

public:


	/*
	 * 기본 보더설정값 (예약용)
	 */
	enum BorderStyle
	{
		BORDER_STYLE_LEFT =		 7L,
		BORDER_STYLE_TOP =		 8L,
		BORDER_STYLE_BOTTOM =	 9L,
		BORDER_STYLE_RIGHT =	 10L,
		BORDER_STYLE_INNER =	 11L
	};

	/*
	 * 보더의 볼드스타일 설정
	 */
	enum BoldStyle
	{
		BOLD_STYLE_CONTINOUS =		 1,
		BOLD_STYLE_DASH =			 -4115,
		BOLD_STYLE_DASHDOT =		 4,
		BOLD_STYLE_DASHDOTDOT =		 5,
		BOLD_STYLE_DOT =			 -4118,
		BOLD_STYLE_DOUBLE =			 -4119,
		BOLD_STYLE_LINESTYLENONE =	 -4142,
		BOLD_STYLE_SLANTDASHDOT =	 13
	};

	/*
	 * 보더의 볼드 두께 설정
	 */
	enum BoldWeight
	{
		BOLD_WEIGHT_THIN =		 1,
		BOLD_WEIGHT_LESS_THICK = 2,
		BOLD_WEIGHT_THICK =		 3

	};

	/*
	 * 텍스트 정렬 설정
	 */
	enum Align
	{
		TEXT_ALIGN_TOP =	 -4160,
		TEXT_ALIGN_BOTTOM =	 -4107,
		TEXT_ALIGN_LEFT =	 -4131,
		TEXT_ALIGN_CENTER =	 -4108,
		TEXT_ALIGN_RIGHT =	 -4152
	};

	/*
	 * 차트 종류 설정
	 */
	enum Chart
	{
		CHART_PIE =			 5,
		CHART_DONUT =		 -4120,
		CHART_LINE =		 4,
		CHART_POINTLINE =	 65,
		CHART_STICK =		 51
	};

	/**
	* 엑셀의 그래프를 그리기위한 구조체
	*/
	struct ChartData
	{
		// 1. 캡션 2. 캡션의 값
		std::map<CString, CString> chartPairData;	

		// vba colorindex 표 참조
		std::vector<int> legendColors;	

		// 차트 종류 열거형 Chart 참조
		Chart kindOfChart;		

		// 차트 제목
		CString strChartTitle;						
	};

private:
 

	// 엑셀의 기능들의 객체 집합 구조체
	struct ExcelObject
	{
		// 엑셀데이터의 최상위 객체
		CApplication		app;	

		// 워크북들을 담고있는 콜렉션 객체
		CWorkbooks			wbs;			

		// 단일 워크북 객체
		CWorkbook			wb;		

		// 워크시트를 담고있는 콜렉션 객체
		CWorksheets			wss;		

		// 단일 워크시트 객체
		CWorksheet			ws;		

		// 워크시트 안에 존재하는 셀 범위를 담당하는 객체
		CRange				range;				

		// 해당 범위안에 있는 데이터의 폰트를 지정하는 객체
		CFont0				font;	

		// 해당 범위안에 있는 셀의 보더값들을 담고있는 객체
		CBorders			borders;	

		// 보더값의 단일 객체
		CBorder				bl, bt, bb, br;	

		// 해당 범위를 디자인할수있는 객체
		Cnterior			interior;	

		// 워크북에 존재하는 차트 콜렉션 객체
		CCharts				charts;	

		// 차트콜렉션에서 추가한 단일 차트객체
		CChart				chart;				

		// 차트객체의 타이틀을 담고있는 객체
		CChartTitle			title;		

		// 차트객체의 데이터의 색을 입힐수있는 객체
		CChartColorFormat	cform;	

		// 차트객체의 데이터의 채우기기능을 사용할수 있는 객체
		CChartFillFormat	fform;

		// 차트객체의 계열값을 담고있는 객체
		CSeries				srs;				

		// 계열값의 데이터와 그래프를 연결해주는 객체
		CPoint0				point;	

		// 차트객체의 범례값을 가지고있는 객체
		CLegend				legend;	

		// 단일 범례값을 담고있는 객체
		CLegendEntry		entry;	

		// 범례값의 데이터와 그래프를 연결해주는 객체
		CLegendKey			key;			

		// 엑셀의 가로 페이지나누기값을 담고있는 콜렉션 객체
		CHPageBreaks		hpbs;		

		// 단일 가로 페이지나누기 객체
		CHPageBreak			hpb;

		// 엑셀의 세로 페이지나누기값을 담고있는 콜렉션 객체
		CVPageBreaks		vpbs;				

		// 단일 세로 페이지나누기 객체
		CVPageBreak			vpb;

		// 엑셀의 전체 창을 나타내는 콜렉션 객체
		CWindows			wnds;

		// 단일 창객체
		CWindow0			wnd;					
	};

	// 엑셀의 기능들의 객체 집합 구조체 포인터
	ExcelObject* m_eo;

	// 현재 워크시트의 가로축 위치값
	int m_nDepth;			

	// 복사한 워크시트의 가로축 위치값
	int m_nCpyDepth;

	// 사방면 보더값 사용 유무
	bool m_bSetBl, m_bSetBr, m_bSetBb, m_bSetBt;	

	// 셀의 범위가 설정되었는지 유무
	bool m_bSetRange;

	// 셀의 높이값이 저장되는 변수
	int m_nSaveHeight;

	// 셀의 tab이 저장되는 변수
	int m_nSaveTab;

public:

	/**
	* SetRangeStyle
	* @ param1	:	범위에 칠할 색상값
	* @ param2	:	셀의 전체 범위의 정렬을 설정하는 값
	* @ param3	:	셀의 전체 범위의 보더값 사용 유무
	* @ param4	:	셀의 전체 범위의 텍스트 두께 사용 유무
	* @ param5	:	셀의 전체 범위의 테두리의 스타일 설정
	* @ param6	:	셀의 전체 범위의 테두리 두께 설정
	* @ param7	:	셀의 전체 범위의 모든 셀의 테두리 두께 설정
	* @ see		:	엑셀 범위안에 있는 셀의 스타일을 설정하는 함수
	*/
	void SetRangeStyle(OAColor CellColor, Align TextAlign, bool BoxBorder, bool FontBold, BoldStyle BoxBoldStyle = BOLD_STYLE_CONTINOUS, BoldWeight BoxBorderBold = BOLD_WEIGHT_THICK, BoldWeight CellBorderBold = BOLD_WEIGHT_THIN);

	/**
	* SetBoxStyle
	* @ param1	:	셀의 전체 범위의 보더값 사용 유무
	* @ param2	:	셀의 전체 범위의 테두리의 스타일 설정
	* @ param3	:	셀의 전체 범위의 테두리 두께 설정
	* @ param4	:	셀의 전체 범위의 모든 셀의 테두리 두께 설정
	* @ see		:	SetRangeStyle 함수의 범위의 셀들의 테두리를 설정하는 함수
	*/
	void SetBoxStyle(bool BoxBorder, BoldStyle BoxBoldStyle = BOLD_STYLE_CONTINOUS, BoldWeight BoxBorderBold = BOLD_WEIGHT_THICK, BoldWeight CellBorderBold = BOLD_WEIGHT_THIN);

	/**
	* SetRange
	* @ param1	:	범위를 시작할 column값
	* @ param2	:	범위를 시작할 low값
	* @ param3	:	범위를 설정할 가로길이
	* @ param4	:	범위를 설정할 세로길이
	* @ param5	:	범위 설정중에 또다른 범위가 설정되었을 경우 그값을 구분하기 위한 값
	* @ see		:	엑셀의 셀의 범위를 설정하는 함수
	* @ return	:	범위 설정 성공 여부
	*/
	bool SetRange(unsigned int nTab, unsigned int nDepth, unsigned int nWidth, unsigned int nHeight, bool bParent = true);

	/**
	* StartExcel
	* @ param	:	시작할 워크시트의 이름
	* @ see		:	엑셀을 시작하기 위한 함수
	* @ return	:	시작 성공 여부
	*/
	bool StartExcel(CString WorkSheetName);

	/**
	* SaveWorkBook
	* @ param	:	엑셀을 저장할 경로명
	* @ see		:	엑셀을 저장하기 위한 함수
	*/
	void SaveWorkBook(CString strFilePath);

	/**
	* CloseExcel
	* @ param	:	엑셀을 저장할 경로명
	* @ see		:	엑셀을 저장하기 하고 종료하기위한 함수
	*/
	void CloseExcel(CString strFilePath, bool executeShell = true);

	/**
	* SetItem
	* @ param1	:	엑셀의 범위로부터의 세로 인덱스
	* @ param2	:	엑셀의 범위로부터의 가로 인덱스
	* @ param3	:	엑셀의 범위에 입력할 텍스트
	* @ see		:	범위설정이 완료된 후 해당범위의 (0, 0) 지점부터 시작하여 인자로 넘어온 인덱스에 텍스트를 입력해주는 함수
	*/
	void SetItem(int nHeight, int nWidth, CString strText);

	/**
	* SetItem
	* @ param	:	엑셀의 범위에 입력할 텍스트
	* @ see		:	범위설정이 완료된 후 해당범위에 텍스트를 입력해주는 함수 (해당 범위가 Merge가 된 상태이거나 1,1 범위일 경우에 사용)
	*/
	void SetItem(CString strText);

	/**
	* SetMerge
	* @ see		:	범위설정이 완료된 후 해당범위를 하나의 셀로 합치는 함수
	*/
	void SetMerge();

	/**
	* SetMerge
	* @ param1	:	하나로 합칠 시작지점 좌표
	* @ param2	:	하나로 합칠 끝 지점 좌표
	* @ see		:	범위설정이 완료된 후 해당범위를 하나의 셀로 합치는 함수
					시작포인트부터 끝포인트까지의 지점을 하나로 합친다.
	*/
	void SetMerge(CPoint startPoint, CPoint endPoint);

	/**
	* SetLineEnd
	* @ param	:	워크시트의 칸을 건너뛸 행 갯수
	* @ see		:	현재 워크시트의 행을 인자 값 만큼 건너 뛴다.
	*/
	void SetLineEnd(unsigned int nLineEnd = 1);

	/**
	* GetDepth
	* @ see		:	현재 워크시트의 행의 인덱스를 가져오는 함수
	* @ return	:	현재 가르키고있는 마지막 행의 인덱스 번호
	*/
	int GetDepth() const;

	/**
	* SetDepth
	* @ param	:	저장할 행
	* @ see		:	현재 워크시트의 행의 인덱스를 설정하는 함수
	*/
	void SetDepth(int nDepth);

	/**
	* SetCellWidth
	* @ param	:	설정할 넓이
	* @ see		:	설정한 범위의 셀의 가로길이를 설정하는 함수
	*/
	void SetCellWidth(double dWidth);

	/**
	* SetCellHeight
	* @ param	:	설정할 높이
	* @ see		:	설정한 범위의 셀의 세로길이를 설정하는 함수
	*/
	void SetCellHeight(double dHeight);

	/**
	* SetVisibleTime
	* @ param	:	시간
	* @ see		:	단순히 시간출력에서 길어지면 ####이 출력되는데 앞에 '을 붙여서 막는 함수
	* @ return	:	앞에 ' 을 붙여서 리턴함
	*/
	CString SetVisibleTime(CString strTime);

	/**
	* SetBlOn
	* @ param	:	왼쪽 보더 사용 유무
	* @ see		:	설정한 범위의 왼쪽 테두리를 사용할지 여부를 정하는 함수
	*/
	void SetBlOn(bool setBl);

	/**
	* SetBtOn
	* @ param	:	위쪽 보더 사용 유무
	* @ see		:	설정한 범위의 위쪽 테두리를 사용할지 여부를 정하는 함수
	*/
	void SetBtOn(bool setBt);

	/**
	* SetBbOn
	* @ param	:	아래쪽 보더 사용 유무
	* @ see		:	설정한 범위의 아래쪽 테두리를 사용할지 여부를 정하는 함수
	*/
	void SetBbOn(bool setBb);

	/**
	* SetBrOn
	* @ param	:	오른쪽 보더 사용 유무
	* @ see		:	설정한 범위의 오른쪽 테두리를 사용할지 여부를 정하는 함수
	*/
	void SetBrOn(bool setBr);

	/**
	* SetAutoFit
	* @ param	:	자동맞춤을 설정할 열, 너비값 구분자
	* @ see		:	인자로 들어온 열, 너비값을 자동으로 정의된 넓이로 설정하는 함수
	*/
	void SetAutoFit(bool bEntireType);

	/**
	* SetChart
	* @ param	:	사용할 통계 데이터
	* @ see		:	인자로 넘어온 통계 데이터로 엑셀에 그래프를 그리는 함수
					새로운 워크시트 생성 후 작업, 필요한 데이터 헤더, 값 입력 후 작업
	*/
	void SetChart(ChartData chartData);

	/**
	* AddWorkSheet
	* @ param	:	추가할 워크시트의 이름
	* @ see		:	워크시트를 추가하는 함수
	*/
	void AddWorkSheet(CString WorkSheetName);

	/**
	* SetIndent
	* @ param	:	들여쓰기 개수 (0 ~ 15 사이 값만)
	* @ see		:	설정한 범위의 텍스트의 들여쓰기를 설정하는 함수
	*/
	void SetIndent(int nIndentValue);
	
	/**
	* SetAutoLineChange
	* @ param	:	자동 줄바꿈 유무
	* @ see		:	설정한 범위의 텍스트의 자동 줄바꿈을 설정한다.
	*/
	void SetAutoLineChange(BOOL setLineEnd);

	/**
	* SetVerticalTextAlign
	* @ param	:	정렬할 값
	* @ see		:	설정한 범위의 텍스트의 세로축 정렬을 설정한다
	*/
	void SetVerticalTextAlign(Align TextAlign);

	/**
	* SetHorizonTextAlign
	* @ param	:	정렬할 값
	* @ see		:	설정한 범위의 텍스트의 가로축 정렬을 설정한다
	*/
	void SetHorizonTextAlign(Align TextAlign);

	/**
	* SetPageBreak
	* @ param1	:	가로축 페이지나누기 사용유무
	* @ param2	:	세로축 페이지나누기 사용유무
	* @ see		:	페이지 나누기를 가로축 한페이지로 설정해주는 함수
	*/
	void SetPageBreak(bool bUseVerticalBreak, bool bUseHorizonBreak);

	/**
	* GetRowFixelValue
	* @ see		:	현재 설정된 범위의 열 높이값을 리턴하는 함수
					선택된 열 범위가 다중값 일 경우 전체 열의 높이값을 리턴 
	* @ return	:	설정된범위의 높이값
	*/
	double GetRowFixelValue();

	/**
	* GetColumnFixelValue
	* @ see		:	현재 설정된 범위의 행 넓이값을 리턴하는 함수
					선택된 행 범위가 다중값 일 경우 전체 행의 넓이값을 리턴
	* @ return	:	설정된범위의 넓이값
	*/
	double GetColumnFixelValue();

	/**
	* ResetDepth
	* @ see		:	현재 행의 인덱스를 0으로 초기화하는 함수 (_cpyDepth에 복사됨)
	*/
	void ResetDepth();

	/**
	* RevertDepth
	* @ see		:	_cpyDepth에 복사했던 뎁스를 현재 뎁스에 저장
	*/
	void RevertDepth();

	/**
	* GetCpyDepth
	* @ see		:	_cpyDepth의 값을 가져옴
	* @ return	:	_cpyDepth 값
	*/
	int GetCpyDepth() const;

	/**
	* SetFontBold
	* @ param	:	글자의 두께를 설정할 지 여부
	* @ see		:	현재 설정된 범위의 글자의 두께를 설정
	*/
	void SetFontBold(bool FontBold);

	/**
	* SetFontSize
	* @ param	:	크기를 설정할 값
	* @ see		:	현재 설정된 범위의 글자의 크기를 설정
	*/
	void SetFontSize(int nFontSize);

	/**
	* SetFontStyle
	* @ param	:	폰트의 스타일 스트링값
	* @ see		:	현재 설정된 범위의 글자의 스타일을 설정 (예: 궁서체)
	*/
	void SetFontStyle(CString strFontStyle);

	/**
	* SetFontColor
	* @ param	:	설정할 색상값
	* @ see		:	현재 설정된 범위의 글자의 색상을 설정
	*/
	void SetFontColor(OAColor FontColor);

	/**
	* SetFontColor
	* @ param1	:	현재 범위를 기준으로의 시작지점 좌표
	* @ param2	:	현재 범위를 기준으로의 끝지점 좌표
	* @ param3	:	설정할 색상값
	* @ see		:	지정한 범위의 글자의 색상을 설정
	*/
	void SetFontColor(CPoint startPoint, CPoint endPoint, OAColor FontColor);

	/**
	* SetAutoRowSize
	* @ see		:	셀의 row값을 일정 비율로 설정
	*/
	void SetAutoRowSize();

	/**
	* SaveDepth
	* @ see		:	현재 엑셀의 행을 저장. 항상 SetRange 함수 사용 후 제일 마지막에 사용한다.
	*/
	void SaveDepth();

	/**
	* SetBkColor
	* @ param	:	설정할 색상
	* @ see		:	설정한 범위의 셀의 배경색을 설정하는 함수
	*/
	void SetBkColor(OAColor CellColor);

	/**
	* SetBkColor
	* @ param1	:	현재 범위를 기준으로의 시작지점 좌표
	* @ param2	:	현재 범위를 기준으로의 끝지점 좌표
	* @ param3	:	설정할 색상
	* @ see		:	지정한 범위의 셀의 배경색을 설정하는 함수
	*/
	void SetBkColor(CPoint startPoint, CPoint endPoint, OAColor CellColor);

	/**
	* SetBoxBorder
	* @ param	:	테두리의 두께
	* @ see		:	설정한 범위의 셀의 모든 테두리값을 설정하고 두께를 지정하는 함수
	*/
	void SetBoxBorder(BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderLeft
	* @ param1	:	테두리의 스타일
	* @ param2	:	테두리의 두께
	* @ see		:	설정한 범위의 셀의 왼쪽 테두리의 모양과 두께를 설정하는 함수
	*/
	void SetBoxBorderLeft(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderRight
	* @ param1	:	테두리의 스타일
	* @ param2	:	테두리의 두께
	* @ see		:	설정한 범위의 셀의 오른쪽 테두리의 모양과 두께를 설정하는 함수
	*/
	void SetBoxBorderRight(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderTop
	* @ param1	:	테두리의 스타일
	* @ param2	:	테두리의 두께
	* @ see		:	설정한 범위의 셀의 위쪽 테두리의 모양과 두께를 설정하는 함수
	*/
	void SetBoxBorderTop(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderBottom
	* @ param1	:	테두리의 스타일
	* @ param2	:	테두리의 두께
	* @ see		:	설정한 범위의 셀의 아래쪽 테두리의 모양과 두께를 설정하는 함수
	*/
	void SetBoxBorderBottom(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* VisibleWorkSheet
	* @ param1	:	숨길 워크시트 인덱스번호 0부터시작
	* @ param2	:	시트를 숨길지 여부 (true = 표시, false = 숨김)
	* @ see		:	시트를 숨기는 함수
	*/
	void VisibleWorkSheet(int nWorkSheetIndex, bool bVisible);

	/**
	* SetActiveWorkSheet
	* @ param	:	활성시트로 설정할 워크시트 인덱스번호 0부터시작
	* @ see		:	시트를 활성시트로 설정하는 함수
	*/
	void SetActiveWorkSheet(int nWorkSheetIndex);

private:

	/**
	* SetRangeData
	* @ param1	:	범위를 설정할 첫 가로축 시작지점
	* @ param2	:	저장할 가로축 인덱스 알파벳 
	* @ see		:	셀의 범위를 설정할 때 가로축의 인덱스 (알파벳)를 설정하는 함수
	* @ return	:	범위 설정 성공 여부
	*/
	bool SetRangeData(int nLoopNum, CString& fixStr);

	/**
	* SetLoopCnt
	* @ param1	:	범위를 설정할 가로축 지점
	* @ see		:	SetRangeData 함수에서 알파벳을 구하기위해 내부적으로 처리되어 char 값으로 vector에 담아 리턴하는 함수
	* @ return	:	가로축 인덱스 알파벳이 char형태로 vector에 포함되어 리턴
	*/
	std::vector<char> SetLoopCnt(int nLoopNum);

	/**
	* SetLoopCnt
	* @ param1	:	저장할 텍스트 변수
	* @ param2	:	SetLoopCnt 함수에서 수행된 알파벳이 담긴 vector
	* @ see		:	SetLoopCnt 함수를 실행한 후 vector값으로 문자열을 만들어주는 함수 (알파벳인지 조건처리)
	* @ return	:	vector로 넘어온 char 값이 알파벳인경우 true, 알파벳이 아닌경우 false
	*/
	bool SetFix(CString& prefix, std::vector<char> loopVector);

	/**
	* SetPieChart
	* @ see		:	원형차트 기본 데이터 설정
	*/
	void SetPieChart();

	/**
	* SetLineChart
	* @ see		:	선형차트 기본 데이터 설정
	*/
	void SetLineChart();

	/**
	* SetStickChart
	* @ see		:	막대차트 기본 데이터 설정
	*/
	void SetStickChart();

	/**
	* SetChartTitle
	* @ param	:	설정할 그래프의 타이틀
	* @ see		:	설정한 그래프의 타이틀을 설정하는 함수
	*/
	void SetChartTitle(CString chartTitle);

	/**
	* SetPieChartColor
	* @ param1	:	ChartData에서 저장된 범례의 사이즈
	* @ param2	:	설정할 범례의 색상
	* @ see		:	원형차트 종류 색 설정 함수
	*/
	void SetPieChartColor(int legendSize, std::vector<int> legendColors);

	/**
	* SetLineChartColor
	* @ param1	:	선형차트가 라인인지 포인트라인인지 구분
	* @ param2	:	ChartData에서 저장된 범례의 사이즈
	* @ param3	:	설정할 범례의 색상
	* @ see		:	선형차트 종류 색 설정 함수
	*/
	void SetLineChartColor(int whichChart, int legendSize, std::vector<int> legendColors);

	/**
	* SetStickChartColor
	* @ param1	:	ChartData에서 저장된 범례의 사이즈
	* @ param2	:	설정할 범례의 색상
	* @ see		:	막대차트 종류 색 설정 함수
	*/
	void SetStickChartColor(int legendSize, std::vector<int> legendColors);

	/**
	* BorderReset
	* @ param	:	보더값 사용설정 값
	* @ see		:	사방면의 보더 유무값 사용 설정 함수
	*/
	void BorderReset(bool bReset);

	/**
	* DataSwapping
	* @ param1	:	변경할 첫번째 값
	* @ param2	:	변경할 두번째 값
	* @ see		:	Range 범위 안에서 좌표계산하기 전에 start와 end값을 스왑해주는 함수
	*/
	void DataSwapping(LONG* nTargetData1, LONG* nTargetData2);
	
	/**
	* InsertChartData
	* @ param1	:	등록할 차트의 종류
	* @ param2	:	차트에 들어갈 범례와 값이 들어갈 데이터
	* @ see		:	차트의 데이터가 포함된 워크시트를 자동으로 생성하는 함수
	*/
	void InsertChartData(int whichChart, std::map<CString, CString> chartPairData);
};

