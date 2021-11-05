#include "pch.h"
#include "ExcelFormat.h"
 
CExcelFormat::CExcelFormat()
{
	eo = new ExcelObject;
	_bSetBr = true;
	_bSetBl = true;
	_bSetBb = true;
	_bSetBt = true;
	_bAttach = false;
	_bSetRange = false;
	_nDepth = 0;
	_nCpyDepth = 0;
	_nSaveHeight = 0;
	_nSaveTab = 0;
	_type = OAT_EXCEL;
}


CExcelFormat::~CExcelFormat()
{
	if (eo)
	{
		delete eo;
		eo = (ExcelObject*)nullptr;
	}
}
 
std::vector<char> CExcelFormat::SetLoopCnt(int nLoopNum)
{
	std::vector<char> vectorSeqeunce;
	const int ASC_IDX_CNT = 26;
	const int ASC_FIRST_IDX = 64;
	int nShare = 0;
	int nRemainder = 0;

	while (true)
	{
		nShare = nLoopNum / ASC_IDX_CNT;
		nRemainder = nLoopNum % ASC_IDX_CNT;

		if (nRemainder == 0)
		{
			nShare -= 1;
			nRemainder = ASC_IDX_CNT;
		}
		
		vectorSeqeunce.push_back(static_cast<char>(nRemainder + ASC_FIRST_IDX));
		nLoopNum = nShare;
		if (nShare <= ASC_IDX_CNT)
		{
			if (nShare > 0) vectorSeqeunce.push_back(static_cast<char>(nShare + ASC_FIRST_IDX));
			break;
		}
	}

	return vectorSeqeunce;
}

bool CExcelFormat::SetFix(CString& fix, std::vector<char> loopVector)
{
	bool bResult = true;
	CString strFix = _T("");
	for (int i = 0; i < (int)loopVector.size(); i++)
	{
		if (!strFix.IsEmpty())
		{
			strFix.Empty();
		} 

		if (!isalpha(loopVector.at(i)))
		{
			bResult = false;
			break;
		}
		strFix.Format("%c", loopVector.at(i));
		fix = strFix + fix;
	}

	return bResult;
}

bool CExcelFormat::SetRangeData(int nLoopNum, CString& fixStr)
{
	bool bResult = true;
	std::vector<char> loopVector = SetLoopCnt(nLoopNum);
	if (!SetFix(fixStr, loopVector))
	{
		bResult = false;
	}

	return bResult;
}

bool CExcelFormat::SetRange(unsigned int nTab, unsigned int nDepth, unsigned int nWidth, unsigned int nHeight, bool bParent /* = true*/)
{
	bool bResult = false;
	_bSetRange = false;
	BorderReset(false);

	if (nTab < 1)
	{
		return bResult;
	}
	
	nWidth -= 1;
	CString prefixStr = "";
	CString suffixStr = "";
	CString prefixNum = "";
	CString sufixNum = "";
	bool bPrefixSuccess = false;
	bool bSuffixSuccess = false;
	
	// set start column range
	if (SetRangeData(nTab, prefixStr))
	{
		bPrefixSuccess = true;
	}
	
	// set end column range
	if (SetRangeData(nTab + nWidth, suffixStr))
	{
		bSuffixSuccess = true;
	}
	
	// set format range success
	if (bPrefixSuccess && bSuffixSuccess)
	{
		// set start, end row range
		int startCellNum = nDepth + 1;
		int endCellNum = nDepth + nHeight;
		prefixNum.Format("%d", startCellNum);	
		sufixNum.Format("%d", endCellNum);		
		
		eo->_range = eo->_ws.get_Range(COleVariant(prefixStr + prefixNum), COleVariant(suffixStr + sufixNum));
		_bSetRange = true;
		bResult = true;
		if (bParent)
		{
			_nSaveHeight = nHeight;
			_nSaveTab = nTab;
		}
	}

	return bResult;
}

void CExcelFormat::DataSwapping(LONG* nTargetData1, LONG* nTargetData2)
{
	LONG nSwap = 0;
	if (*nTargetData1 > *nTargetData2)
	{
		nSwap = *nTargetData1;
		*nTargetData1 = *nTargetData2;
		*nTargetData2 = nSwap;
	}
}

void CExcelFormat::BorderReset(bool bReset)
{
	_bSetBr = bReset;
	_bSetBl = bReset;
	_bSetBb = bReset;
	_bSetBt = bReset;
}

void CExcelFormat::SetFontColor(Color FontColor)
{
	if (_bSetRange)
	{
		eo->_font = eo->_range.get_Font();
		eo->_font.put_Color(COleVariant((DOUBLE)FontColor));
	}
}

void CExcelFormat::SetFontColor(CPoint startPoint, CPoint endPoint, Color FontColor)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (_bSetRange)
	{
		CRange copyRange = eo->_range;

		if (SetRange(_nSaveTab + startPoint.x - 1, _nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetFontColor(FontColor);
			eo->_range = copyRange;
		}
	}
}

void CExcelFormat::SetFontStyle(CString strFontStyle)
{
	if (_bSetRange)
	{
		eo->_font = eo->_range.get_Font();
		eo->_font.put_Name(COleVariant(strFontStyle));
	}
}

void CExcelFormat::SetFontBold(bool FontBold)
{
	if (_bSetRange)
	{
		if (FontBold)
		{
			eo->_font = eo->_range.get_Font();
			eo->_font.put_Bold(COleVariant((short)TRUE));
		}
	}
}

void CExcelFormat::SetFontSize(int nFontSize)
{
	if (_bSetRange)
	{
		eo->_font = eo->_range.get_Font();
		eo->_font.put_Size(COleVariant(long(nFontSize)));
	}
}

void CExcelFormat::SetBkColor(Color CellColor)
{
	if (_bSetRange)
	{
		eo->_interior = eo->_range.get_Interior();
		eo->_interior.put_Color(COleVariant((DOUBLE)CellColor));
	}
}

void CExcelFormat::SetBkColor(CPoint startPoint, CPoint endPoint, Color CellColor)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (_bSetRange)
	{
		CRange copyRange = eo->_range;

		if (SetRange(_nSaveTab + startPoint.x - 1, _nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetBkColor(CellColor);
			eo->_range = copyRange;
		}
	}
}

void CExcelFormat::SetBoxBorder(BoldWeight BoxBoldWeight)
{
	if (_bSetRange)
	{
		eo->_borders = eo->_range.get_Borders();
		eo->_borders.put_LineStyle(COleVariant((short)BoxBoldWeight));
	}
}

void CExcelFormat::SetBoxBorderLeft(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (_bSetRange)
	{
		eo->_borders = eo->_range.get_Borders();
		eo->_bl = eo->_borders.get_Item(CExcelFormat::BORDER_STYLE_LEFT);
		eo->_bl.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			eo->_bl.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void CExcelFormat::SetBoxBorderRight(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (_bSetRange)
	{
		eo->_borders = eo->_range.get_Borders();
		eo->_br = eo->_borders.get_Item(CExcelFormat::BORDER_STYLE_RIGHT);
		eo->_br.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			eo->_br.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void CExcelFormat::SetBoxBorderTop(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (_bSetRange)
	{
		eo->_borders = eo->_range.get_Borders();
		eo->_bt = eo->_borders.get_Item(CExcelFormat::BORDER_STYLE_TOP);
		eo->_bt.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			eo->_bt.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void CExcelFormat::SetBoxBorderBottom(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (_bSetRange)
	{
		eo->_borders = eo->_range.get_Borders();
		eo->_bb = eo->_borders.get_Item(CExcelFormat::BORDER_STYLE_BOTTOM);
		eo->_bb.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			eo->_bb.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void CExcelFormat::SetRangeStyle(Color CellColor, Align TextAlign, bool BoxBorder, bool FontBold, BoldStyle BoxBoldStyle /* = BOLD_CONTINOUS*/, BoldWeight BoxBoldWeight /* = BOLD_THICK*/, BoldWeight CellBorderBold /* = BOLD_THIN*/)
{
	SetBkColor(CellColor);
	SetFontBold(FontBold);

	if (BoxBorder)
	{
		BorderReset(true);
		SetBoxStyle(BoxBorder, BoxBoldStyle, BoxBoldWeight);
	}
	
	SetHorizonTextAlign(TextAlign);
}

void CExcelFormat::SetBoxStyle(bool BoxBorder, BoldStyle BoxBoldStyle /* = BOLD_CONTINOUS*/, BoldWeight BoxBoldWeight /* = BOLD_THICK*/, BoldWeight CellBorderBold /* = BOLD_THIN*/)
{
	if (BoxBorder)
	{
		SetBoxBorder(CellBorderBold);
	}
	if (_bSetBl) SetBoxBorderLeft(BoxBoldStyle, BoxBoldWeight);
	if (_bSetBt) SetBoxBorderTop(BoxBoldStyle, BoxBoldWeight);
	if (_bSetBr) SetBoxBorderRight(BoxBoldStyle, BoxBoldWeight);
	if (_bSetBb) SetBoxBorderBottom(BoxBoldStyle, BoxBoldWeight);
}

void CExcelFormat::SetBlOn(bool setBl)
{
	this->_bSetBl = setBl;
}

void CExcelFormat::SetBtOn(bool setBt)
{
	this->_bSetBt = setBt;
}

void CExcelFormat::SetBbOn(bool setBb)
{
	this->_bSetBb = setBb;
}

void CExcelFormat::SetBrOn(bool setBr)
{
	this->_bSetBr = setBr;
}

double CExcelFormat::GetColumnFixelValue()
{
	double dColumnWidth = 0;
	if (_bSetRange)
	{
		COleVariant cols;
		cols = eo->_range.get_ColumnWidth();
		dColumnWidth = cols.dblVal;
	}
	return dColumnWidth;
}

double CExcelFormat::GetRowFixelValue()
{
	double dRowHeight = 0;
	if (_bSetRange)
	{
		COleVariant cols;
		cols = eo->_range.get_RowHeight();
		dRowHeight = cols.dblVal;
	}
	return dRowHeight;
}


void CExcelFormat::SetAutoFit(bool bEntireType)
{
	if (_bSetRange)
	{
		CRange cols;
		if (bEntireType)
		{
			cols = eo->_range.get_EntireRow();
		}
		else
		{
			cols = eo->_range.get_EntireColumn();
		}
		cols.AutoFit();
	}
}

void CExcelFormat::VisibleWorkSheet(int nWorkSheetIndex, bool bVisible)
{
	CWorksheet ws = eo->_ws;
	eo->_ws = eo->_wss.get_Item(COleVariant(short(nWorkSheetIndex + 1)));
	eo->_ws.put_Visible(bVisible ? -1 : 0);	// -1 -> 시트 표시   0 -> 시트 숨김
	eo->_ws = ws;
}

void CExcelFormat::AddWorkSheet(CString WorkSheetName)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	covOptional.vt = VT_DISPATCH;
	covOptional.pdispVal = eo->_ws.m_lpDispatch;
	eo->_ws.m_lpDispatch->AddRef();
	eo->_ws = eo->_wss.Add(_colOption, covOptional, COleVariant((short)1), _colOption);
	eo->_ws.put_Name(WorkSheetName);
	_nDepth = 0;
	_nCpyDepth = 0;
	_bSetRange = false;
	_nSaveHeight = 0;
	_nSaveTab = 0;
	BorderReset(false);
}


bool CExcelFormat::StartExcel(CString WorkSheetName)
{
	if (!eo->_app.CreateDispatch("Excel.Application"))
	{
		return false;
	}
	eo->_app.put_Visible(false);

	eo->_app.put_SheetsInNewWorkbook(1);

	_colTrue = (short)TRUE;
	_colFalse = (short)FALSE;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	memcpy(_colOption, covOptional, sizeof(covOptional));

	eo->_wbs.AttachDispatch(eo->_app.get_Workbooks());
	eo->_wb.AttachDispatch(eo->_wbs.Add(_colOption));
	eo->_wss = eo->_wb.get_Sheets();
	eo->_ws = eo->_wss.get_Item(COleVariant((short)1));
	eo->_ws.put_Name(WorkSheetName);
	eo->_range.AttachDispatch(eo->_ws.get_Cells(), true);
	
	_bAttach = true;

	return true;
}

void CExcelFormat::SaveWorkBook(CString strFilePath)
{
	eo->_app.put_DisplayAlerts(FALSE);
	eo->_wb._SaveAs(COleVariant(strFilePath), _colOption, _colOption, _colOption, _colOption, _colOption, 0, _colOption, _colOption, _colOption, _colOption, _colOption);
	eo->_app.Quit();
	_nDepth = 0;
	_nCpyDepth = 0;
}

void CExcelFormat::CloseExcel(CString strFilePath)
{
	if (_bAttach)
	{
		SaveWorkBook(strFilePath);

		eo->_hpb.ReleaseDispatch();
		eo->_hpbs.ReleaseDispatch();
		eo->_vpb.ReleaseDispatch();
		eo->_vpbs.ReleaseDispatch();
		eo->_wnd.ReleaseDispatch();
		eo->_wnds.ReleaseDispatch();
		eo->_cform.ReleaseDispatch();
		eo->_fform.ReleaseDispatch();
		eo->_point.ReleaseDispatch();
		eo->_srs.ReleaseDispatch();
		eo->_key.ReleaseDispatch();
		eo->_entry.ReleaseDispatch();
		eo->_legend.ReleaseDispatch();
		eo->_title.ReleaseDispatch();
		eo->_chart.ReleaseDispatch();
		eo->_charts.ReleaseDispatch();
		eo->_bt.ReleaseDispatch();
		eo->_bb.ReleaseDispatch();
		eo->_br.ReleaseDispatch();
		eo->_bl.ReleaseDispatch();
		eo->_borders.ReleaseDispatch();
		eo->_font.ReleaseDispatch();
		eo->_interior.ReleaseDispatch();
		eo->_range.ReleaseDispatch();
		eo->_ws.ReleaseDispatch();
		eo->_wss.ReleaseDispatch();
		eo->_wb.ReleaseDispatch();
		eo->_wbs.ReleaseDispatch();
		eo->_app.ReleaseDispatch();

		_bAttach = false;
	}

	if (!_bAttach) 
	{
		::ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox("엑셀 저장에 실패하였습니다.");
	}
}

void CExcelFormat::SetPageBreak(bool bUseVerticalBreak, bool bUseHorizonBreak)
{
	eo->_wnds = eo->_wb.get_Windows();
	eo->_wnd = eo->_wnds.get_Item(COleVariant((long)1));
	eo->_wnd.put_View(2);

	if (bUseVerticalBreak)
	{
		eo->_vpbs = eo->_ws.get_VPageBreaks();

		for (int i = 0; i < (int)eo->_vpbs.get_Count(); i++)
		{
			eo->_vpb = eo->_vpbs.get_Item(i + 1);
			eo->_vpb.DragOff(-4161, 1);
		}
	}

	if (bUseHorizonBreak)
	{
		eo->_hpbs = eo->_ws.get_HPageBreaks();

		for (int i = 0; i < (int)eo->_hpbs.get_Count(); i++)
		{
			eo->_hpb = eo->_hpbs.get_Item(i + 1);
			eo->_hpb.DragOff(-4121, 1);
		}
	}

	eo->_wnd.put_View(1);
}

void CExcelFormat::SetHorizonTextAlign(Align TextAlign)
{
	if (_bSetRange)
	{
		if (TextAlign == TEXT_ALIGN_LEFT || TextAlign == TEXT_ALIGN_CENTER || TextAlign == TEXT_ALIGN_RIGHT)
			eo->_range.put_HorizontalAlignment(COleVariant((short)TextAlign));
	}
}

void CExcelFormat::SetVerticalTextAlign(Align TextAlign)
{
	if (_bSetRange)
	{
		if (TextAlign == TEXT_ALIGN_TOP || TextAlign == TEXT_ALIGN_CENTER || TextAlign == TEXT_ALIGN_BOTTOM)
			eo->_range.put_VerticalAlignment(COleVariant((short)TextAlign));
	}
}

void CExcelFormat::SetAutoLineChange(BOOL setLineEnd)
{
	if (_bSetRange)
	{
		eo->_range.put_WrapText(COleVariant((short)setLineEnd));
	}
}

void CExcelFormat::SetIndent(int nIndentValue)
{
	if (_bSetRange)
	{
		if (nIndentValue >= 0 && nIndentValue <= 15)
			eo->_range.put_IndentLevel(COleVariant((long)nIndentValue));
	}
}

void CExcelFormat::SaveDepth()
{
	this->_nDepth += _nSaveHeight;
	_nSaveHeight = 0;
}

void CExcelFormat::SetItem(int nWidth, int nHeight, CString strText)
{
	if (_bSetRange)
	{
		eo->_range.put_Item(COleVariant((long)nHeight), COleVariant((long)nWidth), COleVariant(strText));
	}
}

void CExcelFormat::SetItem(CString strText)
{
	if (_bSetRange)
	{
		eo->_range.put_Value2(COleVariant(strText));
	}
}

void CExcelFormat::SetMerge()
{
	if (_bSetRange)
	{
		eo->_range.Merge(COleVariant(0L));
	}
}

void CExcelFormat::SetMerge(CPoint startPoint, CPoint endPoint)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (_bSetRange)
	{
		CRange copyRange = eo->_range;

		if (SetRange(_nSaveTab + startPoint.x - 1, _nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetMerge();
			eo->_range = copyRange;
		}
	}
}

void CExcelFormat::SetLineEnd(unsigned int nLineEnd /* = 1*/)
{
	this->_nDepth += nLineEnd;
}

void CExcelFormat::SetDepth(int nDepth)
{
	this->_nDepth = nDepth;
}

int CExcelFormat::GetDepth() const
{
	return this->_nDepth;
}

int CExcelFormat::GetCpyDepth() const
{
	return this->_nCpyDepth;
}

void CExcelFormat::ResetDepth()
{
	_nCpyDepth = _nDepth;
	_nDepth = 0;
}

void CExcelFormat::RevertDepth()
{
	_nDepth = _nCpyDepth;
}

void CExcelFormat::SetAutoRowSize()
{
	double dRowFixelValue = GetRowFixelValue();
	if (dRowFixelValue > HEADER_ROW_VALUE)
	{
		SetAutoFit(AUTO_ROW);
	}
	else
	{
		SetCellHeight(HEADER_ROW_VALUE);
	}
}

void CExcelFormat::SetCellWidth(double dWidth)
{
	if (_bSetRange)
	{
		eo->_range.put_ColumnWidth(COleVariant(dWidth));
	}
}

void CExcelFormat::SetCellHeight(double dHeight)
{
	if (_bSetRange)
	{
		eo->_range.put_RowHeight(COleVariant(dHeight));
	}
}

CString CExcelFormat::SetVisibleTime(CString strTime)
{
	CString formatTime;
	formatTime.Format(_T("'%s"), strTime);
	return formatTime;
}

void CExcelFormat::InsertChartData(int whichChart, std::map<CString, CString> chartPairData)
{
	AddWorkSheet("통계 데이터");
	if (SetRange(1, GetDepth(), (int)chartPairData.size(), 1))
	{
		int nIndex = 0;
		for (std::map<CString, CString>::iterator iter = chartPairData.begin(); iter != chartPairData.end(); iter++)
		{
			++nIndex;
			SetItem(nIndex, 1, iter->first);
		}
		SaveDepth();
	}

	if (SetRange(1, GetDepth(), (int)chartPairData.size(), 1))
	{
		int nIndex = 0;
		CString strFormat;
		for (std::map<CString, CString>::iterator iter = chartPairData.begin(); iter != chartPairData.end(); iter++)
		{
			++nIndex;
			CString strValue = iter->second;
			if (whichChart == CHART_PIE || whichChart == CHART_DONUT)
			{
				if (_ttof(strValue) > 0)
				{
					strFormat.Format("%s%%", strValue);
					SetItem(nIndex, 1, strFormat);
				}
			}
			else
			{
				strFormat.Format("%s%%", strValue);
				SetItem(nIndex, 1, strFormat);
			}
		}
		SaveDepth();
	}
}

void CExcelFormat::SetChart(ChartData chartData)
{
	InsertChartData(chartData.kindOfChart, chartData.chartPairData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	covOptional.vt = VT_DISPATCH;
	covOptional.pdispVal = eo->_ws.m_lpDispatch;
	eo->_ws.m_lpDispatch->AddRef();

	eo->_charts = eo->_wb.get_Charts();
	eo->_chart = eo->_charts.Add(_colOption, covOptional, _colOption);
	eo->_chart.put_Name("통계 그래프");
	
	eo->_chart.put_ChartType(chartData.kindOfChart);

	if (chartData.kindOfChart == CHART_PIE || chartData.kindOfChart == CHART_DONUT)
	{
		SetPieChart();
		SetPieChartColor((int)chartData.chartPairData.size(), chartData.legendColors);
	}
	else if (chartData.kindOfChart == CHART_LINE || chartData.kindOfChart == CHART_POINTLINE)
	{
		SetLineChart();
		SetLineChartColor(chartData.kindOfChart, (int)chartData.chartPairData.size(), chartData.legendColors);
	}
	else if (chartData.kindOfChart == CHART_STICK)
	{
		SetStickChart();
		SetStickChartColor((int)chartData.chartPairData.size(), chartData.legendColors);
	}
	
	SetChartTitle(chartData.strChartTitle);
	VisibleWorkSheet(eo->_ws.get_Index() - 1, false);
	SetActiveWorkSheet(0);
}

void CExcelFormat::SetActiveWorkSheet(int nWorkSheetIndex)
{
	eo->_ws = eo->_wss.get_Item(COleVariant(long(nWorkSheetIndex + 1)));
	eo->_ws.Activate();
}

void CExcelFormat::SetStickChart()
{
	eo->_chart.SetElement(2);	//상단에 차트제목표시		(title)
	eo->_chart.SetElement(101);	//오른쪽에 범례표시			(legend)
	eo->_chart.SetElement(201);	//중앙에 데이터레이블표시
}

void CExcelFormat::SetPieChart()
{
	// https://docs.microsoft.com/ko-kr/office/vba/api/office.msochartelementtype 차트 엘레먼트값
	eo->_chart.SetElement(2);	//상단에 차트제목표시		(title)
	eo->_chart.SetElement(101);	//오른쪽에 범례표시			(legend)
	eo->_chart.SetElement(201);	//중앙에 데이터레이블표시

	//첫번째 인자 데이터레이블유형 //https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.excel.xldatalabelstype?view=excel-pia
	//두번째 인자 범례사용
	//세번째 인자 텍스트노출
	//네번째 인자 계열출력
	//다섯번째 인자 데이터 헤더텍스트 출력(제목)
	//여섯번째 인자 데이터 바디텍스트 출력(값)
	//일곱번째 인자 데이터 퍼센테이지 출력
	//여덟번째 인자 출력텍스트 간 구분자설정
	eo->_chart.ApplyDataLabels(5, _colTrue, _colTrue, _colFalse, _colFalse, _colTrue, _colFalse, _colTrue, _colFalse, COleVariant(_T("\n")));
}

void CExcelFormat::SetLineChart()
{
	eo->_chart.SetElement(2);
	eo->_chart.SetElement(101);
}

void CExcelFormat::SetChartTitle(CString chartTitle)
{
	if (eo->_chart.get_HasTitle())
	{
		eo->_title = eo->_chart.get_ChartTitle();
		eo->_title.put_Caption(chartTitle);
	}
}

void CExcelFormat::SetLineChartColor(int whichChart, int legendSize, std::vector<int> legendColors)
{
	if (eo->_chart.get_HasLegend())
	{
		eo->_legend = eo->_chart.get_Legend();
		eo->_entry = eo->_legend.LegendEntries(COleVariant((short)1));
		eo->_key = eo->_entry.get_LegendKey();
		eo->_borders = eo->_key.get_Border();
		eo->_borders.put_Color(COleVariant((DOUBLE)COLOR_THICK_GREY));

		if (whichChart == CHART_POINTLINE)
		{
			eo->_srs = eo->_chart.SeriesCollection(COleVariant(long(1)));
			eo->_interior = eo->_range.get_Interior();
			for (int i = 1; i <= legendSize; i++)
			{
				eo->_interior.put_ColorIndex(COleVariant(long(legendColors.at(i - 1))));
				eo->_point = eo->_srs.Points(COleVariant(long(i)));
				eo->_point.put_MarkerStyle(8);
				eo->_point.put_MarkerSize(8);
				for (int j = 1; j <= legendSize; j++)
				{
					if (i == j)
					{	
						eo->_point.put_MarkerBackgroundColor(long(eo->_interior.get_Color().dblVal));
						eo->_point.put_MarkerForegroundColor(COLOR_WHITE);
						break;
					}
				}
			}
		}

		eo->_chart.put_HasLegend(FALSE);
	}
}

void CExcelFormat::SetPieChartColor(int legendSize, std::vector<int> legendColors)
{
	if (eo->_chart.get_HasLegend())
	{
		for (int i = 1; i <= legendSize; i++)
		{
			eo->_legend = eo->_chart.get_Legend();
			eo->_entry = eo->_legend.LegendEntries(COleVariant((short)i));
			eo->_key = eo->_entry.get_LegendKey();
			eo->_fform = eo->_key.get_Fill();
			eo->_cform = eo->_fform.get_ForeColor();
			for (int j = 1; j <= legendSize; j++)
			{
				if (i == j)
				{
					eo->_cform.put_SchemeColor(legendColors.at(i - 1));
					break;
				}
			}
		}
	}
}

void CExcelFormat::SetStickChartColor(int legendSize, std::vector<int> legendColors)
{
	if (eo->_chart.get_HasLegend())
	{
		eo->_srs = eo->_chart.SeriesCollection(COleVariant(long(1)));
		for (int i = 1; i <= legendSize; i++)
		{
			eo->_point = eo->_srs.Points(COleVariant(long(i)));
			eo->_fform = eo->_point.get_Fill();
			eo->_cform = eo->_fform.get_ForeColor();
			for (int j = 1; j <= legendSize; j++)
			{
				if (i == j)
				{
					eo->_cform.put_SchemeColor(legendColors.at(i - 1));
					break;
				}
			}
		}
	}
}

int main()
{
	return 0;
}