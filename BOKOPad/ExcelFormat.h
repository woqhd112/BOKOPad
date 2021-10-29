#pragma once
  
#include "CExcel\CApplication.h"
#include "CExcel\CRange.h"
#include "CExcel\CWorkbook.h"
#include "CExcel\CWorkbooks.h"
#include "CExcel\CWorksheet.h"
#include "CExcel\CWorksheets.h"
#include "CExcel\CFont0.h"
#include "CExcel\CBorder.h"
#include "CExcel\CBorders.h"
#include "CExcel\CCellFormat.h"
#include "CExcel\Cnterior.h"
#include "CExcel\CChart.h"
#include "CExcel\CCharts.h"
#include "CExcel\CChartTitle.h"
#include "CExcel\CChartColorFormat.h"	
#include "CExcel\CChartFillFormat.h"	
#include "CExcel\CLegend.h"
#include "CExcel\CLegendEntry.h"
#include "CExcel\CLegendKey.h"
#include "CExcel\CHPageBreaks.h"
#include "CExcel\CHPageBreak.h"
#include "CExcel\CVPageBreaks.h"
#include "CExcel\CVPageBreak.h"
#include "CExcel\CWindows.h"
#include "CExcel\CWindow0.h"
#include "CExcel\CSeries.h"
#include "CExcel\CPoint0.h"
#include <vector> 
#include <map>
#include <afx.h> 
#include <afxwin.h>  
#include <afxext.h>  
#include <afxdlgs.h>
#include <afxdisp.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#define AUTO_COLUMN				0
#define AUTO_ROW				1
#define COMMON_COLUMN_VALUE		18
#define COMMON_ROW_VALUE		16.5
#define HEADER_ROW_VALUE		28
#define TITLE_ROW_VALUE			24


//https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.excel._chart?view=excel-pia	ms excel api (.net ver)
//https://docs.microsoft.com/en-us/office/vba/api/excel.workbooks.add	ms excel api (vba ver)
//https://gusrealworld.tistory.com/47 vba colorindex ǥ

// ms office�� ���� Ŭ������ ����ϱ� ���ϰ� ó���ϴ� Ŭ����
class CExcelFormat
{
public:

	// ������
	CExcelFormat();

	// �Ҹ���
	~CExcelFormat();

public:

	/*
	 * Excel RGB Color
	 */
	enum Color
	{
		// cell color
		COLOR_THICK_GREY =				 RGB(128, 128, 128),
		COLOR_LESS_THICK_GREY =			 RGB(166, 166, 166),
		COLOR_GREY =					 RGB(191, 191, 191),
		COLOR_LESS_LIGHT_GREY =			 RGB(217, 217, 217),
		COLOR_LIGHT_GREY =				 RGB(242, 242, 242),
		COLOR_WHITE =					 RGB(255, 255, 255),
		COLOR_BLACK =					 RGB(0, 0, 0),
		COLOR_RED =						 RGB(255, 0, 0),
		//project&&env depth color
		COLOR_PASTEL_ORANGE =			 RGB(237, 125, 49),
		COLOR_PASTEL_LESS_ORANGE =		 RGB(244, 176, 132),
		COLOR_PASTEL_LIGHT_ORANGE =		 RGB(252, 228, 214),
		//group depth color
		COLOR_PASTEL_GREEN =			 RGB(146, 208, 80),
		COLOR_PASTEL_LESS_GREEN =		 RGB(169, 208, 142),
		COLOR_PASTEL_LIGHT_GREEN =		 RGB(226, 239, 218),
		//scenario depth color
		COLOR_PASTEL_BLUE =				 RGB(47, 117, 181),
		COLOR_PASTEL_LESS_BLUE =		 RGB(189, 215, 238),
		COLOR_PASTEL_LIGHT_BLUE =		 RGB(221, 235, 247),
		//message depth color
		COLOR_PASTEL_THICK_YELLOW =		 RGB(255, 192, 0),
		COLOR_PASTEL_YELLOW =			 RGB(255, 217, 102),
		COLOR_PASTEL_LESS_YELLOW =		 RGB(255, 230, 153),
		COLOR_PASTEL_LIGHT_YELLOW =		 RGB(255, 242, 204),
		//reason depth color
		COLOR_PASTEL_TURQUOISE =		 RGB(32, 178, 198),
		COLOR_PASTEL_LESS_TURQUOISE =	 RGB(147, 219, 229),
		COLOR_PASTEL_LIGHT_TURQUOISE =	 RGB(221, 244, 247)
	};

	/*
	 * �⺻ ���������� (�����)
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
	 * ������ ���彺Ÿ�� ����
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
	 * ������ ���� �β� ����
	 */
	enum BoldWeight
	{
		BOLD_WEIGHT_THIN =		 1,
		BOLD_WEIGHT_LESS_THICK = 2,
		BOLD_WEIGHT_THICK =		 3

	};

	/*
	 * �ؽ�Ʈ ���� ����
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
	 * ��Ʈ ���� ����
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
	* ������ �׷����� �׸������� ����ü
	*/
	struct ChartData
	{
		// 1. ĸ�� 2. ĸ���� ��
		std::map<CString, CString> chartPairData;	

		// vba colorindex ǥ ����
		std::vector<int> legendColors;	

		// ��Ʈ ���� ������ Chart ����
		Chart kindOfChart;		

		// ��Ʈ ����
		CString strChartTitle;						
	};

private:
 
	// �������� ����ϱ����� true ��
	COleVariant _colTrue;

	// �������� ����ϱ����� false ��
	COleVariant _colFalse;

	// �������� ����ϱ����� �ɼǰ�
	COleVariant _colOption; 

	// ������ ��ɵ��� ��ü ���� ����ü
	struct ExcelObject
	{
		// ������������ �ֻ��� ��ü
		CApplication _app;	

		// ��ũ�ϵ��� ����ִ� �ݷ��� ��ü
		CWorkbooks _wbs;			

		// ���� ��ũ�� ��ü
		CWorkbook _wb;		

		// ��ũ��Ʈ�� ����ִ� �ݷ��� ��ü
		CWorksheets _wss;		

		// ���� ��ũ��Ʈ ��ü
		CWorksheet _ws;		

		// ��ũ��Ʈ �ȿ� �����ϴ� �� ������ ����ϴ� ��ü
		CRange _range;				

		// �ش� �����ȿ� �ִ� �������� ��Ʈ�� �����ϴ� ��ü
		CFont0 _font;	

		// �ش� �����ȿ� �ִ� ���� ���������� ����ִ� ��ü
		CBorders _borders;	

		// �������� ���� ��ü
		CBorder _bl, _bt, _bb, _br;	

		// �ش� ������ �������Ҽ��ִ� ��ü
		Cnterior _interior;	

		// ��ũ�Ͽ� �����ϴ� ��Ʈ �ݷ��� ��ü
		CCharts _charts;	

		// ��Ʈ�ݷ��ǿ��� �߰��� ���� ��Ʈ��ü
		CChart _chart;				

		// ��Ʈ��ü�� Ÿ��Ʋ�� ����ִ� ��ü
		CChartTitle _title;		

		// ��Ʈ��ü�� �������� ���� �������ִ� ��ü
		CChartColorFormat _cform;	

		// ��Ʈ��ü�� �������� ä������� ����Ҽ� �ִ� ��ü
		CChartFillFormat _fform;

		// ��Ʈ��ü�� �迭���� ����ִ� ��ü
		CSeries _srs;				

		// �迭���� �����Ϳ� �׷����� �������ִ� ��ü
		CPoint0 _point;	

		// ��Ʈ��ü�� ���ʰ��� �������ִ� ��ü
		CLegend _legend;	

		// ���� ���ʰ��� ����ִ� ��ü
		CLegendEntry _entry;	

		// ���ʰ��� �����Ϳ� �׷����� �������ִ� ��ü
		CLegendKey _key;			

		// ������ ���� �����������Ⱚ�� ����ִ� �ݷ��� ��ü
		CHPageBreaks _hpbs;		

		// ���� ���� ������������ ��ü
		CHPageBreak _hpb;

		// ������ ���� �����������Ⱚ�� ����ִ� �ݷ��� ��ü
		CVPageBreaks _vpbs;				

		// ���� ���� ������������ ��ü
		CVPageBreak _vpb;

		// ������ ��ü â�� ��Ÿ���� �ݷ��� ��ü
		CWindows _wnds;

		// ���� â��ü
		CWindow0 _wnd;					
	};

	// ������ ��ɵ��� ��ü ���� ����ü ������
	ExcelObject* eo;

	// ���� ��ũ��Ʈ�� ������ ��ġ��
	int _nDepth;			

	// ������ ��ũ��Ʈ�� ������ ��ġ��
	int _nCpyDepth;

	// ���� ������ ��� ����
	bool _bSetBl, _bSetBr, _bSetBb, _bSetBt;	

	// ���� ��ŸƮ����
	bool _bAttach;	

	// ���� ������ �����Ǿ����� ����
	bool _bSetRange;

	// ���� ���̰��� ����Ǵ� ����
	int _nSaveHeight;

	// ���� tab�� ����Ǵ� ����
	int _nSaveTab;

public:

	/**
	* SetRangeStyle
	* @ param1	:	������ ĥ�� ����
	* @ param2	:	���� ��ü ������ ������ �����ϴ� ��
	* @ param3	:	���� ��ü ������ ������ ��� ����
	* @ param4	:	���� ��ü ������ �ؽ�Ʈ �β� ��� ����
	* @ param5	:	���� ��ü ������ �׵θ��� ��Ÿ�� ����
	* @ param6	:	���� ��ü ������ �׵θ� �β� ����
	* @ param7	:	���� ��ü ������ ��� ���� �׵θ� �β� ����
	* @ see		:	���� �����ȿ� �ִ� ���� ��Ÿ���� �����ϴ� �Լ�
	*/
	void SetRangeStyle(Color CellColor, Align TextAlign, bool BoxBorder, bool FontBold, BoldStyle BoxBoldStyle = BOLD_STYLE_CONTINOUS, BoldWeight BoxBorderBold = BOLD_WEIGHT_THICK, BoldWeight CellBorderBold = BOLD_WEIGHT_THIN);

	/**
	* SetBoxStyle
	* @ param1	:	���� ��ü ������ ������ ��� ����
	* @ param2	:	���� ��ü ������ �׵θ��� ��Ÿ�� ����
	* @ param3	:	���� ��ü ������ �׵θ� �β� ����
	* @ param4	:	���� ��ü ������ ��� ���� �׵θ� �β� ����
	* @ see		:	SetRangeStyle �Լ��� ������ ������ �׵θ��� �����ϴ� �Լ�
	*/
	void SetBoxStyle(bool BoxBorder, BoldStyle BoxBoldStyle = BOLD_STYLE_CONTINOUS, BoldWeight BoxBorderBold = BOLD_WEIGHT_THICK, BoldWeight CellBorderBold = BOLD_WEIGHT_THIN);

	/**
	* SetRange
	* @ param1	:	������ ������ column��
	* @ param2	:	������ ������ low��
	* @ param3	:	������ ������ ���α���
	* @ param4	:	������ ������ ���α���
	* @ param5	:	���� �����߿� �Ǵٸ� ������ �����Ǿ��� ��� �װ��� �����ϱ� ���� ��
	* @ see		:	������ ���� ������ �����ϴ� �Լ�
	* @ return	:	���� ���� ���� ����
	*/
	bool SetRange(unsigned int nTab, unsigned int nDepth, unsigned int nWidth, unsigned int nHeight, bool bParent = true);

	/**
	* StartExcel
	* @ param	:	������ ��ũ��Ʈ�� �̸�
	* @ see		:	������ �����ϱ� ���� �Լ�
	* @ return	:	���� ���� ����
	*/
	bool StartExcel(CString WorkSheetName);

	/**
	* SaveWorkBook
	* @ param	:	������ ������ ��θ�
	* @ see		:	������ �����ϱ� ���� �Լ�
	*/
	void SaveWorkBook(CString strFilePath);

	/**
	* CloseExcel
	* @ param	:	������ ������ ��θ�
	* @ see		:	������ �����ϱ� �ϰ� �����ϱ����� �Լ�
	*/
	void CloseExcel(CString strFilePath);

	/**
	* SetItem
	* @ param1	:	������ �����κ����� ���� �ε���
	* @ param2	:	������ �����κ����� ���� �ε���
	* @ param3	:	������ ������ �Է��� �ؽ�Ʈ
	* @ see		:	���������� �Ϸ�� �� �ش������ (0, 0) �������� �����Ͽ� ���ڷ� �Ѿ�� �ε����� �ؽ�Ʈ�� �Է����ִ� �Լ�
	*/
	void SetItem(int nHeight, int nWidth, CString strText);

	/**
	* SetItem
	* @ param	:	������ ������ �Է��� �ؽ�Ʈ
	* @ see		:	���������� �Ϸ�� �� �ش������ �ؽ�Ʈ�� �Է����ִ� �Լ� (�ش� ������ Merge�� �� �����̰ų� 1,1 ������ ��쿡 ���)
	*/
	void SetItem(CString strText);

	/**
	* SetMerge
	* @ see		:	���������� �Ϸ�� �� �ش������ �ϳ��� ���� ��ġ�� �Լ�
	*/
	void SetMerge();

	/**
	* SetMerge
	* @ param1	:	�ϳ��� ��ĥ �������� ��ǥ
	* @ param2	:	�ϳ��� ��ĥ �� ���� ��ǥ
	* @ see		:	���������� �Ϸ�� �� �ش������ �ϳ��� ���� ��ġ�� �Լ�
					��������Ʈ���� ������Ʈ������ ������ �ϳ��� ��ģ��.
	*/
	void SetMerge(CPoint startPoint, CPoint endPoint);

	/**
	* SetLineEnd
	* @ param	:	��ũ��Ʈ�� ĭ�� �ǳʶ� �� ����
	* @ see		:	���� ��ũ��Ʈ�� ���� ���� �� ��ŭ �ǳ� �ڴ�.
	*/
	void SetLineEnd(unsigned int nLineEnd = 1);

	/**
	* GetDepth
	* @ see		:	���� ��ũ��Ʈ�� ���� �ε����� �������� �Լ�
	* @ return	:	���� ����Ű���ִ� ������ ���� �ε��� ��ȣ
	*/
	int GetDepth() const;

	/**
	* SetDepth
	* @ param	:	������ ��
	* @ see		:	���� ��ũ��Ʈ�� ���� �ε����� �����ϴ� �Լ�
	*/
	void SetDepth(int nDepth);

	/**
	* SetCellWidth
	* @ param	:	������ ����
	* @ see		:	������ ������ ���� ���α��̸� �����ϴ� �Լ�
	*/
	void SetCellWidth(double dWidth);

	/**
	* SetCellHeight
	* @ param	:	������ ����
	* @ see		:	������ ������ ���� ���α��̸� �����ϴ� �Լ�
	*/
	void SetCellHeight(double dHeight);

	/**
	* SetVisibleTime
	* @ param	:	�ð�
	* @ see		:	�ܼ��� �ð���¿��� ������� ####�� ��µǴµ� �տ� '�� �ٿ��� ���� �Լ�
	* @ return	:	�տ� ' �� �ٿ��� ������
	*/
	CString SetVisibleTime(CString strTime);

	/**
	* SetBlOn
	* @ param	:	���� ���� ��� ����
	* @ see		:	������ ������ ���� �׵θ��� ������� ���θ� ���ϴ� �Լ�
	*/
	void SetBlOn(bool setBl);

	/**
	* SetBtOn
	* @ param	:	���� ���� ��� ����
	* @ see		:	������ ������ ���� �׵θ��� ������� ���θ� ���ϴ� �Լ�
	*/
	void SetBtOn(bool setBt);

	/**
	* SetBbOn
	* @ param	:	�Ʒ��� ���� ��� ����
	* @ see		:	������ ������ �Ʒ��� �׵θ��� ������� ���θ� ���ϴ� �Լ�
	*/
	void SetBbOn(bool setBb);

	/**
	* SetBrOn
	* @ param	:	������ ���� ��� ����
	* @ see		:	������ ������ ������ �׵θ��� ������� ���θ� ���ϴ� �Լ�
	*/
	void SetBrOn(bool setBr);

	/**
	* SetAutoFit
	* @ param	:	�ڵ������� ������ ��, �ʺ� ������
	* @ see		:	���ڷ� ���� ��, �ʺ��� �ڵ����� ���ǵ� ���̷� �����ϴ� �Լ�
	*/
	void SetAutoFit(bool bEntireType);

	/**
	* SetChart
	* @ param	:	����� ��� ������
	* @ see		:	���ڷ� �Ѿ�� ��� �����ͷ� ������ �׷����� �׸��� �Լ�
					���ο� ��ũ��Ʈ ���� �� �۾�, �ʿ��� ������ ���, �� �Է� �� �۾�
	*/
	void SetChart(ChartData chartData);

	/**
	* AddWorkSheet
	* @ param	:	�߰��� ��ũ��Ʈ�� �̸�
	* @ see		:	��ũ��Ʈ�� �߰��ϴ� �Լ�
	*/
	void AddWorkSheet(CString WorkSheetName);

	/**
	* SetIndent
	* @ param	:	�鿩���� ���� (0 ~ 15 ���� ����)
	* @ see		:	������ ������ �ؽ�Ʈ�� �鿩���⸦ �����ϴ� �Լ�
	*/
	void SetIndent(int nIndentValue);
	
	/**
	* SetAutoLineChange
	* @ param	:	�ڵ� �ٹٲ� ����
	* @ see		:	������ ������ �ؽ�Ʈ�� �ڵ� �ٹٲ��� �����Ѵ�.
	*/
	void SetAutoLineChange(BOOL setLineEnd);

	/**
	* SetVerticalTextAlign
	* @ param	:	������ ��
	* @ see		:	������ ������ �ؽ�Ʈ�� ������ ������ �����Ѵ�
	*/
	void SetVerticalTextAlign(Align TextAlign);

	/**
	* SetHorizonTextAlign
	* @ param	:	������ ��
	* @ see		:	������ ������ �ؽ�Ʈ�� ������ ������ �����Ѵ�
	*/
	void SetHorizonTextAlign(Align TextAlign);

	/**
	* SetPageBreak
	* @ param1	:	������ ������������ �������
	* @ param2	:	������ ������������ �������
	* @ see		:	������ �����⸦ ������ ���������� �������ִ� �Լ�
	*/
	void SetPageBreak(bool bUseVerticalBreak, bool bUseHorizonBreak);

	/**
	* GetRowFixelValue
	* @ see		:	���� ������ ������ �� ���̰��� �����ϴ� �Լ�
					���õ� �� ������ ���߰� �� ��� ��ü ���� ���̰��� ���� 
	* @ return	:	�����ȹ����� ���̰�
	*/
	double GetRowFixelValue();

	/**
	* GetColumnFixelValue
	* @ see		:	���� ������ ������ �� ���̰��� �����ϴ� �Լ�
					���õ� �� ������ ���߰� �� ��� ��ü ���� ���̰��� ����
	* @ return	:	�����ȹ����� ���̰�
	*/
	double GetColumnFixelValue();

	/*
	 * ���� �ε��غ�
	 * ������ �����̸��� ��ΰ� ����
	 * ���� �غ� ��������
	 */

	/**
	* OnReadyExcel
	* @ param1	:	������ ���� �� ������ ���� �̸�
	* @ param2	:	������ ���� �� ������ ���� ���
	* @ see		:	���� ������ �غ��ϴ� �Լ�
	* @ return	:	���� �غ� ���� ����
	*/
	bool OnReadyExcel(CString* strFileName, CString* strFilePath);

	/**
	* ResetDepth
	* @ see		:	���� ���� �ε����� 0���� �ʱ�ȭ�ϴ� �Լ� (_cpyDepth�� �����)
	*/
	void ResetDepth();

	/**
	* RevertDepth
	* @ see		:	_cpyDepth�� �����ߴ� ������ ���� ������ ����
	*/
	void RevertDepth();

	/**
	* GetCpyDepth
	* @ see		:	_cpyDepth�� ���� ������
	* @ return	:	_cpyDepth ��
	*/
	int GetCpyDepth() const;

	/**
	* SetFontBold
	* @ param	:	������ �β��� ������ �� ����
	* @ see		:	���� ������ ������ ������ �β��� ����
	*/
	void SetFontBold(bool FontBold);

	/**
	* SetFontSize
	* @ param	:	ũ�⸦ ������ ��
	* @ see		:	���� ������ ������ ������ ũ�⸦ ����
	*/
	void SetFontSize(int nFontSize);

	/**
	* SetFontStyle
	* @ param	:	��Ʈ�� ��Ÿ�� ��Ʈ����
	* @ see		:	���� ������ ������ ������ ��Ÿ���� ���� (��: �ü�ü)
	*/
	void SetFontStyle(CString strFontStyle);

	/**
	* SetFontColor
	* @ param	:	������ ����
	* @ see		:	���� ������ ������ ������ ������ ����
	*/
	void SetFontColor(Color FontColor);

	/**
	* SetFontColor
	* @ param1	:	���� ������ ���������� �������� ��ǥ
	* @ param2	:	���� ������ ���������� ������ ��ǥ
	* @ param3	:	������ ����
	* @ see		:	������ ������ ������ ������ ����
	*/
	void SetFontColor(CPoint startPoint, CPoint endPoint, Color FontColor);

	/**
	* SetAutoRowSize
	* @ see		:	���� row���� ���� ������ ����
	*/
	void SetAutoRowSize();

	/**
	* SaveDepth
	* @ see		:	���� ������ ���� ����. �׻� SetRange �Լ� ��� �� ���� �������� ����Ѵ�.
	*/
	void SaveDepth();

	/**
	* SetBkColor
	* @ param	:	������ ����
	* @ see		:	������ ������ ���� ������ �����ϴ� �Լ�
	*/
	void SetBkColor(Color CellColor);

	/**
	* SetBkColor
	* @ param1	:	���� ������ ���������� �������� ��ǥ
	* @ param2	:	���� ������ ���������� ������ ��ǥ
	* @ param3	:	������ ����
	* @ see		:	������ ������ ���� ������ �����ϴ� �Լ�
	*/
	void SetBkColor(CPoint startPoint, CPoint endPoint, Color CellColor);

	/**
	* SetBoxBorder
	* @ param	:	�׵θ��� �β�
	* @ see		:	������ ������ ���� ��� �׵θ����� �����ϰ� �β��� �����ϴ� �Լ�
	*/
	void SetBoxBorder(BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderLeft
	* @ param1	:	�׵θ��� ��Ÿ��
	* @ param2	:	�׵θ��� �β�
	* @ see		:	������ ������ ���� ���� �׵θ��� ���� �β��� �����ϴ� �Լ�
	*/
	void SetBoxBorderLeft(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderRight
	* @ param1	:	�׵θ��� ��Ÿ��
	* @ param2	:	�׵θ��� �β�
	* @ see		:	������ ������ ���� ������ �׵θ��� ���� �β��� �����ϴ� �Լ�
	*/
	void SetBoxBorderRight(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderTop
	* @ param1	:	�׵θ��� ��Ÿ��
	* @ param2	:	�׵θ��� �β�
	* @ see		:	������ ������ ���� ���� �׵θ��� ���� �β��� �����ϴ� �Լ�
	*/
	void SetBoxBorderTop(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* SetBoxBorderBottom
	* @ param1	:	�׵θ��� ��Ÿ��
	* @ param2	:	�׵θ��� �β�
	* @ see		:	������ ������ ���� �Ʒ��� �׵θ��� ���� �β��� �����ϴ� �Լ�
	*/
	void SetBoxBorderBottom(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight);

	/**
	* VisibleWorkSheet
	* @ param1	:	���� ��ũ��Ʈ �ε�����ȣ 0���ͽ���
	* @ param2	:	��Ʈ�� ������ ���� (true = ǥ��, false = ����)
	* @ see		:	��Ʈ�� ����� �Լ�
	*/
	void VisibleWorkSheet(int nWorkSheetIndex, bool bVisible);

	/**
	* SetActiveWorkSheet
	* @ param	:	Ȱ����Ʈ�� ������ ��ũ��Ʈ �ε�����ȣ 0���ͽ���
	* @ see		:	��Ʈ�� Ȱ����Ʈ�� �����ϴ� �Լ�
	*/
	void SetActiveWorkSheet(int nWorkSheetIndex);

private:

	/**
	* SetRangeData
	* @ param1	:	������ ������ ù ������ ��������
	* @ param2	:	������ ������ �ε��� ���ĺ� 
	* @ see		:	���� ������ ������ �� �������� �ε��� (���ĺ�)�� �����ϴ� �Լ�
	* @ return	:	���� ���� ���� ����
	*/
	bool SetRangeData(int nLoopNum, CString& fixStr);

	/**
	* SetLoopCnt
	* @ param1	:	������ ������ ������ ����
	* @ see		:	SetRangeData �Լ����� ���ĺ��� ���ϱ����� ���������� ó���Ǿ� char ������ vector�� ��� �����ϴ� �Լ�
	* @ return	:	������ �ε��� ���ĺ��� char���·� vector�� ���ԵǾ� ����
	*/
	std::vector<char> SetLoopCnt(int nLoopNum);

	/**
	* SetLoopCnt
	* @ param1	:	������ �ؽ�Ʈ ����
	* @ param2	:	SetLoopCnt �Լ����� ����� ���ĺ��� ��� vector
	* @ see		:	SetLoopCnt �Լ��� ������ �� vector������ ���ڿ��� ������ִ� �Լ� (���ĺ����� ����ó��)
	* @ return	:	vector�� �Ѿ�� char ���� ���ĺ��ΰ�� true, ���ĺ��� �ƴѰ�� false
	*/
	bool SetFix(CString& prefix, std::vector<char> loopVector);

	/**
	* SetPieChart
	* @ see		:	������Ʈ �⺻ ������ ����
	*/
	void SetPieChart();

	/**
	* SetLineChart
	* @ see		:	������Ʈ �⺻ ������ ����
	*/
	void SetLineChart();

	/**
	* SetStickChart
	* @ see		:	������Ʈ �⺻ ������ ����
	*/
	void SetStickChart();

	/**
	* SetChartTitle
	* @ param	:	������ �׷����� Ÿ��Ʋ
	* @ see		:	������ �׷����� Ÿ��Ʋ�� �����ϴ� �Լ�
	*/
	void SetChartTitle(CString chartTitle);

	/**
	* SetPieChartColor
	* @ param1	:	ChartData���� ����� ������ ������
	* @ param2	:	������ ������ ����
	* @ see		:	������Ʈ ���� �� ���� �Լ�
	*/
	void SetPieChartColor(int legendSize, std::vector<int> legendColors);

	/**
	* SetLineChartColor
	* @ param1	:	������Ʈ�� �������� ����Ʈ�������� ����
	* @ param2	:	ChartData���� ����� ������ ������
	* @ param3	:	������ ������ ����
	* @ see		:	������Ʈ ���� �� ���� �Լ�
	*/
	void SetLineChartColor(int whichChart, int legendSize, std::vector<int> legendColors);

	/**
	* SetStickChartColor
	* @ param1	:	ChartData���� ����� ������ ������
	* @ param2	:	������ ������ ����
	* @ see		:	������Ʈ ���� �� ���� �Լ�
	*/
	void SetStickChartColor(int legendSize, std::vector<int> legendColors);

	/**
	* BorderReset
	* @ param	:	������ ��뼳�� ��
	* @ see		:	������ ���� ������ ��� ���� �Լ�
	*/
	void BorderReset(bool bReset);

	/**
	* DataSwapping
	* @ param1	:	������ ù��° ��
	* @ param2	:	������ �ι�° ��
	* @ see		:	Range ���� �ȿ��� ��ǥ����ϱ� ���� start�� end���� �������ִ� �Լ�
	*/
	void DataSwapping(LONG* nTargetData1, LONG* nTargetData2);
	
	/**
	* InsertChartData
	* @ param1	:	����� ��Ʈ�� ����
	* @ param2	:	��Ʈ�� �� ���ʿ� ���� �� ������
	* @ see		:	��Ʈ�� �����Ͱ� ���Ե� ��ũ��Ʈ�� �ڵ����� �����ϴ� �Լ�
	*/
	void InsertChartData(int whichChart, std::map<CString, CString> chartPairData);
};

