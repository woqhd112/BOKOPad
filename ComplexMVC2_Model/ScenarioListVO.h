#pragma once
#include "commonInclude.h"


class ScenarioListVO
{
public:

	ScenarioListVO()
		: sceSEQ(0)
		, notCNT(0)
		, sceTITLE("")
	{
	
	}

	ScenarioListVO(int sceSEQ, int notCNT, ComplexString sceTITLE)
		: sceSEQ(sceSEQ)
		, notCNT(notCNT)
		, sceTITLE(sceTITLE)
	{

	}

	~ScenarioListVO()
	{
	
	}

	void Clear()
	{
		sceSEQ = 0;
		notCNT = 0;
		sceTITLE = "";
	}

	void SetSceSEQ(int in)
	{
		this->sceSEQ = in;
	}

	void SetNotCNT(int in)
	{
		this->notCNT = in;
	}

	void SetSceTITLE(ComplexString in)
	{
		this->sceTITLE = in;
	}

	int GetSceSEQ() const
	{
		return sceSEQ;
	}

	int GetNotCNT() const
	{
		return notCNT;
	}

	ComplexString GetSceTITLE() const
	{
		return sceTITLE;
	}

private:

	int sceSEQ;
	int notCNT;
	ComplexString sceTITLE;

};