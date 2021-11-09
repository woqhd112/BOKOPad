#pragma once
#include <afx.h> 
#include <afxwin.h>  
#include <afxext.h>  
#include <afxdlgs.h>
#include <afxdisp.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

enum Color
{
	COLOR_THICK_GREY = RGB(128, 128, 128),
	COLOR_LESS_THICK_GREY = RGB(166, 166, 166),
	COLOR_GREY = RGB(191, 191, 191),
	COLOR_LESS_LIGHT_GREY = RGB(217, 217, 217),
	COLOR_LIGHT_GREY = RGB(242, 242, 242),
	COLOR_WHITE = RGB(255, 255, 255),
	COLOR_BLACK = RGB(0, 0, 0),
	COLOR_RED = RGB(255, 0, 0),
	COLOR_PASTEL_ORANGE = RGB(237, 125, 49),
	COLOR_PASTEL_LESS_ORANGE = RGB(244, 176, 132),
	COLOR_PASTEL_LIGHT_ORANGE = RGB(252, 228, 214),
	COLOR_PASTEL_GREEN = RGB(146, 208, 80),
	COLOR_PASTEL_LESS_GREEN = RGB(169, 208, 142),
	COLOR_PASTEL_LIGHT_GREEN = RGB(226, 239, 218),
	COLOR_PASTEL_BLUE = RGB(47, 117, 181),
	COLOR_PASTEL_LESS_BLUE = RGB(189, 215, 238),
	COLOR_PASTEL_LIGHT_BLUE = RGB(221, 235, 247),
	COLOR_PASTEL_THICK_YELLOW = RGB(255, 192, 0),
	COLOR_PASTEL_YELLOW = RGB(255, 217, 102),
	COLOR_PASTEL_LESS_YELLOW = RGB(255, 230, 153),
	COLOR_PASTEL_LIGHT_YELLOW = RGB(255, 242, 204),
	COLOR_PASTEL_TURQUOISE = RGB(32, 178, 198),
	COLOR_PASTEL_LESS_TURQUOISE = RGB(147, 219, 229),
	COLOR_PASTEL_LIGHT_TURQUOISE = RGB(221, 244, 247)
};

class OAInterface
{
public:

	OAInterface() {}
	~OAInterface() {}

	bool OnReadyProcess(CString* strFileName, CString* strFilePath)
	{
		if (_type == OAT_WORD)
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
		}
		else if (_type == OAT_EXCEL)
		{
			CFileDialog fileDlg(FALSE, "xlsx", "*.xlsx", OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "REPORT file (*.xlsx) | *.xlsx |");

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
		}

		return true;
	}


private:

protected:

	void StartProcess()
	{
		_colTrue = (short)TRUE;
		_colFalse = (short)FALSE;
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		memcpy(_colOption, covOptional, sizeof(covOptional));
		_bAttach = true;
	}

	void CloseProcess(CString strFilePath)
	{
		if (!_bAttach)
		{
			::ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_SHOW);
		}
		else
		{
			AfxMessageBox("엑셀 저장에 실패하였습니다.");
		}
	}

	enum OAType
	{
		OAT_WORD = 0,
		OAT_EXCEL = 1,
	};

	COleVariant _colTrue;

	COleVariant _colFalse;

	COleVariant _colOption;

	OAType _type;

	bool _bAttach;

};
