#pragma once

#include "ComplexOut.h"
#include "ComplexIn.h"

namespace ComplexLibrary
{
	class ComplexSystem
	{
	public:

		ComplexSystem()
		{
			out = ComplexSingleton<ComplexOut>::GetInstance();
			in = ComplexSingleton<ComplexIn>::GetInstance();
		}

		~ComplexSystem()
		{
			ComplexSingleton<ComplexOut>::DestroyInstance();
			ComplexSingleton<ComplexIn>::DestroyInstance();
		}

		ComplexOut out;
		ComplexIn in;

	};
}