#include "pch.h"
#include "ExcelAutomationManager.h"
 
ExcelAutomationManager::ExcelAutomationManager()
{
	m_eo = new ExcelObject;
	m_bSetBr = true;
	m_bSetBl = true;
	m_bSetBb = true;
	m_bSetBt = true;
	im_bAttach = false;
	m_bSetRange = false;
	m_nDepth = 0;
	m_nCpyDepth = 0;
	m_nSaveHeight = 0;
	m_nSaveTab = 0;
	im_type = OAT_EXCEL;
}


ExcelAutomationManager::~ExcelAutomationManager()
{
	if (m_eo)
	{
		delete m_eo;
		m_eo = (ExcelObject*)nullptr;
	}
}
 
std::vector<char> ExcelAutomationManager::SetLoopCnt(int nLoopNum)
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

bool ExcelAutomationManager::SetFix(CString& fix, std::vector<char> loopVector)
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

bool ExcelAutomationManager::SetRangeData(int nLoopNum, CString& fixStr)
{
	bool bResult = true;
	std::vector<char> loopVector = SetLoopCnt(nLoopNum);
	if (!SetFix(fixStr, loopVector))
	{
		bResult = false;
	}

	return bResult;
}

bool ExcelAutomationManager::SetRange(unsigned int nTab, unsigned int nDepth, unsigned int nWidth, unsigned int nHeight, bool bParent /* = true*/)
{
	bool bResult = false;
	m_bSetRange = false;
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
		
		m_eo->range = m_eo->ws.get_Range(COleVariant(prefixStr + prefixNum), COleVariant(suffixStr + sufixNum));
		m_bSetRange = true;
		bResult = true;
		if (bParent)
		{
			m_nSaveHeight = nHeight;
			m_nSaveTab = nTab;
		}
	}

	return bResult;
}

void ExcelAutomationManager::DataSwapping(LONG* nTargetData1, LONG* nTargetData2)
{
	LONG nSwap = 0;
	if (*nTargetData1 > *nTargetData2)
	{
		nSwap = *nTargetData1;
		*nTargetData1 = *nTargetData2;
		*nTargetData2 = nSwap;
	}
}

void ExcelAutomationManager::BorderReset(bool bReset)
{
	m_bSetBr = bReset;
	m_bSetBl = bReset;
	m_bSetBb = bReset;
	m_bSetBt = bReset;
}

void ExcelAutomationManager::SetFontColor(OAColor FontColor)
{
	if (m_bSetRange)
	{
		m_eo->font = m_eo->range.get_Font();
		m_eo->font.put_Color(COleVariant((DOUBLE)FontColor));
	}
}

void ExcelAutomationManager::SetFontColor(CPoint startPoint, CPoint endPoint, OAColor FontColor)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (m_bSetRange)
	{
		CRange copyRange = m_eo->range;

		if (SetRange(m_nSaveTab + startPoint.x - 1, m_nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetFontColor(FontColor);
			m_eo->range = copyRange;
		}
	}
}

void ExcelAutomationManager::SetFontStyle(CString strFontStyle)
{
	if (m_bSetRange)
	{
		m_eo->font = m_eo->range.get_Font();
		m_eo->font.put_Name(COleVariant(strFontStyle));
	}
}

void ExcelAutomationManager::SetFontBold(bool FontBold)
{
	if (m_bSetRange)
	{
		if (FontBold)
		{
			m_eo->font = m_eo->range.get_Font();
			m_eo->font.put_Bold(COleVariant((short)TRUE));
		}
	}
}

void ExcelAutomationManager::SetFontSize(int nFontSize)
{
	if (m_bSetRange)
	{
		m_eo->font = m_eo->range.get_Font();
		m_eo->font.put_Size(COleVariant(long(nFontSize)));
	}
}

void ExcelAutomationManager::SetBkColor(OAColor CellColor)
{
	if (m_bSetRange)
	{
		m_eo->interior = m_eo->range.get_Interior();
		m_eo->interior.put_Color(COleVariant((DOUBLE)CellColor));
	}
}

void ExcelAutomationManager::SetBkColor(CPoint startPoint, CPoint endPoint, OAColor CellColor)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (m_bSetRange)
	{
		CRange copyRange = m_eo->range;

		if (SetRange(m_nSaveTab + startPoint.x - 1, m_nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetBkColor(CellColor);
			m_eo->range = copyRange;
		}
	}
}

void ExcelAutomationManager::SetBoxBorder(BoldWeight BoxBoldWeight)
{
	if (m_bSetRange)
	{
		m_eo->borders = m_eo->range.get_Borders();
		m_eo->borders.put_LineStyle(COleVariant((short)BoxBoldWeight));
	}
}

void ExcelAutomationManager::SetBoxBorderLeft(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (m_bSetRange)
	{
		m_eo->borders = m_eo->range.get_Borders();
		m_eo->bl = m_eo->borders.get_Item(ExcelAutomationManager::BORDER_STYLE_LEFT);
		m_eo->bl.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			m_eo->bl.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void ExcelAutomationManager::SetBoxBorderRight(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (m_bSetRange)
	{
		m_eo->borders = m_eo->range.get_Borders();
		m_eo->br = m_eo->borders.get_Item(ExcelAutomationManager::BORDER_STYLE_RIGHT);
		m_eo->br.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			m_eo->br.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void ExcelAutomationManager::SetBoxBorderTop(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (m_bSetRange)
	{
		m_eo->borders = m_eo->range.get_Borders();
		m_eo->bt = m_eo->borders.get_Item(ExcelAutomationManager::BORDER_STYLE_TOP);
		m_eo->bt.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			m_eo->bt.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void ExcelAutomationManager::SetBoxBorderBottom(BoldStyle BoxBoldStyle, BoldWeight BoxBoldWeight)
{
	if (m_bSetRange)
	{
		m_eo->borders = m_eo->range.get_Borders();
		m_eo->bb = m_eo->borders.get_Item(ExcelAutomationManager::BORDER_STYLE_BOTTOM);
		m_eo->bb.put_LineStyle(COleVariant((short)BoxBoldStyle));
		if (BoxBoldStyle != BOLD_STYLE_LINESTYLENONE)
		{
			m_eo->bb.put_Weight(COleVariant((short)BoxBoldWeight));
		}
	}
}

void ExcelAutomationManager::SetRangeStyle(OAColor CellColor, Align TextAlign, bool BoxBorder, bool FontBold, BoldStyle BoxBoldStyle /* = BOLD_CONTINOUS*/, BoldWeight BoxBoldWeight /* = BOLD_THICK*/, BoldWeight CellBorderBold /* = BOLD_THIN*/)
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

void ExcelAutomationManager::SetBoxStyle(bool BoxBorder, BoldStyle BoxBoldStyle /* = BOLD_CONTINOUS*/, BoldWeight BoxBoldWeight /* = BOLD_THICK*/, BoldWeight CellBorderBold /* = BOLD_THIN*/)
{
	if (BoxBorder)
	{
		SetBoxBorder(CellBorderBold);
	}
	if (m_bSetBl) SetBoxBorderLeft(BoxBoldStyle, BoxBoldWeight);
	if (m_bSetBt) SetBoxBorderTop(BoxBoldStyle, BoxBoldWeight);
	if (m_bSetBr) SetBoxBorderRight(BoxBoldStyle, BoxBoldWeight);
	if (m_bSetBb) SetBoxBorderBottom(BoxBoldStyle, BoxBoldWeight);
}

void ExcelAutomationManager::SetBlOn(bool setBl)
{
	this->m_bSetBl = setBl;
}

void ExcelAutomationManager::SetBtOn(bool setBt)
{
	this->m_bSetBt = setBt;
}

void ExcelAutomationManager::SetBbOn(bool setBb)
{
	this->m_bSetBb = setBb;
}

void ExcelAutomationManager::SetBrOn(bool setBr)
{
	this->m_bSetBr = setBr;
}

double ExcelAutomationManager::GetColumnFixelValue()
{
	double dColumnWidth = 0;
	if (m_bSetRange)
	{
		COleVariant cols;
		cols = m_eo->range.get_ColumnWidth();
		dColumnWidth = cols.dblVal;
	}
	return dColumnWidth;
}

double ExcelAutomationManager::GetRowFixelValue()
{
	double dRowHeight = 0;
	if (m_bSetRange)
	{
		COleVariant cols;
		cols = m_eo->range.get_RowHeight();
		dRowHeight = cols.dblVal;
	}
	return dRowHeight;
}


void ExcelAutomationManager::SetAutoFit(bool bEntireType)
{
	if (m_bSetRange)
	{
		CRange cols;
		if (bEntireType)
		{
			cols = m_eo->range.get_EntireRow();
		}
		else
		{
			cols = m_eo->range.get_EntireColumn();
		}
		cols.AutoFit();
	}
}

void ExcelAutomationManager::VisibleWorkSheet(int nWorkSheetIndex, bool bVisible)
{
	CWorksheet ws = m_eo->ws;
	m_eo->ws = m_eo->wss.get_Item(COleVariant(short(nWorkSheetIndex + 1)));
	m_eo->ws.put_Visible(bVisible ? -1 : 0);	// -1 -> 시트 표시   0 -> 시트 숨김
	m_eo->ws = ws;
}

void ExcelAutomationManager::AddWorkSheet(CString WorkSheetName)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	covOptional.vt = VT_DISPATCH;
	covOptional.pdispVal = m_eo->ws.m_lpDispatch;
	m_eo->ws.m_lpDispatch->AddRef();
	m_eo->ws = m_eo->wss.Add(im_colOption, covOptional, COleVariant((short)1), im_colOption);
	m_eo->ws.put_Name(WorkSheetName);
	m_nDepth = 0;
	m_nCpyDepth = 0;
	m_bSetRange = false;
	m_nSaveHeight = 0;
	m_nSaveTab = 0;
	BorderReset(false);
}


bool ExcelAutomationManager::StartExcel(CString WorkSheetName)
{
	if (!m_eo->app.CreateDispatch("Excel.Application"))
	{
		return false;
	}
	m_eo->app.put_Visible(false);

	m_eo->app.put_SheetsInNewWorkbook(1);

	StartProcess();

	m_eo->wbs.AttachDispatch(m_eo->app.get_Workbooks());
	m_eo->wb.AttachDispatch(m_eo->wbs.Add(im_colOption));
	m_eo->wss = m_eo->wb.get_Sheets();
	m_eo->ws = m_eo->wss.get_Item(COleVariant((short)1));
	m_eo->ws.put_Name(WorkSheetName);
	m_eo->range.AttachDispatch(m_eo->ws.get_Cells(), true);
	
	return true;
}

void ExcelAutomationManager::SaveWorkBook(CString strFilePath)
{
	m_eo->app.put_DisplayAlerts(FALSE);
	m_eo->wb._SaveAs(COleVariant(strFilePath), im_colOption, im_colOption, im_colOption, im_colOption, im_colOption, 0, im_colOption, im_colOption, im_colOption, im_colOption, im_colOption);
	m_eo->app.Quit();
	m_nDepth = 0;
	m_nCpyDepth = 0;
}

void ExcelAutomationManager::CloseExcel(CString strFilePath, bool executeShell)
{
	if (im_bAttach)
	{
		SaveWorkBook(strFilePath);

		m_eo->hpb.ReleaseDispatch();
		m_eo->hpbs.ReleaseDispatch();
		m_eo->vpb.ReleaseDispatch();
		m_eo->vpbs.ReleaseDispatch();
		m_eo->wnd.ReleaseDispatch();
		m_eo->wnds.ReleaseDispatch();
		m_eo->cform.ReleaseDispatch();
		m_eo->fform.ReleaseDispatch();
		m_eo->point.ReleaseDispatch();
		m_eo->srs.ReleaseDispatch();
		m_eo->key.ReleaseDispatch();
		m_eo->entry.ReleaseDispatch();
		m_eo->legend.ReleaseDispatch();
		m_eo->title.ReleaseDispatch();
		m_eo->chart.ReleaseDispatch();
		m_eo->charts.ReleaseDispatch();
		m_eo->bt.ReleaseDispatch();
		m_eo->bb.ReleaseDispatch();
		m_eo->br.ReleaseDispatch();
		m_eo->bl.ReleaseDispatch();
		m_eo->borders.ReleaseDispatch();
		m_eo->font.ReleaseDispatch();
		m_eo->interior.ReleaseDispatch();
		m_eo->range.ReleaseDispatch();
		m_eo->ws.ReleaseDispatch();
		m_eo->wss.ReleaseDispatch();
		m_eo->wb.ReleaseDispatch();
		m_eo->wbs.ReleaseDispatch();
		m_eo->app.ReleaseDispatch();

		im_bAttach = false;
	}

	CloseProcess(strFilePath, executeShell);
}

void ExcelAutomationManager::SetPageBreak(bool bUseVerticalBreak, bool bUseHorizonBreak)
{
	m_eo->wnds = m_eo->wb.get_Windows();
	m_eo->wnd = m_eo->wnds.get_Item(COleVariant((long)1));
	m_eo->wnd.put_View(2);

	if (bUseVerticalBreak)
	{
		m_eo->vpbs = m_eo->ws.get_VPageBreaks();

		for (int i = 0; i < (int)m_eo->vpbs.get_Count(); i++)
		{
			m_eo->vpb = m_eo->vpbs.get_Item(i + 1);
			m_eo->vpb.DragOff(-4161, 1);
		}
	}

	if (bUseHorizonBreak)
	{
		m_eo->hpbs = m_eo->ws.get_HPageBreaks();

		for (int i = 0; i < (int)m_eo->hpbs.get_Count(); i++)
		{
			m_eo->hpb = m_eo->hpbs.get_Item(i + 1);
			m_eo->hpb.DragOff(-4121, 1);
		}
	}

	m_eo->wnd.put_View(1);
}

void ExcelAutomationManager::SetHorizonTextAlign(Align TextAlign)
{
	if (m_bSetRange)
	{
		if (TextAlign == TEXT_ALIGN_LEFT || TextAlign == TEXT_ALIGN_CENTER || TextAlign == TEXT_ALIGN_RIGHT)
			m_eo->range.put_HorizontalAlignment(COleVariant((short)TextAlign));
	}
}

void ExcelAutomationManager::SetVerticalTextAlign(Align TextAlign)
{
	if (m_bSetRange)
	{
		if (TextAlign == TEXT_ALIGN_TOP || TextAlign == TEXT_ALIGN_CENTER || TextAlign == TEXT_ALIGN_BOTTOM)
			m_eo->range.put_VerticalAlignment(COleVariant((short)TextAlign));
	}
}

void ExcelAutomationManager::SetAutoLineChange(BOOL setLineEnd)
{
	if (m_bSetRange)
	{
		m_eo->range.put_WrapText(COleVariant((short)setLineEnd));
	}
}

void ExcelAutomationManager::SetIndent(int nIndentValue)
{
	if (m_bSetRange)
	{
		if (nIndentValue >= 0 && nIndentValue <= 15)
			m_eo->range.put_IndentLevel(COleVariant((long)nIndentValue));
	}
}

void ExcelAutomationManager::SaveDepth()
{
	this->m_nDepth += m_nSaveHeight;
	m_nSaveHeight = 0;
}

void ExcelAutomationManager::SetItem(int nWidth, int nHeight, CString strText)
{
	if (m_bSetRange)
	{
		m_eo->range.put_Item(COleVariant((long)nHeight), COleVariant((long)nWidth), COleVariant(strText));
	}
}

void ExcelAutomationManager::SetItem(CString strText)
{
	if (m_bSetRange)
	{
		m_eo->range.put_Value2(COleVariant(strText));
	}
}

void ExcelAutomationManager::SetMerge()
{
	if (m_bSetRange)
	{
		m_eo->range.Merge(COleVariant(0L));
	}
}

void ExcelAutomationManager::SetMerge(CPoint startPoint, CPoint endPoint)
{
	if (startPoint.x < 1 || startPoint.y < 1 || endPoint.x < 1 || endPoint.y < 1) return;

	DataSwapping(&startPoint.x, &endPoint.x);
	DataSwapping(&startPoint.y, &endPoint.y);

	if (m_bSetRange)
	{
		CRange copyRange = m_eo->range;

		if (SetRange(m_nSaveTab + startPoint.x - 1, m_nDepth + startPoint.y - 1, endPoint.x - startPoint.x + 1, endPoint.y - startPoint.y + 1, false))
		{
			SetMerge();
			m_eo->range = copyRange;
		}
	}
}

void ExcelAutomationManager::SetLineEnd(unsigned int nLineEnd /* = 1*/)
{
	this->m_nDepth += nLineEnd;
}

void ExcelAutomationManager::SetDepth(int nDepth)
{
	this->m_nDepth = nDepth;
}

int ExcelAutomationManager::GetDepth() const
{
	return this->m_nDepth;
}

int ExcelAutomationManager::GetCpyDepth() const
{
	return this->m_nCpyDepth;
}

void ExcelAutomationManager::ResetDepth()
{
	m_nCpyDepth = m_nDepth;
	m_nDepth = 0;
}

void ExcelAutomationManager::RevertDepth()
{
	m_nDepth = m_nCpyDepth;
}

void ExcelAutomationManager::SetAutoRowSize()
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

void ExcelAutomationManager::SetCellWidth(double dWidth)
{
	if (m_bSetRange)
	{
		m_eo->range.put_ColumnWidth(COleVariant(dWidth));
	}
}

void ExcelAutomationManager::SetCellHeight(double dHeight)
{
	if (m_bSetRange)
	{
		m_eo->range.put_RowHeight(COleVariant(dHeight));
	}
}

CString ExcelAutomationManager::SetVisibleTime(CString strTime)
{
	CString formatTime;
	formatTime.Format(_T("'%s"), strTime);
	return formatTime;
}

void ExcelAutomationManager::InsertChartData(int whichChart, std::map<CString, CString> chartPairData)
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

void ExcelAutomationManager::SetChart(ChartData chartData)
{
	InsertChartData(chartData.kindOfChart, chartData.chartPairData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	covOptional.vt = VT_DISPATCH;
	covOptional.pdispVal = m_eo->ws.m_lpDispatch;
	m_eo->ws.m_lpDispatch->AddRef();

	m_eo->charts = m_eo->wb.get_Charts();
	m_eo->chart = m_eo->charts.Add(im_colOption, covOptional, im_colOption);
	m_eo->chart.put_Name("통계 그래프");
	
	m_eo->chart.put_ChartType(chartData.kindOfChart);

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
	VisibleWorkSheet(m_eo->ws.get_Index() - 1, false);
	SetActiveWorkSheet(0);
}

void ExcelAutomationManager::SetActiveWorkSheet(int nWorkSheetIndex)
{
	m_eo->ws = m_eo->wss.get_Item(COleVariant(long(nWorkSheetIndex + 1)));
	m_eo->ws.Activate();
}

void ExcelAutomationManager::SetStickChart()
{
	m_eo->chart.SetElement(2);	//상단에 차트제목표시		(title)
	m_eo->chart.SetElement(101);	//오른쪽에 범례표시			(legend)
	m_eo->chart.SetElement(201);	//중앙에 데이터레이블표시
}

void ExcelAutomationManager::SetPieChart()
{
	// https://docs.microsoft.com/ko-kr/office/vba/api/office.msochartelementtype 차트 엘레먼트값
	m_eo->chart.SetElement(2);	//상단에 차트제목표시		(title)
	m_eo->chart.SetElement(101);	//오른쪽에 범례표시			(legend)
	m_eo->chart.SetElement(201);	//중앙에 데이터레이블표시

	//첫번째 인자 데이터레이블유형 //https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.excel.xldatalabelstype?view=excel-pia
	//두번째 인자 범례사용
	//세번째 인자 텍스트노출
	//네번째 인자 계열출력
	//다섯번째 인자 데이터 헤더텍스트 출력(제목)
	//여섯번째 인자 데이터 바디텍스트 출력(값)
	//일곱번째 인자 데이터 퍼센테이지 출력
	//여덟번째 인자 출력텍스트 간 구분자설정
	m_eo->chart.ApplyDataLabels(5, im_colTrue, im_colTrue, im_colFalse, im_colFalse, im_colTrue, im_colFalse, im_colTrue, im_colFalse, COleVariant(_T("\n")));
}

void ExcelAutomationManager::SetLineChart()
{
	m_eo->chart.SetElement(2);
	m_eo->chart.SetElement(101);
}

void ExcelAutomationManager::SetChartTitle(CString chartTitle)
{
	if (m_eo->chart.get_HasTitle())
	{
		m_eo->title = m_eo->chart.get_ChartTitle();
		m_eo->title.put_Caption(chartTitle);
	}
}

void ExcelAutomationManager::SetLineChartColor(int whichChart, int legendSize, std::vector<int> legendColors)
{
	if (m_eo->chart.get_HasLegend())
	{
		m_eo->legend = m_eo->chart.get_Legend();
		m_eo->entry = m_eo->legend.LegendEntries(COleVariant((short)1));
		m_eo->key = m_eo->entry.get_LegendKey();
		m_eo->borders = m_eo->key.get_Border();
		m_eo->borders.put_Color(COleVariant((DOUBLE)COLOR_THICK_GREY));

		if (whichChart == CHART_POINTLINE)
		{
			m_eo->srs = m_eo->chart.SeriesCollection(COleVariant(long(1)));
			m_eo->interior = m_eo->range.get_Interior();
			for (int i = 1; i <= legendSize; i++)
			{
				m_eo->interior.put_ColorIndex(COleVariant(long(legendColors.at(i - 1))));
				m_eo->point = m_eo->srs.Points(COleVariant(long(i)));
				m_eo->point.put_MarkerStyle(8);
				m_eo->point.put_MarkerSize(8);
				for (int j = 1; j <= legendSize; j++)
				{
					if (i == j)
					{	
						m_eo->point.put_MarkerBackgroundColor(long(m_eo->interior.get_Color().dblVal));
						m_eo->point.put_MarkerForegroundColor(COLOR_WHITE);
						break;
					}
				}
			}
		}

		m_eo->chart.put_HasLegend(FALSE);
	}
}

void ExcelAutomationManager::SetPieChartColor(int legendSize, std::vector<int> legendColors)
{
	if (m_eo->chart.get_HasLegend())
	{
		for (int i = 1; i <= legendSize; i++)
		{
			m_eo->legend = m_eo->chart.get_Legend();
			m_eo->entry = m_eo->legend.LegendEntries(COleVariant((short)i));
			m_eo->key = m_eo->entry.get_LegendKey();
			m_eo->fform = m_eo->key.get_Fill();
			m_eo->cform = m_eo->fform.get_ForeColor();
			for (int j = 1; j <= legendSize; j++)
			{
				if (i == j)
				{
					m_eo->cform.put_SchemeColor(legendColors.at(i - 1));
					break;
				}
			}
		}
	}
}

void ExcelAutomationManager::SetStickChartColor(int legendSize, std::vector<int> legendColors)
{
	if (m_eo->chart.get_HasLegend())
	{
		m_eo->srs = m_eo->chart.SeriesCollection(COleVariant(long(1)));
		for (int i = 1; i <= legendSize; i++)
		{
			m_eo->point = m_eo->srs.Points(COleVariant(long(i)));
			m_eo->fform = m_eo->point.get_Fill();
			m_eo->cform = m_eo->fform.get_ForeColor();
			for (int j = 1; j <= legendSize; j++)
			{
				if (i == j)
				{
					m_eo->cform.put_SchemeColor(legendColors.at(i - 1));
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