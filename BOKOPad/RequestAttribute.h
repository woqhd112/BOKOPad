#pragma once
#include "DBDataKernel.h"


class RequestAttribute : private DBDataKernel
{
public:

	RequestAttribute()
	{
	
	}

	virtual ~RequestAttribute()
	{
	
	}

	void SetRequestAttributes(NoteInformationVO& in)
	{
		SetNoteInformation(in);
	}

	void SetRequestAttributes(ScenarioListVO& in)
	{
		SetScenarioList(in);
	}

	void SetRequestAttributes(PadOptionVO& in)
	{
		SetPadOption(in);
	}

	void SetRequestAttributes(ComplexVector<NoteInformationVO>& in)
	{
		ComplexVector<NoteInformationVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetNoteInformation(iter->value);
			iter++;
		}
	}

	void SetRequestAttributes(ComplexVector<ScenarioListVO>& in)
	{
		ComplexVector<ScenarioListVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetScenarioList(iter->value);
			iter++;
		}
	}

	void SetRequestAttributes(ComplexVector<PadOptionVO>& in)
	{
		ComplexVector<PadOptionVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetPadOption(iter->value);
			iter++;
		}
	}

	void GetRequestAttributes(NoteInformationVO* out, int get_row_index = 0)
	{
		GetNoteInformation(out, get_row_index);
		ResetNoteInformation();
	}

	void GetRequestAttributes(ScenarioListVO* out, int get_row_index = 0)
	{
		GetScenarioList(out, get_row_index);
		ResetScenarioList();
	}

	void GetRequestAttributes(PadOptionVO* out, int get_row_index = 0)
	{
		GetPadOption(out, get_row_index);
		ResetPadOption();
	}

	void GetRequestAttributes(ComplexVector<NoteInformationVO>* out)
	{
		GetNoteInformationContainer(out);
		ResetNoteInformation();
	}

	void GetRequestAttributes(ComplexVector<ScenarioListVO>* out)
	{
		GetScenarioListContainer(out);
		ResetScenarioList();
	}

	void GetRequestAttributes(ComplexVector<PadOptionVO>* out)
	{
		GetPadOptionContainer(out);
		ResetPadOption();
	}

private:


};

