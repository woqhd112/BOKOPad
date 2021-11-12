#pragma once
#include <string>
#include <iostream>

#include "ComplexFormat.h"

#define USE_MYFORMAT	1

namespace ComplexLibrary
{
	class ComplexString
	{
	public: 
		 
		ComplexString()
			: m_size(0), m_buf(nullptr)
		{
			m_size = 1;
			m_buf = new char[m_size];
			m_buf[0] = '\0';
		}

		ComplexString(int size)
			: m_size(size), m_buf(nullptr)
		{
			m_size = size + 1;
			m_buf = new char[m_size];
			m_buf[m_size - 1] = '\0';
		}

		ComplexString(char c)
			: m_size(0), m_buf(nullptr)
		{
			m_size = 2;
			m_buf = new char[m_size];
			m_buf[0] = c;
			m_buf[1] = '\0';
		}

		ComplexString(const char* buf)
			: m_size(0), m_buf(nullptr)
		{
			m_size = static_cast<int>(strlen(buf)) + 1;

			m_buf = new char[m_size];

			for (int i = 0; i < m_size; i++)
			{
				m_buf[i] = buf[i];
			}
			m_buf[m_size - 1] = '\0';
			//strcpy_s(m_buf, m_size, buf);
		}

		ComplexString(std::string buf)
			: m_size(0), m_buf(nullptr)
		{
			m_size = static_cast<int>(buf.length());

			m_buf = new char[m_size];
			for (int i = 0; i < m_size; i++)
			{
				m_buf[i] = buf[i];
			}
			m_buf[m_size - 1] = '\0';
			//strcpy_s(m_buf, m_size, buf.c_str());
		}

		ComplexString(const ComplexString& buf)
			: m_size(0), m_buf(nullptr)
		{
			m_size = buf.m_size;
			m_buf = new char[m_size];

			for (int i = 0; i < m_size; i++)
			{
				m_buf[i] = buf.m_buf[i];
			}
			m_buf[m_size - 1] = '\0';
			//strcpy_s(m_buf, m_size, buf.m_buf);
		}

		~ComplexString()
		{
			if (m_buf != nullptr)
				delete[] m_buf;
		}

		int GetLength()
		{
			return m_size - 1;
		}

		const char* GetBuffer()
		{
			return m_buf;
		}

		ComplexString Left(int leftToCount)
		{
			if (m_buf == nullptr)
				return "";

			if (leftToCount <= 0)
				return "";

			if (leftToCount > (m_size - 1))
				leftToCount = m_size - 1;

			char* tmp = new char[m_size];

			int i = 0;
			for (i = 0; i < leftToCount; i++)
			{
				tmp[i] = m_buf[i];
			}
			tmp[i] = '\0';

			ComplexString strTmp = tmp;
			delete[] tmp;

			return strTmp;
		}

		ComplexString Right(int rightToCount)
		{
			if (m_buf == nullptr)
				return "";

			if (rightToCount <= 0)
				return "";

			if (rightToCount > (m_size - 1))
				rightToCount = m_size - 1;

			char* tmp = new char[m_size];

			int i = 0;
			for (i = rightToCount - 1; i < m_size - 1; i++)
			{
				tmp[i] = m_buf[i];
			}
			tmp[i] = '\0';

			ComplexString strTmp = tmp;
			delete[] tmp;

			return strTmp;
		}

		char GetAt(int idx)
		{
			if (idx < 0 || idx >(m_size - 1))
				return 0;

			return m_buf[idx];
		}

		void SetAt(int idx, char c)
		{
			if (idx < 0 || idx >(m_size - 1 - 1))
				return;

			m_buf[idx] = c;
		}

		bool Compare(ComplexString& buf)
		{
			return (strcmp(m_buf, buf.m_buf) == 0) ? true : false;
		}

		bool Compare(const char* buf)
		{
			return (strcmp(m_buf, buf) == 0) ? true : false;
		}

		void Format(const char* format, ...)
		{
			va_list args;
			__crt_va_start(args, format);

			//int format_size = strlen(format) + 1 + 1 + 1;


#if USE_MYFORMAT
			ComplexFormat formatting;
			const char* tmp = formatting.GetFormatString(format, args);

			m_size = strlen(tmp) + 1;

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			for (int i = 0; i < m_size - 1; i++)
			{
				m_buf[i] = tmp[i];
			}
#elif
			char tmp[4096];

			m_size = static_cast<int>(vsnprintf(tmp, 4096, format, args) + 1);

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			for (int i = 0; i < m_size - 1; i++)
			{
				m_buf[i] = tmp[i];
			}
#endif
			m_buf[m_size - 1] = '\0';

			__crt_va_end(args);
		}

		void AppendFormat(const char* format, ...)
		{
			va_list args;
			__crt_va_start(args, format);

#if USE_MYFORMAT
			ComplexFormat formatting;
			const char* tmp = formatting.GetFormatString(format, args);

			int arg_size = strlen(tmp) + 1;
#elif
			char tmp[4096];
			int arg_size = static_cast<int>(vsnprintf(tmp, 4096, format, args) + 1);
#endif

			int before_size = m_size;
			char* before_buf = new char[m_size];

			strcpy_s(before_buf, m_size, m_buf);

			m_size += (arg_size - 1);

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			for (int i = 0; i < before_size - 1; i++)
			{
				m_buf[i] = before_buf[i];
			}
			for (int i = 0; i < arg_size - 1; i++)
			{
				m_buf[i + before_size - 1] = tmp[i];
			}

			m_buf[m_size - 1] = '\0';

			delete[] before_buf;

			__crt_va_end(args);
		}

		void SetLower(int index)
		{
			if (index > (m_size - 2))
				return;

			char check_c = m_buf[index];
			if (isalpha(check_c) != 0)	// alpha ok
			{
				if (isalpha(check_c) == 1)	// upper ok
				{
					check_c += 32;	// convert upper -> lower
					m_buf[index] = check_c;
				}
			}
		}

		void SetUpper(int index)
		{
			if (index > (m_size - 2))
				return;

			char check_c = m_buf[index];
			if (isalpha(check_c) != 0)	// alpha ok
			{
				if (isalpha(check_c) == 2)	// lower ok
				{
					check_c -= 32;	// convert lower -> upper
					m_buf[index] = check_c;
				}
			}
		}

		void SetAllLower()
		{
			int last_idx = m_size - 1 - 1;
			while (last_idx != -1)
			{
				SetLower(last_idx);
				last_idx--;
			}
		}

		void SetAllUpper()
		{
			int last_idx = m_size - 1 - 1;
			while (last_idx != -1)
			{
				SetUpper(last_idx);
				last_idx--;
			}
		}

		void AppendString(ComplexString buf)
		{
			ComplexString add_buf;

			add_buf = *this + buf;

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[add_buf.m_size];
			for (int i = 0; i < add_buf.m_size - 1; i++)
			{
				m_buf[i] = add_buf.m_buf[i];
			}
			m_buf[add_buf.m_size - 1] = '\0';
			m_size = add_buf.m_size;
		}

		bool IsEmpty()
		{
			return (m_size <= 1);
		}

		void Trim()
		{
			TrimLeft();
			TrimRight();
		}

		void TrimLeft()
		{
			while (m_buf[0] == 32 || m_buf[0] == 9)
			{
				Erase(0);
			}
		}

		void TrimRight()
		{
			while (m_buf[m_size - 1 - 1] == 32 || m_buf[m_size - 1 - 1] == 9)
			{
				Erase(m_size - 1 - 1);
			}
		}

		ComplexString Reverse()
		{
			char* reverse_buf = new char[m_size];

			for (int i = 0; i < m_size - 1; i++)
			{
				reverse_buf[i] = m_buf[m_size - 1 - 1 - i];
			}
			reverse_buf[m_size - 1] = '\0';

			ComplexString return_buf(reverse_buf);
			delete[] reverse_buf;

			return return_buf;
		}

		// find == true -> return find first index buf
		// return value -> not find : -1 / find : first find index
		int Find(ComplexString findbuf)
		{
			bool bFind = false;

			int find_index = 0;
			bFind = loopFind(findbuf, find_index, 0);

			if (!bFind)
				return -1;

			return find_index;
		}

		bool Erase(int index)
		{
			if (index > (m_size - 2))
				return false;

			char* newBuf = new char[m_size - 1];

			int i = 0;
			for (i = 0; i < index; i++)
			{
				newBuf[i] = m_buf[i];
			}
			i++;
			if (i < m_size - 1 - 1)
			{
				for (int j = 0; j < m_size - 1 - 1; j++)
				{
					newBuf[j + i - 1] = m_buf[j + i];
				}
			}
			newBuf[m_size - 1 - 1] = '\0';

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size - 1];

			for (i = 0; i < m_size - 1 - 1; i++)
			{
				m_buf[i] = newBuf[i];
			}
			m_buf[m_size - 1 - 1] = '\0';

			delete[] newBuf;
			m_size--;

			return true;
		}

		ComplexString GetText(int startidx, int endidx)
		{
			if (endidx - startidx <= 0)
				return "";
			ComplexString at_string;
			int at_size = endidx - startidx + 1;
			char* at_buf = new char[at_size + 1];
			for (int i = 0; i < at_size; i++)
			{
				at_buf[i] = m_buf[i + startidx];
			}
			at_buf[at_size] = '\0';
			at_string = at_buf;

			return at_string;
		}

		void Clear()
		{
			if (m_buf != nullptr)
				delete[] m_buf;

			m_size = 1;
			m_buf = new char[m_size];
			m_buf[0] = '\0';
		}

		bool RemoveAll(ComplexString find_buf)
		{
			bool bFind = false;

			while (Remove(find_buf))
			{
				bFind = true;
			}

			return bFind;
		}

		bool Remove(ComplexString find_buf)
		{
			int find_idx = Find(find_buf);
			if (find_idx == -1)
				return false;

			int pre_start_idx = find_idx;
			int suf_start_idx = find_idx + (find_buf.m_size - 1);
			int pre_buf_size = pre_start_idx;
			int suf_buf_size = m_size - (suf_start_idx + 1);

			char* pre_buf = new char[pre_buf_size + 1];
			// 여기 수정
			char* suf_buf = new char[suf_buf_size + 1];

			for (int i = 0; i < pre_buf_size; i++)
			{
				pre_buf[i] = m_buf[i];
			}
			pre_buf[pre_buf_size] = '\0';

			for (int i = 0; i < suf_buf_size; i++)
			{
				suf_buf[i] = m_buf[i + suf_start_idx];
			}
			suf_buf[suf_buf_size] = '\0';

			if (m_buf != nullptr)
				delete[] m_buf;

			m_size = (pre_buf_size)+(suf_buf_size);
			m_buf = new char[m_size + 1];

			int new_idx = 0;
			for (int i = 0; i < pre_buf_size; i++)
			{
				m_buf[i] = pre_buf[i];
				new_idx = i;
			}

			for (int i = 0; i < suf_buf_size; i++)
			{
				m_buf[i + pre_buf_size] = suf_buf[i];
			}
			m_buf[m_size] = '\0';
			m_size++;

			delete[] pre_buf;
			delete[] suf_buf;

			return true;
		}

		bool ReplaceAll(ComplexString find_buf, ComplexString change_buf)
		{
			bool bFind = false;

			while (Replace(find_buf, change_buf))
			{
				bFind = true;
			}

			return bFind;
		}

		bool Replace(ComplexString find_buf, ComplexString change_buf)
		{
			int find_idx = Find(find_buf);
			if (find_idx == -1)
				return false;

			int pre_start_idx = find_idx;
			int suf_start_idx = find_idx + (find_buf.m_size - 1);
			int pre_buf_size = pre_start_idx;
			int suf_buf_size = m_size - (suf_start_idx + 1);

			char* pre_buf = new char[pre_buf_size + 1];
			// 여기 수정
			char* suf_buf = new char[suf_buf_size + 1];

			for (int i = 0; i < pre_buf_size; i++)
			{
				pre_buf[i] = m_buf[i];
			}
			pre_buf[pre_buf_size] = '\0';

			for (int i = 0; i < suf_buf_size; i++)
			{
				suf_buf[i] = m_buf[i + suf_start_idx];
			}
			suf_buf[suf_buf_size] = '\0';

			if (m_buf != nullptr)
				delete[] m_buf;

			m_size = (pre_buf_size)+(change_buf.m_size - 1) + (suf_buf_size);
			m_buf = new char[m_size + 1];

			int new_idx = 0;
			for (int i = 0; i < pre_buf_size; i++)
			{
				m_buf[i] = pre_buf[i];
				new_idx = i;
			}

			for (int i = 0; i < change_buf.m_size - 1; i++)
			{
				m_buf[i + pre_buf_size] = change_buf.m_buf[i];
			}

			for (int i = 0; i < suf_buf_size; i++)
			{
				m_buf[i + pre_buf_size + change_buf.m_size - 1] = suf_buf[i];
			}
			m_buf[m_size] = '\0';
			m_size++;

			delete[] pre_buf;
			delete[] suf_buf;

			return true;
		}

		operator const char*()
		{
			return m_buf;
		}

		ComplexString& operator = (const char* buf)
		{
			m_size = static_cast<int>(strlen(buf)) + 1;

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			strcpy_s(m_buf, m_size, buf);

			return *this;
		}

		/*ComplexString& operator = (ComplexString& buf)
		{
			m_size = buf.m_size;

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			strcpy_s(m_buf, m_size, buf.m_buf);

			return *this;
		}*/

		ComplexString& operator = (const ComplexString& buf)
		{
			m_size = buf.m_size;

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[m_size];
			strcpy_s(m_buf, m_size, buf.m_buf);

			return *this;
		}

		ComplexString operator + (int num)
		{
			ComplexString tmp;
			tmp.Format("%d", num);
			ComplexString thisBuf = *this;
			thisBuf += tmp;

			return thisBuf;
		}

		ComplexString operator + (double num)
		{
			ComplexString tmp;
			tmp.Format("%f", num);
			ComplexString thisBuf = *this;
			thisBuf += tmp;

			return thisBuf;
		}

		ComplexString operator + (const char* buf)
		{
			ComplexString tmp;
			char* in = new char[m_size];
			strcpy_s(in, m_size, m_buf);

			int bufsize = static_cast<int>(strlen(buf));

			int newbufsize = m_size + bufsize;

			char* newbuf = new char[newbufsize];

			for (int i = 0; i < m_size - 1; i++)
			{
				newbuf[i] = in[i];
			}
			for (int i = 0; i < bufsize; i++)
			{
				newbuf[i + m_size - 1] = buf[i];
			}
			newbuf[newbufsize - 1] = '\0';

			delete[] in;

			tmp = newbuf;
			delete[] newbuf;

			return tmp;
		}

		ComplexString operator + (ComplexString& buf)
		{
			ComplexString tmp;
			char* in = new char[m_size];
			strcpy_s(in, m_size, m_buf);

			int bufsize = static_cast<int>(strlen(buf.m_buf));

			int newbufsize = m_size + bufsize;
			char* newbuf = new char[newbufsize];

			for (int i = 0; i < m_size - 1; i++)
			{
				newbuf[i] = in[i];
			}
			for (int i = 0; i < bufsize; i++)
			{
				newbuf[i + m_size - 1] = buf.m_buf[i];
			}
			newbuf[newbufsize - 1] = '\0';

			delete[] in;

			tmp = newbuf;
			delete[] newbuf;

			return tmp;
		}

		ComplexString& operator += (int num)
		{
			ComplexString tmp;
			tmp.Format("%d", num);
			*this += tmp;

			return *this;
		}

		ComplexString& operator += (double num)
		{
			ComplexString tmp;
			tmp.Format("%f", num);
			*this += tmp;

			return *this;
		}

		ComplexString& operator += (const char* buf)
		{
			int bufsize = static_cast<int>(strlen(buf));
			int totalsize = m_size + bufsize + (m_size > 0 ? 0 : 1);

			char* tmp = new char[m_size];
			strcpy_s(tmp, m_size, m_buf);

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[totalsize];
			for (int i = 0; i < m_size - 1; i++)
			{
				m_buf[i] = tmp[i];
			}
			for (int i = 0; i < bufsize; i++)
			{
				m_buf[i + m_size - 1] = buf[i];
			}
			m_buf[totalsize - 1] = '\0';

			delete[] tmp;
			m_size = totalsize;
			return *this;
		}

		ComplexString& operator += (ComplexString& buf)
		{
			int bufsize = static_cast<int>(strlen(buf.m_buf));
			int totalsize = m_size + bufsize + (m_size > 0 ? 0 : 1);

			char* tmp = new char[m_size];
			strcpy_s(tmp, m_size, m_buf);

			if (m_buf != nullptr)
				delete[] m_buf;

			m_buf = new char[totalsize];
			for (int i = 0; i < m_size - 1; i++)
			{
				m_buf[i] = tmp[i];
			}
			for (int i = 0; i < bufsize; i++)
			{
				m_buf[i + m_size - 1] = buf.m_buf[i];
			}
			m_buf[totalsize - 1] = '\0';

			delete[] tmp;
			m_size = totalsize;
			return *this;
		}

		bool operator == (ComplexString& buf)
		{
			return (strcmp(m_buf, buf.m_buf) == 0) ? true : false;
		}

		bool operator == (const char* buf)
		{
			return (strcmp(m_buf, buf) == 0) ? true : false;
		}

		bool operator != (ComplexString& buf)
		{
			return (strcmp(m_buf, buf.m_buf) != 0) ? true : false;
		}

		bool operator != (const char* buf)
		{
			return (strcmp(m_buf, buf) != 0) ? true : false;
		}

		friend static std::istream& operator >> (std::istream& cin, ComplexString& buf);
		friend static std::ostream& operator << (std::ostream& cout, ComplexString& buf);

	private:

		bool loopFind(ComplexString& findbuf, int& find_index, int index)
		{
			bool bNotFind = true;

			int h = 0;
			for (h = index; h < m_size; h++)
			{
				if (m_buf[h] == findbuf.m_buf[0])
				{
					bNotFind = false;
					break;
				}
			}

			if (bNotFind)
				return false;

			int compare_cnt = 0;
			for (int i = 0; i < findbuf.m_size - 1; i++)
			{
				if (m_buf[h + i] == findbuf.m_buf[i])
				{
					compare_cnt++;
				}
			}

			if (compare_cnt == (findbuf.m_size - 1))
			{
				find_index = h;
				return true;
			}
			h++;

			return loopFind(findbuf, find_index, h);
		}

	private:

		int m_size;
		char* m_buf;

	};

	static std::istream& operator >> (std::istream& cin, ComplexString& buf)
	{
		char str[100];
		cin >> str;
		buf = ComplexString(str);
		return cin;
	}

	static std::ostream& operator << (std::ostream& cout, ComplexString& buf)
	{
		cout << buf.GetBuffer();
		return cout;
	}
}