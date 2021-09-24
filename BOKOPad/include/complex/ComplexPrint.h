#pragma once

#include "ComplexString.h"

namespace ComplexLibrary
{
	class ComplexPrint
	{
		friend class ComplexLog;
	public:

		ComplexPrint()
		{
	
		}

		~ComplexPrint()
		{
	
		}

		void Print(ComplexString buf)
		{
			Printf("%s", buf.GetBuffer());
		}

		void Printf(const char* format, ...)
		{
			va_list args;
			__crt_va_start(args, format);
			ComplexPrintf(format, args);
			__crt_va_end(args);
		}

		void Println(ComplexString buf)
		{ 
			Printf("%s\n", buf.GetBuffer());
		}

	private:

		void ComplexPrintf(const char* format, va_list args)
		{
			ComplexFormat formatting;
			ComplexString test = formatting.GetFormatString(format, args);
			PrintString(test.GetBuffer());
		}

		void PrintString(const char* string)
		{
			while (*string)
			{
				putc(*string, stdout);
				string++;
			}
		}
	};
}