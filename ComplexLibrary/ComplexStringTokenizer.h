#pragma once

#include "ComplexString.h"
#include "ComplexLinkedList.h"

namespace ComplexLibrary
{
	class ComplexStringTokenizer
	{
	public:

		typedef ComplexLinkedList<ComplexString>::iterator TokenIterator;
		/*
		param 1 : 스트링 값
		param 2 : 자를 토큰 (값이 없으면 디폴트 ' ')
		param 3 : 자를 토큰 까지 컨테이너에 담을건지 여부 (값이 없으면 디폴트 false)
		*/

		ComplexStringTokenizer()
			: m_iterator(nullptr)
		{

		}

		ComplexStringTokenizer(ComplexString buf, char token = 32, bool include_token = false)
		{
			ApplyStringTokenize(buf, token, include_token);
		}

		~ComplexStringTokenizer()
		{
			Clear();
		}

		bool HasNextToken()
		{
			if (m_iterator == m_tokens.end())
				return false;

			return true;
		}

		bool HasNextIterator()
		{
			return HasNextToken();
		}

		TokenIterator NextIterator()
		{
			if (m_iterator == m_tokens.end())
				return TokenIterator(nullptr);

			return m_iterator++;
		}

		ComplexString NextToken()
		{
			if (m_iterator == m_tokens.end())
				return "";

			ComplexString returnValue = m_iterator->value;
			m_iterator++;
			return returnValue;
		}

		void ApplyStringTokenize(ComplexString buf, char token = 32, bool include_token = false)
		{
			Clear();
			Split(buf, token, include_token);
		}

		void Clear()
		{
			m_iterator = m_tokens.end();
			m_tokens.clear();
		}

		int GetTokenCount()
		{
			return m_tokens.size();
		}

	private:

		void Split(ComplexString buf, char token, bool include_token)
		{
			if (SplitBuf(buf, token, include_token))
				m_iterator = m_tokens.begin();
		}

		bool SplitBuf(ComplexString strings, char div, bool include_token)
		{
			int find_cnt = 0;
			const char* buf = strings.GetBuffer();
			for (int i = 0; i < strings.GetLength(); i++)
			{
				if (buf[i] == div)
				{
					find_cnt++;
				}
			}

			if (find_cnt <= 0)
				return false;

			/*if (loopSplit(buf, strings.GetLength(), div, 0, include_token) == false)
			{
				if (m_tokens.empty())
					return false;
			}*/

			ComplexString returnBuf = loopSplit(buf, strings.GetLength(), div, 0, include_token);

			while (returnBuf != "")
			{
				returnBuf = loopSplit(returnBuf.GetBuffer(), returnBuf.GetLength(), div, 0, include_token);
			}

			if (m_tokens.empty())
				return false;

			return true;
		}

		ComplexString loopSplit(const char* buf, int bufSize, char div, int matrix_cnt, bool include_token)
		{
			ComplexString preFindString, sufFindString;
			char* tmp1 = new char[bufSize + 1];
			//char* tmp2 = new char[bufs.GetLength() + 1];
			bool bFind = false;

			int buf_idx = 0;
			int pre_len = 0;
			//int suf_len = 0;
			//bool oneFind = true;

			for (int i = 0; i < static_cast<int>(strlen(buf)); i++)
			{
				if (buf[i] == div)
				{
					bFind = true;
					/*if (oneFind)
					{
						buf_idx = 0;
						continue;
					}*/

				}

				if (bFind)
				{
					break;
					//tmp2[buf_idx] = buf[i];
					//buf_idx++;
					//suf_len++;
					//oneFind = false;
				}
				else
				{
					tmp1[buf_idx] = buf[i];
					buf_idx++;
					pre_len++;
				}
			}

			tmp1[pre_len] = '\0';
			//tmp2[suf_len] = '\0';
			preFindString = tmp1;
			//sufFindString = tmp2;
			sufFindString = buf;
			sufFindString.Remove(preFindString + ComplexString(div));
			delete[] tmp1;
			//delete[] tmp2;


			m_tokens.push_tail(preFindString);
			if (bFind)
			{
				if (include_token)
					m_tokens.push_tail(ComplexString(div));
			}

			if (sufFindString.GetLength() + 1 <= 1)
				return "";

			return sufFindString;
		}

		//bool loopSplit(const char* buf, int bufSize, char div, int matrix_cnt, bool include_token)
		//{
		//	ComplexString preFindString, sufFindString;
		//	char* tmp1 = new char[bufSize + 1];
		//	//char* tmp2 = new char[bufs.GetLength() + 1];
		//	bool bFind = false;

		//	int buf_idx = 0;
		//	int pre_len = 0;
		//	//int suf_len = 0;
		//	//bool oneFind = true;

		//	for (int i = 0; i < static_cast<int>(strlen(buf)); i++)
		//	{
		//		if (buf[i] == div)
		//		{
		//			bFind = true;
		//			/*if (oneFind)
		//			{
		//				buf_idx = 0;
		//				continue;
		//			}*/

		//		}

		//		if (bFind)
		//		{
		//			break;
		//			//tmp2[buf_idx] = buf[i];
		//			//buf_idx++;
		//			//suf_len++;
		//			//oneFind = false;
		//		}
		//		else
		//		{
		//			tmp1[buf_idx] = buf[i];
		//			buf_idx++;
		//			pre_len++;
		//		}
		//	}

		//	tmp1[pre_len] = '\0';
		//	//tmp2[suf_len] = '\0';
		//	preFindString = tmp1;
		//	//sufFindString = tmp2;
		//	sufFindString = buf;
		//	sufFindString.Remove(preFindString + ComplexString(div));
		//	delete[] tmp1;
		//	//delete[] tmp2;


		//	m_tokens.push_tail(preFindString);
		//	if (bFind)
		//	{
		//		if (include_token)
		//			m_tokens.push_tail(ComplexString(div));
		//	}

		//	if (sufFindString.GetLength() + 1 <= 1)
		//		return false;

		//	return loopSplit(sufFindString.GetBuffer(), bufSize, div, matrix_cnt + 1, include_token);
		//}

		TokenIterator m_iterator;
		ComplexLinkedList<ComplexString> m_tokens;
	};
}