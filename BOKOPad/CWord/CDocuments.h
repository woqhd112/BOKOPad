// 컴퓨터에서 형식 라이브러리 마법사의 [클래스 추가]를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

//#import "C:\\Program Files (x86)\\Microsoft Office\\Office15\\MSWORD.OLB" no_namespace
// CDocuments 래퍼 클래스

class CDocuments : public COleDispatchDriver
{
public:
	CDocuments() {} // COleDispatchDriver 기본 생성자를 호출합니다.
	CDocuments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDocuments(const CDocuments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// Documents 메서드
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, nullptr);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH Item(VARIANT * Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void Close(VARIANT * SaveChanges, VARIANT * OriginalFormat, VARIANT * RouteDocument)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0x451, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, SaveChanges, OriginalFormat, RouteDocument);
	}
	LPDISPATCH AddOld(VARIANT * Template, VARIANT * NewTemplate)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Template, NewTemplate);
		return result;
	}
	LPDISPATCH OpenOld(VARIANT * FileName, VARIANT * ConfirmConversions, VARIANT * ReadOnly, VARIANT * AddToRecentFiles, VARIANT * PasswordDocument, VARIANT * PasswordTemplate, VARIANT * Revert, VARIANT * WritePasswordDocument, VARIANT * WritePasswordTemplate, VARIANT * Format)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, FileName, ConfirmConversions, ReadOnly, AddToRecentFiles, PasswordDocument, PasswordTemplate, Revert, WritePasswordDocument, WritePasswordTemplate, Format);
		return result;
	}
	void Save(VARIANT * NoPrompt, VARIANT * OriginalFormat)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, NoPrompt, OriginalFormat);
	}
	LPDISPATCH Add(VARIANT * Template, VARIANT * NewTemplate, VARIANT * DocumentType, VARIANT * Visible)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Template, NewTemplate, DocumentType, Visible);
		return result;
	}
	LPDISPATCH Open2000(VARIANT * FileName, VARIANT * ConfirmConversions, VARIANT * ReadOnly, VARIANT * AddToRecentFiles, VARIANT * PasswordDocument, VARIANT * PasswordTemplate, VARIANT * Revert, VARIANT * WritePasswordDocument, VARIANT * WritePasswordTemplate, VARIANT * Format, VARIANT * Encoding, VARIANT * Visible)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, FileName, ConfirmConversions, ReadOnly, AddToRecentFiles, PasswordDocument, PasswordTemplate, Revert, WritePasswordDocument, WritePasswordTemplate, Format, Encoding, Visible);
		return result;
	}
	void CheckOut(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, FileName);
	}
	BOOL CanCheckOut(LPCTSTR FileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, FileName);
		return result;
	}
	LPDISPATCH Open2002(VARIANT * FileName, VARIANT * ConfirmConversions, VARIANT * ReadOnly, VARIANT * AddToRecentFiles, VARIANT * PasswordDocument, VARIANT * PasswordTemplate, VARIANT * Revert, VARIANT * WritePasswordDocument, VARIANT * WritePasswordTemplate, VARIANT * Format, VARIANT * Encoding, VARIANT * Visible, VARIANT * OpenAndRepair, VARIANT * DocumentDirection, VARIANT * NoEncodingDialog)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, FileName, ConfirmConversions, ReadOnly, AddToRecentFiles, PasswordDocument, PasswordTemplate, Revert, WritePasswordDocument, WritePasswordTemplate, Format, Encoding, Visible, OpenAndRepair, DocumentDirection, NoEncodingDialog);
		return result;
	}
	LPDISPATCH Open(VARIANT * FileName, VARIANT * ConfirmConversions, VARIANT * ReadOnly, VARIANT * AddToRecentFiles, VARIANT * PasswordDocument, VARIANT * PasswordTemplate, VARIANT * Revert, VARIANT * WritePasswordDocument, VARIANT * WritePasswordTemplate, VARIANT * Format, VARIANT * Encoding, VARIANT * Visible, VARIANT * OpenAndRepair, VARIANT * DocumentDirection, VARIANT * NoEncodingDialog, VARIANT * XMLTransform)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, FileName, ConfirmConversions, ReadOnly, AddToRecentFiles, PasswordDocument, PasswordTemplate, Revert, WritePasswordDocument, WritePasswordTemplate, Format, Encoding, Visible, OpenAndRepair, DocumentDirection, NoEncodingDialog, XMLTransform);
		return result;
	}
	LPDISPATCH OpenNoRepairDialog(VARIANT * FileName, VARIANT * ConfirmConversions, VARIANT * ReadOnly, VARIANT * AddToRecentFiles, VARIANT * PasswordDocument, VARIANT * PasswordTemplate, VARIANT * Revert, VARIANT * WritePasswordDocument, VARIANT * WritePasswordTemplate, VARIANT * Format, VARIANT * Encoding, VARIANT * Visible, VARIANT * OpenAndRepair, VARIANT * DocumentDirection, VARIANT * NoEncodingDialog, VARIANT * XMLTransform)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, FileName, ConfirmConversions, ReadOnly, AddToRecentFiles, PasswordDocument, PasswordTemplate, Revert, WritePasswordDocument, WritePasswordTemplate, Format, Encoding, Visible, OpenAndRepair, DocumentDirection, NoEncodingDialog, XMLTransform);
		return result;
	}
	LPDISPATCH AddBlogDocument(LPCTSTR ProviderID, LPCTSTR PostURL, LPCTSTR BlogName, LPCTSTR PostID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, ProviderID, PostURL, BlogName, PostID);
		return result;
	}

	// Documents 속성
public:

};
