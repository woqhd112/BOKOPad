#pragma once

#include "ComplexString.h"
#include "ComplexRandom.h"
#include "ComplexConvert.h"
#include "ComplexTime.h"
#include "ComplexSingleton.h"

namespace ComplexLibrary
{
	class ComplexUtility
	{
	public:

		ComplexUtility()
		{
			time = ComplexSingleton<ComplexTime>::GetInstance();
			convert = ComplexSingleton<ComplexConvert>::GetInstance();
			random = ComplexSingleton<ComplexRandom>::GetInstance();
		}

		virtual ~ComplexUtility()
		{
			ComplexSingleton<ComplexTime>::DestroyInstance();
			ComplexSingleton<ComplexConvert>::DestroyInstance();
			ComplexSingleton<ComplexRandom>::DestroyInstance();
		}

		ComplexTime time;
		ComplexConvert convert;
		ComplexRandom random;

	};
}