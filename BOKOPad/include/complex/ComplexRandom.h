#pragma once

#include "ComplexTime.h"

namespace ComplexLibrary
{
	class ComplexRandom
	{
	public:

		ComplexRandom()
		{

		}

		virtual ~ComplexRandom()
		{

		}

		int Random(int start_range, int end_range)
		{
			if (start_range > end_range)
			{
				int tmp = end_range;
				end_range = start_range;
				start_range = tmp;
			}
			else if (start_range == end_range)
				return end_range;

			int range_count = (end_range - start_range) + 1;
			int seed = Seed(range_count);
			seed += start_range;
			return seed;
		}

		int Random()
		{
			return Seed();
		}

	private:

		int Seed(int last_seed = 0)
		{
			ComplexTime tm;

			static unsigned long long seed = (unsigned long long)tm.GetCurrentTime();

			// 첫번째 시드
			ComplexTimeTable table1 = tm.GetCurrentTime() - 2847172;
			unsigned long long firstSeed = (unsigned long long)table1;

			// 두번째 시드
			unsigned long long secondSeed = (firstSeed % seed) * seed;

			// 세번째 시드
			ComplexTimeTable table2 = tm.GetCurrentTime() - 5691928;
			unsigned long long d = (unsigned long long)table2;
			unsigned long long thirdSeed = (d - firstSeed) * d;

			// 네번째 시드
			unsigned long long fourthSeed = abs(int(firstSeed * secondSeed / thirdSeed));

			seed = firstSeed * seed + secondSeed / thirdSeed * fourthSeed;

			// 마지막 시드
			unsigned long long lastSeed = (unsigned long long)last_seed;
			if (last_seed == 0)
				lastSeed = seed % fourthSeed;

			return int(seed % lastSeed);
		}

	};
}