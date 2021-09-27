#pragma once

#include "ComplexStringTokenizer.h"

namespace ComplexLibrary
{
	class ComplexScanner
	{
	public:

		ComplexScanner()
		{

		}

		virtual ~ComplexScanner()
		{
			m_st.Clear();
		}

		bool HasNext()
		{
			return m_st.HasNextToken();
		}

		// buf를 받고 ' '로 잘라서 보관하고있다가 토큰들이 존재하면 다음 토큰을 리턴하고 존재하지않으면 새롭게 받는다.
		ComplexString Next()
		{
			ComplexString in = "";
			ComplexString returnBuf = in;

			if (m_st.HasNextToken() == false)
			{
				char buf[2048];
				scanf_s("%s", buf, 2048);
				in = buf;
				m_st.ApplyStringTokenize(in, ' ');
			}

			returnBuf = m_st.NextToken();

			return returnBuf;
		}

		// buf를 받고 그대로 리턴한다.
		// nextline 하면 next했을 때 토큰을 전부 초기화하는게 맞는건가? 일단 초기화한다.
		ComplexString NextLine()
		{
			m_st.Clear();

			char buf[2048];

			scanf_s("%s", buf, 2048);

			ComplexString in = buf;

			return in;
		}

	private:

		ComplexStringTokenizer m_st;

	};

}
