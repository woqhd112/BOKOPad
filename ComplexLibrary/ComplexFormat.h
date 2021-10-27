#pragma once

namespace ComplexLibrary
{
	class ComplexFormat
	{
	public:

		ComplexFormat()
		{
			m_buf = new char[4096];
		}

		~ComplexFormat()
		{
			delete[] m_buf;
		}

		char* GetFormatString(const char* format, va_list args)
		{
			//ComplexString returnBuf;

			memset(m_buf, 0, sizeof(char) * 4096);

			int buf_cnt = 0;
			int nformatData;
			char* strFormatData;
			double dFormatData;
			bool open_format = false;
			bool use_empty_zero = false;
			bool end_dot_total = false;
			int totalsize = 0;
			int decimalsize = 0;
			char total_buf[4096];
			char decimal_buf[4096];
			while (*format)
			{
				if (*format == '%')
				{
					open_format = true;
					if (*(format + 1) == '0')
					{
						use_empty_zero = true;
						format++;
					}
					else if (*(format + 1) == '%')
					{
						format++;
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
						//putc(*format, stdout);
						open_format = false;
					}
				}
				else if (*format == 'c')
				{
					if (open_format)
					{
						nformatData = __crt_va_arg(args, int);
						char* buf = new char[4096];

						total_buf[totalsize] = '\0';
						int total_len = StringToInt(total_buf);
						GetStringToChar(buf, nformatData, use_empty_zero, total_len);
						//returnBuf.AppendFormat("%s", buf);
						int size = static_cast<int>(strlen(buf));
						for (int i = 0; i < size + 1; i++)
						{
							m_buf[buf_cnt + i] = buf[i];
						}
						buf_cnt += size;

						delete[] buf;
						memset(total_buf, 0, sizeof(char) * 4096);
						memset(decimal_buf, 0, sizeof(char) * 4096);
						open_format = false;
						use_empty_zero = false;
						end_dot_total = false;
						totalsize = 0;
						decimalsize = 0;
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else if (*format == 'd')
				{
					if (open_format)
					{
						nformatData = __crt_va_arg(args, int);
						char* buf = new char[4096];

						total_buf[totalsize] = '\0';
						int total_len = StringToInt(total_buf);
						GetStringToInt(buf, nformatData, use_empty_zero, total_len);
						//returnBuf.AppendFormat("%s", buf);
						int size = static_cast<int>(strlen(buf));
						for (int i = 0; i < size + 1; i++)
						{
							m_buf[buf_cnt + i] = buf[i];
						}
						buf_cnt += size;

						delete[] buf;
						memset(total_buf, 0, sizeof(char) * 4096);
						memset(decimal_buf, 0, sizeof(char) * 4096);
						open_format = false;
						use_empty_zero = false;
						end_dot_total = false;
						totalsize = 0;
						decimalsize = 0;
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else if (*format == 's')
				{
					if (open_format)
					{
						strFormatData = __crt_va_arg(args, char*);
						char* buf = new char[4096];

						total_buf[totalsize] = '\0';
						int total_len = StringToInt(total_buf);
						GetStringToString(buf, strFormatData, use_empty_zero, total_len);
						//returnBuf.AppendFormat("%s", buf);
						int size = static_cast<int>(strlen(buf));
						for (int i = 0; i < size + 1; i++)
						{
							m_buf[buf_cnt + i] = buf[i];
						}
						buf_cnt += size;

						delete[] buf;
						memset(total_buf, 0, sizeof(char) * 4096);
						memset(decimal_buf, 0, sizeof(char) * 4096);
						open_format = false;
						use_empty_zero = false;
						end_dot_total = false;
						totalsize = 0;
						decimalsize = 0;
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else if (*format == 'f')
				{
					if (open_format)
					{
						dFormatData = __crt_va_arg(args, double);
						char* buf = new char[4096];

						total_buf[totalsize] = '\0';
						decimal_buf[decimalsize] = '\0';
						int total_len = StringToInt(total_buf);
						int decimal_len = StringToInt(decimal_buf);
						GetStringToDouble(buf, dFormatData, use_empty_zero, total_len, decimal_len);
						//returnBuf.AppendFormat("%s", buf);
						int size = static_cast<int>(strlen(buf));
						for (int i = 0; i < size + 1; i++)
						{
							m_buf[buf_cnt + i] = buf[i];
						}
						buf_cnt += size;

						delete[] buf;
						memset(total_buf, 0, sizeof(char) * 4096);
						memset(decimal_buf, 0, sizeof(char) * 4096);
						open_format = false;
						use_empty_zero = false;
						end_dot_total = false;
						totalsize = 0;
						decimalsize = 0;
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else if (*format == '.')
				{
					if (open_format)
					{
						end_dot_total = true;
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else if (*format == '1' || *format == '2' || *format == '3' ||
					*format == '4' || *format == '5' || *format == '6' ||
					*format == '7' || *format == '8' || *format == '9' ||
					*format == '0')
				{
					if (open_format)
					{
						if (end_dot_total)
						{
							decimal_buf[decimalsize] = *format;
							decimalsize++;
						}
						else
						{
							total_buf[totalsize] = *format;
							totalsize++;
						}
					}
					else
					{
						//returnBuf.AppendFormat("%c", *format);
						m_buf[buf_cnt] = *format;
						buf_cnt++;
					}
				}
				else
				{
					//returnBuf.AppendFormat("%c", *format);
					m_buf[buf_cnt] = *format;
					buf_cnt++;
				}

				format++;
			}

			m_buf[buf_cnt] = '\0';

			//return returnBuf;
			return m_buf;
		}

	private:

		char* m_buf;

	private:

		void GetStringToDouble(char* buf, double value, bool use_empty_zero, int total_size, int decimal_size)
		{
			int before_size = 0;
			int after_size = 0;
			int dot_defore = static_cast<int>(value);
			int calc_before = dot_defore;

			while (calc_before)
			{
				calc_before /= 10;
				before_size++;
			}

			double dot_after = value - static_cast<double>(dot_defore);
			unsigned long long floor_remainder_dot_after = static_cast<unsigned long long>(dot_after);

			while (dot_after != floor_remainder_dot_after)
			{
				dot_after *= 10;
				after_size++;
				floor_remainder_dot_after = static_cast<unsigned long long>(dot_after);
			}

			char* integer_part = new char[4096];
			char* double_part = new char[4096];
			char dot = '.';
			int before_total = 0;
			// 전체 사이즈 -1은 . 문자도 길이에 포함되기 때문
			// (전체 사이즈 - 1) - 실수 사이즈(정수 사이즈) 가 정수 사이즈보다 작을 경우 -> 정수부분은 전부 출력하고 실수 부분은 decimal_size 만큼 출력
			if (abs((total_size - 1) - decimal_size) < before_size)
			{
				before_total = before_size;
				GetStringToInt(integer_part, dot_defore, use_empty_zero, before_total);
			}
			else
			{
				if (total_size == 0)
					before_total = before_size;
				else
					before_total = (total_size - 1) - decimal_size;
				GetStringToInt(integer_part, dot_defore, use_empty_zero, before_total);
			}
			// 더블파트 decimal_size 와 after_size로 처리
			// 실제 실수부분이 더 클경우 오차만큼 짜름.
			if (decimal_size < after_size)
			{
				// decimal_size 사이즈만큼 짤라서 출력
				GetStringToInt(double_part, floor_remainder_dot_after, use_empty_zero, after_size);
				if (decimal_size == 0)
					decimal_size = after_size;

				double_part[decimal_size] = '\0';
			}
			// 실제 실수부분이 더 작을경우 뒤에 특정 문자 추가
			else
			{
				// decimal_size - after_size 오차만큼 특정 문자 추가
				GetStringToInt(double_part, floor_remainder_dot_after, use_empty_zero, after_size);
				if (decimal_size == 0)
					decimal_size = after_size;

				for (int i = after_size; i < decimal_size; i++)
				{
					if (use_empty_zero)
						double_part[i] = '0';
					else
						double_part[i] = ' ';
				}
				double_part[after_size] = '\0';
			}

			for (int i = 0; i < before_total; i++)
			{
				buf[i] = integer_part[i];
			}
			buf[before_total] = dot;
			for (int i = 0; i < decimal_size; i++)
			{
				buf[i + before_total + 1] = double_part[i];
			}
			buf[before_total + 1 + decimal_size] = '\0';

			delete[] integer_part;
			delete[] double_part;
		}

		void GetStringToString(char* buf, char* value, bool use_empty_zero, int total_size)
		{
			if (value == NULL)
				return;

			int len = static_cast<int>(strlen(value));

			int empty_count = 0;
			if (total_size >= len)
			{
				empty_count = total_size - len;
			}
			else if (total_size == 0)
				total_size = len + 1;

			for (int i = 0; i < empty_count; i++)
			{
				if (use_empty_zero)
					buf[i] = '0';
				else
					buf[i] = ' ';
			}
			int j = 0;
			for (int i = empty_count; i < total_size; i++)
			{
				buf[i] = value[j];
				j++;
			}
			buf[total_size] = '\0';
		}

		void GetStringToChar(char* buf, char value, bool use_empty_zero, int total_size)
		{
			for (int i = 0; i < total_size - 1; i++)
			{
				if (use_empty_zero)
					buf[i] = '0';
				else
					buf[i] = ' ';
			}
			if (total_size > 0)
			{
				buf[total_size - 1] = value;
				buf[total_size] = '\0';
			}
			else
			{
				buf[0] = value;
				buf[1] = '\0';
			}
		}

		void GetStringToInt(char* buf, unsigned long long value, bool use_empty_zero, int total_size)
		{
			unsigned long long tmp = value;
			int count = 0;
			char* newBuf = new char[4096];
			/*while (tmp)
			{
				int remainder = tmp % 10;
				newBuf[count] = remainder + 48;

				tmp /= 10;
				count++;
			}*/
			do
			{
				int remainder = tmp % 10;
				newBuf[count] = remainder + 48;

				tmp /= 10;
				count++;
			}
			while (tmp);

			if (count < total_size)
			{
				int error = total_size - count;
				int i = 0;
				for (i = 0; i < error; i++)
				{
					if (use_empty_zero)
						newBuf[i + count] = '0';
					else
						newBuf[i + count] = ' ';
				}
				count += i;
			}
			newBuf[count] = '\0';

			int j = 0;
			for (int i = count - 1; i >= 0; i--)
			{
				buf[j] = newBuf[i];
				j++;
			}
			buf[count] = '\0';
			delete[] newBuf;
		}

		int StringToInt(const char* value)
		{
			int size = strlen(value);

			int total = 0;
			int count = 0;
			for (int i = size - 1; i >= 0; i--)
			{
				int su = value[i] - 48;
				total += (su * static_cast<int>(pow(10, count)));
				count++;
			}

			return total;
		}
	};
}