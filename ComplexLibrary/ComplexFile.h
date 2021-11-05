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

		enum FileMode
		{
			FM_WR_CREATE_AND_EOF_WRITE = 0,
			FM_W_OVERWRITE = 1,
			FM_WR_BINARY = 2,
			FM_R_NOT_CREATE_READ = 3
		};

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

		bool Read(ComplexString& buf, FileMode fm = FM_R_NOT_CREATE_READ)
		{
			if (fm == FM_W_OVERWRITE)
				return false;

			if (buf.GetLength() > 4096)
				return false;

			std::ifstream read_file;
			if (fm == FM_WR_CREATE_AND_EOF_WRITE)
				read_file.open(m_filePath.GetBuffer(), std::ios::app);
			else if (fm == FM_WR_BINARY)
				read_file.open(m_filePath.GetBuffer(), std::ios::binary);
			else if(fm == FM_R_NOT_CREATE_READ)
				read_file.open(m_filePath.GetBuffer(), std::ios::in);

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
				buf.AppendString("\n");
			}

			read_file.close();
			return true;
		}

		bool Read(ComplexString path, ComplexString& buf, FileMode fm = FM_R_NOT_CREATE_READ)
		{
			if (fm == FM_W_OVERWRITE)
				return false;

			if (buf.GetLength() > 4096)
				return false;

			std::ifstream read_file;
			if (fm == FM_WR_CREATE_AND_EOF_WRITE)
				read_file.open(path.GetBuffer(), std::ios::app);
			else if (fm == FM_WR_BINARY)
				read_file.open(path.GetBuffer(), std::ios::binary);
			else if (fm == FM_R_NOT_CREATE_READ)
				read_file.open(path.GetBuffer(), std::ios::in);

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
				buf.AppendString("\n");
			}

			read_file.close();
			return true;
		}

		bool Write(ComplexString buf, FileMode fm = FM_W_OVERWRITE)
		{
			if (fm == FM_R_NOT_CREATE_READ)
				return false;

			std::ofstream write_file;

			if (fm == FM_W_OVERWRITE)
				write_file.open(m_filePath.GetBuffer(), std::ios::out);
			else if (fm == FM_WR_BINARY)
				write_file.open(m_filePath.GetBuffer(), std::ios::binary);
			else if (fm == FM_WR_CREATE_AND_EOF_WRITE)
				write_file.open(m_filePath.GetBuffer(), std::ios::app);

			if (!write_file.is_open())
			{
				write_file.close();
				return false;
			}

			write_file.write(buf.GetBuffer(), buf.GetLength());

			write_file.close();
			return true;
		}

		bool Write(ComplexString path, ComplexString buf, FileMode fm = FM_W_OVERWRITE)
		{
			if (fm == FM_R_NOT_CREATE_READ)
				return false;

			std::ofstream write_file;
			if (fm == FM_W_OVERWRITE)
				write_file.open(path.GetBuffer(), std::ios::out);
			else if (fm == FM_WR_BINARY)
				write_file.open(path.GetBuffer(), std::ios::binary);
			else if (fm == FM_WR_CREATE_AND_EOF_WRITE)
				write_file.open(path.GetBuffer(), std::ios::app);

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
