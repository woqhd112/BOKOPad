#include "pch.h"
#include "WordFormat.h"

WordFormat::WordFormat()
{
	m_wo = new WordObject;
	_bAttach = false;
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

bool WordFormat::OnReadyWord(CString* strFileName, CString* strFilePath)
{
	CFileDialog fileDlg(FALSE, "docx", "*.docx", OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "REPORT file (*.docx) | *.docx |");

	if (IDOK != fileDlg.DoModal())
	{
		return false;
	}

	CString strProcedure = _T("");
	*strFileName = fileDlg.GetFileName();
	*strFilePath = fileDlg.GetPathName();
	CFile cFile;
	if (cFile.Open(*strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, NULL))
	{
		USHORT nShort = 0xfeff;
		int nSize = -1;
		nSize = sizeof(nShort);
		cFile.Write(&nShort, nSize);
		cFile.Write((LPTSTR)(LPCTSTR)strProcedure, strProcedure.GetLength() * sizeof(TCHAR));
		cFile.Close();
	}
	else
	{
		AfxMessageBox("저장에 실패했습니다.");
		return false;
	}

	return true;
}

bool WordFormat::StartWord(CString WorkSheetName)
{
	if (!m_wo->_app.CreateDispatch("Word.Application"))
	{
		return false;
	}
	m_wo->_app.put_Visible(false);

	_colTrue = (short)TRUE;
	_colFalse = (short)FALSE;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	memcpy(_colOption, covOptional, sizeof(covOptional));

	m_wo->_docs.AttachDispatch(m_wo->_app.get_Documents());
	m_wo->_doc.AttachDispatch(m_wo->_docs.Add(covOptional, covOptional, covOptional, covOptional));
	m_wo->_selection = m_wo->_app.get_Selection();

	_bAttach = true;

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

	if (!_bAttach)
	{
		::ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox("워드 저장에 실패하였습니다.");
	}
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
	if (_bAttach)
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