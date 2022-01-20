#pragma once
#include <iostream>

namespace ComplexLibrary
{
	class ComplexException
	{
	public:

		enum ExceptionType
		{
			ET_NULL_POINT = 1,
			ET_INDEX_OUT_OF_BOUNDS,
			ET_NUMBER_FORMAT,
			ET_CAPACITY_OVERFLOW,
			ET_DUPLICATE,
			ET_XML_PARSE,
			ET_NOT_FOUND,
			ET_UNIDENTIFIABLE,
		};

	protected:

		ExceptionType m_exceptionType;
		const char* m_exceptTitle;
		const char* m_exceptionMessage;
		const char* m_exceptionGenerationClaseName;
		const char* m_exceptionGenerationFuncName;
		int m_exceptionGenarationLineNo;

	public:

		ComplexException(ExceptionType et)
		{
			m_exceptionType = et;
		}

		~ComplexException()
		{

		}

		const char* GetExceptionTitle() const
		{
			return m_exceptTitle;
		}

		const char* GetExceptionMessage() const
		{
			return m_exceptionMessage;
		}

		ExceptionType GetExceptionType() const
		{
			return m_exceptionType;
		}

		void MessagePrintStackTrace()
		{
			printf("----------------------------------------------------------------------\n");
			printf("Exception Type : \n%s\n", m_exceptTitle);
			printf("Exception Message : \n%s\n", m_exceptionMessage);
			printf("Exception Generate Class Name : \n%s\n", m_exceptionGenerationClaseName);
			printf("Exception Generate Function Name : \n%s\n", m_exceptionGenerationFuncName);
			printf("Exception Generate Line Number : \n%d\n", m_exceptionGenarationLineNo);
			printf("----------------------------------------------------------------------\n");
		}

	};

	class ComplexNullptrException : public ComplexException
	{
	public:

		ComplexNullptrException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_NULL_POINT)
		{
			m_exceptTitle = "Null Point";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexNullptrException()
		{

		}

	private:

	};

	class ComplexIndexOutOfBoundsException : public ComplexException
	{
	public:

		ComplexIndexOutOfBoundsException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_INDEX_OUT_OF_BOUNDS)
		{
			m_exceptTitle = "Index Out Of Bounds";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexIndexOutOfBoundsException()
		{

		}

	private:

	};

	class ComplexNumberFormatException : public ComplexException
	{
	public:

		ComplexNumberFormatException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_NUMBER_FORMAT)
		{
			m_exceptTitle = "Number Format";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexNumberFormatException()
		{

		}

	private:

	};

	class ComplexCapacityOverflowException : public ComplexException
	{
	public:

		ComplexCapacityOverflowException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_CAPACITY_OVERFLOW)
		{
			m_exceptTitle = "Capacity Overflow";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexCapacityOverflowException()
		{

		}

	private:

	};

	class ComplexDuplicateException : public ComplexException
	{
	public:

		ComplexDuplicateException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_DUPLICATE)
		{
			m_exceptTitle = "Duplicate";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexDuplicateException()
		{

		}

	private:

	};

	class ComplexXMLParseException : public ComplexException
	{
	public:

		ComplexXMLParseException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_XML_PARSE)
		{
			m_exceptTitle = "XML Parse";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexXMLParseException()
		{

		}

	private:

	};

	class ComplexNotFoundException : public ComplexException
	{
	public:

		ComplexNotFoundException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_NOT_FOUND)
		{
			m_exceptTitle = "Not Found";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexNotFoundException()
		{

		}

	private:

	};

	class ComplexUnIdentifiableException : public ComplexException
	{
	public:

		ComplexUnIdentifiableException(const char* explanation, const char* exceptionGenerateClassName, const char* exceptionGenerationFuncName, int exceptionGenerationLineNo = __LINE__)
			: ComplexException(ET_UNIDENTIFIABLE)
		{
			m_exceptTitle = "UnIdentifiable";
			m_exceptionMessage = explanation;
			m_exceptionGenerationClaseName = exceptionGenerateClassName;
			m_exceptionGenerationFuncName = exceptionGenerationFuncName;
			m_exceptionGenarationLineNo = exceptionGenerationLineNo;
		}

		~ComplexUnIdentifiableException()
		{

		}

	private:

	};
}