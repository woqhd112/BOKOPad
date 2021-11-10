#pragma once
#include "..\CWord/CApplication0.h"
#include "..\CWord/CDocument0.h"
#include "..\CWord/CDocuments.h"
#include "..\CWord/CFont1.h"
#include "..\CWord/CRange0.h"
#include "..\CWord/CSection.h"
#include "..\CWord/CSelection.h"
#include "..\CWord/CShape.h"
#include "..\CWord/CShapes.h"
#include "..\CWord/CTable0.h"
#include "..\CWord/CTables0.h"
#include "..\CWord/CCells.h"
#include "..\CWord/CCell.h"
#include "OfficeAutomationInterface.h"
#include <map>

//https://docs.microsoft.com/en-us/dotnet/api/microsoft.office.interop.word._document?view=word-pia ms word api (.net ver)

class WordAutomationManager : public OAInterface
{
public:

	WordAutomationManager();
	virtual ~WordAutomationManager();

	bool StartWord(CString WorkSheetName);

	void CloseWord(CString strFilePath, bool executeShell = true);

	void SaveDocument(CString strFilePath);

	void AppendText(CString strText);

	void AppendImage(CString strFilePath, CRect imageRect);

	void SetFontBold(bool FontBold);

	void SetFontSize(int nFontSize);

	void SetFontStyle(CString strFontStyle);

	void SetFontColor(OAColor color);

	void AddTable(CPoint tableRange);

	void AppendTableText(CPoint textCoordinate, CString strText);

	void Merge(CPoint startMergeCoordinate, CPoint endMergeCoordinate);

	void LineEnd();

	void EndTable();

	void Enter();

private:

	int GetCoordinateToIndex(CPoint coordinate);

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

	struct TableCellDataStruct
	{
		bool bMergeStart;
		bool bMergeEnd;
		bool bMerge;
		CString strText;
		CPoint cellCoordinate;
	};

	// ������ ��ɵ��� ��ü ���� ����ü
	struct WordObject
	{
		CApplication0		app;
		CDocument0			doc;
		CDocuments			docs;
		CFont1				font;
		CRange0				range;
		CSection			section;
		CSelection			selection;
		CShape				shape;
		CShapes				shapes;
		CTable0				table;
		CTables0			tables;
		CCells				cells;
		CCell				cell;
	};

	WordObject* m_wo;

	int m_nEndTextPos;

	bool m_bAddTable;
	CPoint m_tableRange;

};

