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
		WSA_WD_PROMPT_TO_SAVE_CHANGES = -2,	// 변경사항을 저장하지않음
		WSA_WD_SAVE_CHANGES = -1,			// 사용자에게 보류중인 변경사항을 저장하라는 메시지 표시
		WSA_WD_DO_NOT_SAVE_CHANGES = 0,		// 사용자에게 묻지않고 보류중인 변경사항을 자동으로 저장
	};

	enum WordOriginalFormat
	{
		WOF_WD_WORD_DOCUMENT = 0,				// 원본 문서 형식
		WOF_WD_ORIGINAL_DOCUMENT_FORMAT = 1,	// 사용자에게 문서 형식을 선택하라는 메시지를 표시
		WOF_WD_PROMPT_USER = 2,					// 마이크로소프트 워드 문서 형식
	};

	// 워드의 기능들의 객체 집합 구조체
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

