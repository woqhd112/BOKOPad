// 컴퓨터에서 형식 라이브러리 마법사의 [클래스 추가]를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

//#import "C:\\Program Files (x86)\\Microsoft Office\\Office15\\MSWORD.OLB" no_namespace
// CTables0 래퍼 클래스

class CTables0 : public COleDispatchDriver
{
public:
	CTables0() {} // COleDispatchDriver 기본 생성자를 호출합니다.
	CTables0(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTables0(const CTables0& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// Tables 메서드
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
	LPDISPATCH Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH AddOld(LPDISPATCH Range, long NumRows, long NumColumns)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_I4;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Range, NumRows, NumColumns);
		return result;
	}
	LPDISPATCH Add(LPDISPATCH Range, long NumRows, long NumColumns, VARIANT * DefaultTableBehavior, VARIANT * AutoFitBehavior)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 VTS_I4 VTS_PVARIANT VTS_PVARIANT;
		InvokeHelper(0xc8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Range, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior);
		return result;
	}
	long get_NestingLevel()
	{
		long result;
		InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}

	// Tables 속성
public:

};
