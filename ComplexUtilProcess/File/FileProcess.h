#pragma once
#include <oleauto.h>
#include "../commonInclude.h"

#ifdef COMPLEXUTILPROCESS_EXPORTS
#define COMPLEXUTILPROCESS_DLL _declspec(dllexport)
#else
#define COMPLEXUTILPROCESS_DLL _declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	namespace ComplexUtilProcess
	{
		COMPLEXUTILPROCESS_DLL void ExportFile(ComplexString writeContent, ComplexString writePath);
		COMPLEXUTILPROCESS_DLL void ImportFile(ComplexString& readContent, ComplexString readPath);
		COMPLEXUTILPROCESS_DLL void UTF8ToANSI(ComplexString& out_ansi, ComplexString in_utf8);
		COMPLEXUTILPROCESS_DLL void ANSIToUTF8(ComplexString& out_utf8, ComplexString in_ansi);
	}
	
#ifdef __cplusplus
}
#endif

