#pragma once
#include "../../commonInclude.h"


class TimelineVO
{
public:

	TimelineVO()
		: timeIDX(0)
		, notSEQ(0)
		, sceSEQ(0)
	{

	}

	TimelineVO(const TimelineVO& vo)
	{
		timeIDX = vo.timeIDX;
		notSEQ = vo.notSEQ;
		sceSEQ = vo.sceSEQ;
	}

	TimelineVO(int timeIDX, int notSEQ, int sceSEQ)
		: timeIDX(timeIDX)
		, notSEQ(notSEQ)
		, sceSEQ(sceSEQ)
	{

	}

	~TimelineVO()
	{

	}

	void Clear()
	{
		timeIDX = 0;
		notSEQ = 0;
		sceSEQ = 0;
	}

	void SetSceSEQ(int in)
	{
		this->sceSEQ = in;
	}

	void SetNotSEQ(int in)
	{
		this->notSEQ = in;
	}

	void SetTimeIDX(int in)
	{
		this->timeIDX = in;
	}

	int GetSceSEQ() const
	{
		return sceSEQ;
	}

	int GetNotSEQ() const
	{
		return notSEQ;
	}

	int GetTimeIDX() const
	{
		return timeIDX;
	}

private:

	int timeIDX;
	int notSEQ;
	int sceSEQ;

};