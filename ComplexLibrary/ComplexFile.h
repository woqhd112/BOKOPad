#pragma once

#include <iostream>
#include <fstream>
#include "ComplexString.h"

namespace ComplexLibrary
{
	class ComplexFile
	{
	public:

		ComplexFile()
		{

		}

		ComplexFile(ComplexString path)
		{
			m_filePath = path;
		}

		~ComplexFile()
		{

		}

		bool Exist(ComplexString path)
		{
			std::ifstream read_file;
			std::ofstream write_file;
			read_file.open(path);
			if (read_file.fail())
			{
				write_file.open(path);
				write_file.close();
				return false;
			}

			read_file.close();
			return true;
		}

		ComplexString GetPath() const
		{
			return m_filePath;
		}

		bool Read(ComplexString& buf)
		{
			if (buf.GetLength() > 4096)
				return false;

			std::ifstream read_file;
			read_file.open(m_filePath.GetBuffer());

			if (!read_file.is_open())
			{
				read_file.close();
				return false;
			}

			while (!read_file.eof())
			{
				char arr[4096];
				read_file.getline(arr, 4096);
				buf.AppendString(arr);
				buf.AppendString("\r\n");
			}

			read_file.close();
			return true;
		}

		bool Read(ComplexString path, ComplexString& buf)
		{
			if (buf.GetLength() > 4096)
				return false;

			std::ifstream read_file;
			read_file.open(path.GetBuffer());

			if (!read_file.is_open())
			{
				read_file.close();
				return false;
			}

			while (!read_file.eof())
			{
				char arr[4096];
				read_file.getline(arr, 4096);
				buf.AppendString(arr);
				buf.AppendString("\r\n");
			}

			read_file.close();
			return true;
		}

		bool Write(ComplexString buf)
		{
			std::ofstream write_file;
			write_file.open(m_filePath.GetBuffer());

			if (!write_file.is_open())
			{
				write_file.close();
				return false;
			}

			write_file.write(buf.GetBuffer(), buf.GetLength());

			write_file.close();
			return true;
		}

		bool Write(ComplexString path, ComplexString buf)
		{
			std::ofstream write_file;
			write_file.open(path.GetBuffer());

			if (!write_file.is_open())
			{
				write_file.close();
				return false;
			}

			write_file.write(buf.GetBuffer(), buf.GetLength());

			write_file.close();
			return true;
		}

	private:


		ComplexString m_filePath;

	};
}
