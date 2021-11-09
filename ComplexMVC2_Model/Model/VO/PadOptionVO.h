#pragma once
#include "../../commonInclude.h"

class PadOptionVO
{
public:

	PadOptionVO()
		: test1(0)
		, test2(0)
	{
	
	}

	PadOptionVO(int test1, int test2)
		: test1(test1)
		, test2(test2)
	{

	}

	PadOptionVO(const PadOptionVO& vo)
	{
		test1 = vo.test1;
		test2 = vo.test2;
	}

	~PadOptionVO()
	{
	
	}

	void Clear()
	{
		test1 = 0;
		test2 = 0;
	}

	void SetTest1(int in)
	{
		test1 = in;
	}

	void SetTest2(int in)
	{
		test2 = in;
	}

	int GetTest1() const
	{
		return test1;
	}

	int GetTest2() const
	{
		return test2;
	}

private:

	int test1;
	int test2;

};
