#include "pch.h"
#include "WordAutomationManager.h"

WordAutomationManager::WordAutomationManager()
{
	m_wo = new WordObject;
	im_bAttach = false;
	m_bAddTable = false;
	m_nEndTextPos = 0;
	im_type = OAT_WORD;
}

WordAutomationManager::~WordAutomationManager()
{
	if (m_wo)
	{
		delete m_wo;
		m_wo = (WordObject*)nullptr;
	}
}

bool WordAutomationManager::StartWord(CString WorkSheetName)
{
	if (!m_wo->app.CreateDispatch("Word.Application"))
	{
		return false;
	}
	m_wo->app.put_Visible(false);

	StartProcess();

	m_wo->docs.AttachDispatch(m_wo->app.get_Documents());
	m_wo->doc.AttachDispatch(m_wo->docs.Add(im_colOption, im_colOption, im_colOption, im_colOption));
	m_wo->selection = m_wo->app.get_Selection();
	m_wo->shapes = m_wo->doc.get_Shapes();

	return true;
}

void WordAutomationManager::CloseWord(CString strFilePath, bool executeShell)
{
	if (im_bAttach)
	{
		SaveDocument(strFilePath);

		m_wo->cell.DetachDispatch();
		m_wo->cells.DetachDispatch();
		m_wo->table.DetachDispatch();
		m_wo->tables.DetachDispatch();
		m_wo->shape.DetachDispatch();
		m_wo->shapes.DetachDispatch();
		m_wo->section.DetachDispatch();
		m_wo->font.DetachDispatch();
		m_wo->range.DetachDispatch();
		m_wo->selection.DetachDispatch();
		m_wo->doc.DetachDispatch();
		m_wo->docs.DetachDispatch();
		m_wo->app.DetachDispatch();

		im_bAttach = false;
	}

	CloseProcess(strFilePath, executeShell);
}

void WordAutomationManager::SaveDocument(CString strFilePath)
{
	COleVariant change((short)WSA_WD_DO_NOT_SAVE_CHANGES, VT_I2);
	COleVariant format((short)WOF_WD_WORD_DOCUMENT, VT_I2);
	COleVariant fileFormat((long)16);
	COleVariant filePath(strFilePath);
	COleVariant reject((short)false, VT_BOOL);
	COleVariant test("");
	COleVariant mode((long)65535);

	m_wo->app.put_DisplayAlerts(FALSE);
	m_wo->doc.SaveAsQuickStyleSet(strFilePath);
	m_wo->doc.SaveAs2(filePath, fileFormat, reject, test, reject, test, reject, reject, reject, reject, reject, reject, reject, reject, reject, reject, mode);
	m_wo->app.Quit(change, format, reject);
}

void WordAutomationManager::AppendText(CString strText)
{
	if (im_bAttach && !m_bAddTable)
	{
		LineEnd();
		m_wo->selection.put_Text(strText);
	}
}

void WordAutomationManager::AppendImage(CString strFilePath, CRect imageRect)
{
	if (im_bAttach)
	{
		LineEnd();

		VARIANT left, top, width, height;
		VariantInit(&left);
		VariantInit(&top);
		VariantInit(&width);
		VariantInit(&height);

		V_VT(&left) = VT_I4;
		V_VT(&top) = VT_I4;
		V_VT(&width) = VT_I4;
		V_VT(&height) = VT_I4;

		V_I4(&left) = imageRect.left;
		V_I4(&top) = imageRect.top;
		V_I4(&width) = imageRect.Width();
		V_I4(&height) = imageRect.Height();

		m_wo->shape = m_wo->shapes.AddPicture(strFilePath, im_colTrue, im_colTrue, &left, &top, &width, &height, im_colOption);
	}
}

void WordAutomationManager::SetFontBold(bool FontBold)
{
	if (im_bAttach)
	{
		m_wo->font = m_wo->selection.get_Font();
		m_wo->font.put_Bold(FontBold);
	}
}

void WordAutomationManager::SetFontSize(int nFontSize)
{
	if (im_bAttach)
	{
		m_wo->font = m_wo->selection.get_Font();
		m_wo->font.put_Size((float)nFontSize);
	}
}

void WordAutomationManager::SetFontStyle(CString strFontStyle)
{
	if (im_bAttach)
	{
		m_wo->font = m_wo->selection.get_Font();
		m_wo->font.put_Name(strFontStyle);
	}
}

void WordAutomationManager::SetFontColor(OAColor color)
{
	if (im_bAttach)
	{
		m_wo->font = m_wo->selection.get_Font();
		m_wo->font.put_Color((long)color);
	}
}

void WordAutomationManager::AddTable(CPoint tableRange)
{
	if (im_bAttach)
	{
		// x, y 범위는 1 부터 시작
		if (tableRange.x <= 0 || tableRange.y <= 0)
			return;

		LineEnd();
		m_wo->range = m_wo->selection.get_Range();
		m_tableRange = tableRange;
		m_wo->tables = m_wo->doc.get_Tables();
		m_wo->table = m_wo->tables.AddOld(m_wo->range, tableRange.y, tableRange.x);

		COleVariant sCmt("표 구분선");

		m_wo->table.put_Style(&sCmt.Detach());

		m_wo->table.put_ApplyStyleHeadingRows(TRUE);
		m_wo->table.put_ApplyStyleLastRow(FALSE);
		m_wo->table.put_ApplyStyleFirstColumn(TRUE);
		m_wo->table.put_ApplyStyleLastColumn(FALSE);
		m_wo->table.put_ApplyStyleRowBands(TRUE);
		m_wo->table.put_ApplyStyleColumnBands(FALSE);

		m_bAddTable = true;
	}
}

void WordAutomationManager::LineEnd()
{
	m_nEndTextPos = m_wo->selection.get_End();
	m_nEndTextPos += 1;
	m_wo->selection.put_Start(m_nEndTextPos);
}

void WordAutomationManager::AppendTableText(CPoint textCoordinate, CString strText)
{
	if (im_bAttach && m_bAddTable)
	{
		// 작업은 꼭 앞에서 뒤로 작업해야함 뒤에서 다시 앞으로 돌아오는 경우는 없음

		// x, y 좌표는 1 부터 시작
		if (textCoordinate.x <= 0 || textCoordinate.y <= 0)
			return;

		// x, y 좌표가 설정한 테이블 범위보다 클경우 
		if (m_tableRange.x < textCoordinate.x || m_tableRange.y < textCoordinate.y)
			return;

		int test = 0;
		if (textCoordinate.x == 1 && textCoordinate.y == 1)
		{
			m_nEndTextPos = m_wo->selection.get_End();
			test = m_nEndTextPos;

			COleVariant var = strText;
			m_nEndTextPos += (wcslen(var.bstrVal) - 1);
		}
		else
		{
			test = m_nEndTextPos + GetCoordinateToIndex(textCoordinate);

			COleVariant var = strText;
			m_nEndTextPos += wcslen(var.bstrVal);

			if (textCoordinate.x == m_tableRange.x)
				m_nEndTextPos++;
		}
		// wchar 처리여야해서 우선 colevariant로 캐스팅해서 bstr 뽑음
		m_wo->selection.put_Start(test);
		m_wo->selection.put_Text(strText);
	}
}

void WordAutomationManager::EndTable()
{
	if (im_bAttach && m_bAddTable)
	{
		m_nEndTextPos = m_wo->selection.get_End();
		m_nEndTextPos += 999;
		m_wo->selection.put_Start(m_nEndTextPos);
		m_bAddTable = false;
	}
}

void WordAutomationManager::Merge(CPoint startMergeCoordinate, CPoint endMergeCoordinate)
{
	if (im_bAttach && m_bAddTable)
	{
		if (startMergeCoordinate.x <= 0 || startMergeCoordinate.y <= 0)
			return;

		if (endMergeCoordinate.x <= 0 || endMergeCoordinate.y <= 0)
			return;

		if (startMergeCoordinate.x > m_tableRange.x || startMergeCoordinate.y > m_tableRange.y)
			return;

		if (endMergeCoordinate.x > m_tableRange.x || endMergeCoordinate.y > m_tableRange.y)
			return;

		if (startMergeCoordinate.x > endMergeCoordinate.x)
			return;

		if (startMergeCoordinate.y > endMergeCoordinate.y)
			return;

		m_wo->cell = m_wo->table.Cell(startMergeCoordinate.y, startMergeCoordinate.x);
		CCell objectCell = m_wo->table.Cell(endMergeCoordinate.y, endMergeCoordinate.x);

		m_wo->cell.Merge(objectCell);
	}
}

void WordAutomationManager::Enter()
{
	if (im_bAttach)
	{
		m_wo->selection.TypeParagraph();
	}
}

int WordAutomationManager::GetCoordinateToIndex(CPoint coordinate)
{
	if (im_bAttach && m_bAddTable)
	{
		return (coordinate.x + (coordinate.y - 1) * m_tableRange.x);
	}

	return 0;
}