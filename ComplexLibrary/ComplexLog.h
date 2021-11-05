#pragma once

#include "ComplexFile.h"
#include "ComplexPrint.h"

namespace ComplexLibrary
{
	class ComplexLog;

	class ComplexFileLog : public ComplexFile
	{
		friend class ComplexLog;
	public:

		ComplexFileLog()
		{

		}

		~ComplexFileLog()
		{

		}

	private:

		void ComplexWrite(ComplexString filePath, const char* format, va_list args)
		{
			ComplexFormat formatting;
			ComplexString buf = formatting.GetFormatString(format, args);
			if (Write(filePath, buf, FM_WR_CREATE_AND_EOF_WRITE) == false)
				return;
		}

	};

	class ComplexLog
	{
	public:

		ComplexLog()
			: m_printLog(new ComplexPrint)
			, m_fileLog(new ComplexFileLog)
		{

		}

		~ComplexLog()
		{
			if (m_printLog)
			{
				delete m_printLog;
				m_printLog = nullptr;
			}

			if (m_fileLog)
			{
				delete m_fileLog;
				m_fileLog = nullptr;
			}
		}

		void FileLog(ComplexString filePath, const char* format, ...)
		{
			va_list args;
			__crt_va_start(args, format);
			m_fileLog->ComplexWrite(filePath, format, args);
			__crt_va_end(args);
		}

		void PrintLog(const char* format, ...)
		{
			va_list args;
			__crt_va_start(args, format);
			m_printLog->ComplexPrintf(format, args);
			__crt_va_end(args);
		}

	private:

		ComplexPrint* m_printLog;
		ComplexFileLog* m_fileLog;

	};
}