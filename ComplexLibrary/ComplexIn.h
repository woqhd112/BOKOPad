#pragma once

#include "ComplexFile.h"
#include "ComplexString.h"
#include "ComplexSingleton.h"
#include "ComplexScanner.h"

namespace ComplexLibrary
{
	class ComplexIn : public ComplexScanner
	{
	public:

		ComplexIn()
		{

		}

		virtual ~ComplexIn()
		{
			ComplexSingleton<ComplexFile>::DestroyInstance();
		}

		bool Read(ComplexString path, ComplexString& buf)
		{
			return ComplexSingleton<ComplexFile>::GetInstance().Read(path, buf);
		}

	private:



	};
}