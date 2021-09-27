#pragma once

#include "ComplexFile.h"
#include "ComplexPrint.h"
#include "ComplexSingleton.h"


namespace ComplexLibrary
{
	class ComplexOut : public ComplexPrint
	{
	public:

		ComplexOut()
		{
		
		}

		virtual ~ComplexOut()
		{
			ComplexSingleton<ComplexFile>::DestroyInstance();
		}

		bool Write(ComplexString path, ComplexString buf)
		{
			return ComplexSingleton<ComplexFile>::GetInstance().Write(path, buf);
		}

	private:


	};
}
