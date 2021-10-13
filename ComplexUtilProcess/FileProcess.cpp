#include "pch.h"
#include "FileProcess.h"
#include "ComplexStringTokenizer.h"

namespace ComplexUtilProcess
{
	COMPLEXUTILPROCESS_DLL void ExportFile(ComplexString writeContent, ComplexString writePath)
	{
		ComplexString fileContent;
		fileContent = writeContent;
		int bufSize = writeContent.GetLength();

		fileContent.ReplaceAll("\r\n", "+");
		ComplexStringTokenizer tokens;
		tokens.ApplyStringTokenize(fileContent, '+', false);

		if (tokens.HasNextToken() == true)
		{
			fileContent = tokens.NextToken();
			bufSize = fileContent.GetLength();
		}

		const char* fileContentBuf = fileContent.GetBuffer();
		
		int fixTitleSize = 10;
		if (bufSize < 10)
			fixTitleSize = bufSize;

		char* titleBuf = new char[fixTitleSize + 1];	// include '\0'

		for (int i = 0; i < fixTitleSize; i++)
		{
			titleBuf[i] = fileContentBuf[i];
		}
		titleBuf[fixTitleSize] = '\0';

		ComplexString writeTitle = titleBuf;
		delete[] titleBuf;
		titleBuf = nullptr;

		ComplexFile file;
		ComplexString strFullPath;
		char last = writePath.GetAt(writePath.GetLength() - 1);
		if(last == '\\' || last == '/')
			strFullPath.Format("%s%s.txt", writePath.GetBuffer(), writeTitle.GetBuffer());
		else
			strFullPath.Format("%s/%s.txt", writePath.GetBuffer(), writeTitle.GetBuffer());

		file.Write(strFullPath, writeContent);
	}

	COMPLEXUTILPROCESS_DLL void ImportFile(ComplexString& readContent, ComplexString readPath)
	{
		ComplexFile file;
		file.Read(readPath, readContent);
		readContent.ReplaceAll("\n", "+");
		readContent.ReplaceAll("+", "\r\n");
	}

	COMPLEXUTILPROCESS_DLL void UTF8ToANSI(ComplexString& out_ansi, ComplexString in_utf8)
	{
		const char* pszCode = in_utf8.GetBuffer();

		BSTR bstrWide;
		char* pszAnsi;
		int length;

		length = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlenA(pszCode) + 1, NULL, NULL);
		bstrWide = SysAllocStringLen(NULL, length);

		MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlenA(pszCode) + 1, bstrWide, length);

		length = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
		pszAnsi = new char[length + 1];

		WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, length, NULL, NULL);
		SysFreeString(bstrWide);

		out_ansi = pszAnsi;
		delete[] pszAnsi;
	}

	COMPLEXUTILPROCESS_DLL void ANSIToUTF8(ComplexString& out_utf8, ComplexString in_ansi)
	{
		const char* pszCode = in_ansi.GetBuffer();

		BSTR bstrCode;
		char* pszUTFCode = NULL;
		int length, length2;

		length = MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlenA(pszCode), NULL, NULL);
		bstrCode = SysAllocStringLen(NULL, length);
		MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlenA(pszCode), bstrCode, length);

		length2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);
		pszUTFCode = new char[length2 + 1];

		WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, length2, NULL, NULL);
		SysFreeString(bstrCode);
		
		out_utf8 = pszUTFCode;
		delete[] pszUTFCode;
	}
}