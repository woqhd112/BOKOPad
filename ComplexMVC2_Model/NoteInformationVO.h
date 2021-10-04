#pragma once
#include "commonInclude.h"

class NoteInformationVO
{
public:

	NoteInformationVO()
		: notSEQ(0)
		, sceSEQ(0)
		, notLOCK(false)
		, notCONTENT("")
	{
	
	}

	NoteInformationVO(const NoteInformationVO& vo)
	{
		notSEQ = vo.notSEQ;
		sceSEQ = vo.sceSEQ;
		notLOCK = vo.notLOCK;
		notCONTENT = vo.notCONTENT;
	}

	NoteInformationVO(int notSEQ, int sceSEQ, bool notLOCK, ComplexString notCONTENT)
		: notSEQ(notSEQ)
		, sceSEQ(sceSEQ)
		, notLOCK(notLOCK)
		, notCONTENT(notCONTENT)
	{

	}

	~NoteInformationVO()
	{
	
	}

	void Clear()
	{
		notSEQ = 0;
		sceSEQ = 0;
		notLOCK = false;
		notCONTENT = "";
	}

	void SetNotSEQ(int in)
	{
		this->notSEQ = in;
	}

	void SetSceSEQ(int in)
	{
		this->sceSEQ = in;
	}

	void SetNotLOCK(bool in)
	{
		this->notLOCK = in;
	}

	void SetNotCONTENT(ComplexString in)
	{
		this->notCONTENT = in;
	}

	int GetNotSEQ() const
	{
		return notSEQ;
	}

	int GetSceSEQ() const
	{
		return sceSEQ;
	}

	bool IsNotLOCK() const
	{
		return notLOCK;
	}

	ComplexString GetNotCONTENT() const
	{
		return notCONTENT;
	}

private:

	int notSEQ;
	int sceSEQ;
	bool notLOCK;
	ComplexString notCONTENT;

};
