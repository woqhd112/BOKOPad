#pragma once
#include "commonInclude.h"

class NoteInformationVO
{
public:

	NoteInformationVO()
		: notSEQ(0)
		, sceSEQ(0)
		, setTIMELINE(false)
		, notLOCK(false)
		, notCONTENT("")
	{
	
	}

	NoteInformationVO(const NoteInformationVO& vo)
	{
		notSEQ = vo.notSEQ;
		sceSEQ = vo.sceSEQ;
		setTIMELINE = vo.setTIMELINE;
		notLOCK = vo.notLOCK;
		notCONTENT = vo.notCONTENT;
	}

	NoteInformationVO(int notSEQ, int sceSEQ, bool setTIMELINE, bool notLOCK, ComplexString notCONTENT)
		: notSEQ(notSEQ)
		, sceSEQ(sceSEQ)
		, setTIMELINE(setTIMELINE)
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
		setTIMELINE = false;
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

	void SetSetTIMELINE(bool in)
	{
		this->setTIMELINE = in;
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

	bool IsSetTIMELINE() const
	{
		return setTIMELINE;
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
	bool setTIMELINE;
	bool notLOCK;
	ComplexString notCONTENT;

};
