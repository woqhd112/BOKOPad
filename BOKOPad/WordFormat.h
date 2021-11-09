#pragma once
#include "CWord/CApplication0.h"
#include "CWord/CDocument0.h"
#include "CWord/CDocuments.h"
#include "CWord/CFont1.h"
#include "CWord/CRange0.h"
#include "CWord/CSection.h"
#include "CWord/CSelection.h"
#include "CWord/CShape.h"
#include "CWord/CShapes.h"
#include "CWord/CTable0.h"
#include "CWord/CTables0.h"
#include "OfficeAutomationInterface.h"

//https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.word._document?view=word-pia ms word api (.net ver)

class WordFormat : public OAInterface
{
public:

	WordFormat();
	virtual ~WordFormat();

	bool StartWord(CString WorkSheetName);

	void CloseWord(CString strFilePath);

	void SaveDocument(CString strFilePath);

	void AppendText(CString strText);

	void SetFontBold(bool FontBold);

	void SetFontSize(int nFontSize);

	void SetFontStyle(CString strFontStyle);

	void SetFontColor(Color color);

	void AddTable(CPoint tableCoordinate);

	void AppendTableText(CPoint textCoordinate, CString strText);

private:

	enum WordSaveAs
	{
		WSA_WD_PROMPT_TO_SAVE_CHANGES = -2,	// ��������� ������������
		WSA_WD_SAVE_CHANGES = -1,			// ����ڿ��� �������� ��������� �����϶�� �޽��� ǥ��
		WSA_WD_DO_NOT_SAVE_CHANGES = 0,		// ����ڿ��� �����ʰ� �������� ��������� �ڵ����� ����
	};

	enum WordOriginalFormat
	{
		WOF_WD_WORD_DOCUMENT = 0,				// ���� ���� ����
		WOF_WD_ORIGINAL_DOCUMENT_FORMAT = 1,	// ����ڿ��� ���� ������ �����϶�� �޽����� ǥ��
		WOF_WD_PROMPT_USER = 2,					// ����ũ�μ���Ʈ ���� ���� ����
	};

	// ������ ��ɵ��� ��ü ���� ����ü
	struct WordObject
	{
		CApplication0		_app;
		CDocument0			_doc;
		CDocuments			_docs;
		CFont1				_font;
		CRange0				_range;
		CSection			_section;
		CSelection			_selection;
		CShape				_shape;
		CShapes				_shapes;
		CTable0				_table;
		CTables0			_tables;
	};

	WordObject* m_wo;

	int _nEndTextPos;

	bool _bAddTable;

};

