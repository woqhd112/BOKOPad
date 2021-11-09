#include "pch.h"
#include "WordFormat.h"

WordFormat::WordFormat()
{
	m_wo = new WordObject;
	_bAttach = false;
	_bAddTable = false;
	_nEndTextPos = 0;
	_type = OAT_WORD;
}

WordFormat::~WordFormat()
{
	if (m_wo)
	{
		delete m_wo;
		m_wo = (WordObject*)nullptr;
	}
}

bool WordFormat::StartWord(CString WorkSheetName)
{
	if (!m_wo->_app.CreateDispatch("Word.Application"))
	{
		return false;
	}
	m_wo->_app.put_Visible(false);

	StartProcess();

	m_wo->_docs.AttachDispatch(m_wo->_app.get_Documents());
	m_wo->_doc.AttachDispatch(m_wo->_docs.Add(_colOption, _colOption, _colOption, _colOption));
	m_wo->_selection = m_wo->_app.get_Selection();

	return true;
}

void WordFormat::CloseWord(CString strFilePath)
{
	if (_bAttach)
	{
		SaveDocument(strFilePath);

		m_wo->_table.DetachDispatch();
		m_wo->_tables.DetachDispatch();
		m_wo->_shape.DetachDispatch();
		m_wo->_shapes.DetachDispatch();
		m_wo->_section.DetachDispatch();
		m_wo->_font.DetachDispatch();
		m_wo->_range.DetachDispatch();
		m_wo->_selection.DetachDispatch();
		m_wo->_doc.DetachDispatch();
		m_wo->_docs.DetachDispatch();
		m_wo->_app.DetachDispatch();

		_bAttach = false;
	}

	CloseProcess(strFilePath);
}

void WordFormat::SaveDocument(CString strFilePath)
{
	COleVariant change((short)WSA_WD_DO_NOT_SAVE_CHANGES, VT_I2);
	COleVariant format((short)WOF_WD_WORD_DOCUMENT, VT_I2);
	COleVariant fileFormat((long)16);
	COleVariant filePath(strFilePath);
	COleVariant reject((short)false, VT_BOOL);
	COleVariant test("");
	COleVariant mode((long)65535);

	m_wo->_app.put_DisplayAlerts(FALSE);
	m_wo->_doc.SaveAsQuickStyleSet(strFilePath);
	m_wo->_doc.SaveAs2(filePath, fileFormat, reject, test, reject, test, reject, reject, reject, reject, reject, reject, reject, reject, reject, reject, mode);
	m_wo->_app.Quit(change, format, reject);
}

void WordFormat::AppendText(CString strText)
{
	if (_bAttach && !_bAddTable)
	{
		_nEndTextPos = m_wo->_selection.get_End();
		m_wo->_selection.put_Start(_nEndTextPos + 1);
		m_wo->_selection.put_Text(strText);
	}
}

void WordFormat::SetFontBold(bool FontBold)
{
	if (_bAttach)
	{
		m_wo->_font = m_wo->_selection.get_Font();
		m_wo->_font.put_Bold(FontBold);
	}
}

void WordFormat::SetFontSize(int nFontSize)
{
	if (_bAttach)
	{
		m_wo->_font = m_wo->_selection.get_Font();
		m_wo->_font.put_Size(nFontSize);
	}
}

void WordFormat::SetFontStyle(CString strFontStyle)
{
	if (_bAttach)
	{
		m_wo->_font = m_wo->_selection.get_Font();
		m_wo->_font.put_Name(strFontStyle);
	}
}

void WordFormat::SetFontColor(Color color)
{
	if (_bAttach)
	{
		m_wo->_font = m_wo->_selection.get_Font();
		m_wo->_font.put_Color((long)color);
	}
}

void WordFormat::AddTable(CPoint tableCoordinate)
{
	if (_bAttach)
	{
		_nEndTextPos = m_wo->_selection.get_End();
		m_wo->_selection.put_Start(_nEndTextPos + 1);
		m_wo->_range = m_wo->_selection.get_Range();

		m_wo->_tables = m_wo->_doc.get_Tables();
		m_wo->_table = m_wo->_tables.AddOld(m_wo->_range, tableCoordinate.x, tableCoordinate.y);

		COleVariant sCmt("표 구분선");

		m_wo->_table.put_Style(&sCmt.Detach());

		m_wo->_table.put_ApplyStyleHeadingRows(TRUE);
		m_wo->_table.put_ApplyStyleLastRow(FALSE);
		m_wo->_table.put_ApplyStyleFirstColumn(TRUE);
		m_wo->_table.put_ApplyStyleLastColumn(FALSE);
		m_wo->_table.put_ApplyStyleRowBands(TRUE);
		m_wo->_table.put_ApplyStyleColumnBands(FALSE);

		_bAddTable = true;
	}
}

void WordFormat::AppendTableText(CPoint textCoordinate, CString strText)
{
	// x, y 좌표는 1 부터 시작하고 range에 설정한 범위만큼
	if (textCoordinate.x <= 0 || textCoordinate.y <= 0)
		return;

	if (_bAttach && _bAddTable)
	{
		_nEndTextPos = m_wo->_selection.get_End();
		m_wo->_selection.put_Start(_nEndTextPos + textCoordinate.x);
		m_wo->_selection.put_Text(strText);
	}
}