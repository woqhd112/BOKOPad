#pragma once
#include "DBDataKernel.h"


class SessionAttribute : private DBDataKernel
{
public:

	SessionAttribute()
	{
	
	}

	virtual ~SessionAttribute()
	{
	
	}

	void SetSession(NoteInformationVO& in)
	{
		SetNoteInformation(in);
	}

	void SetSession(ScenarioListVO& in)
	{
		SetScenarioList(in);
	}

	void SetSession(PadOptionVO& in)
	{
		SetPadOption(in);
	}

	void SetSession(ComplexVector<NoteInformationVO>& in)
	{
		ComplexVector<NoteInformationVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetNoteInformation(iter->value);
			iter++;
		}
	}

	void SetSession(ComplexVector<ScenarioListVO>& in)
	{
		ComplexVector<ScenarioListVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetScenarioList(iter->value);
			iter++;
		}
	}

	void SetSession(ComplexVector<PadOptionVO>& in)
	{
		ComplexVector<PadOptionVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			SetPadOption(iter->value);
			iter++;
		}
	}

	void GetSession(NoteInformationVO* out, int get_row_index = 0)
	{
		GetNoteInformation(out, get_row_index);
	}

	void GetSession(ScenarioListVO* out, int get_row_index = 0)
	{
		GetScenarioList(out, get_row_index);
	}

	void GetSession(PadOptionVO* out, int get_row_index = 0)
	{
		GetPadOption(out, get_row_index);
	}

	void GetSession(ComplexVector<NoteInformationVO>* out)
	{
		GetNoteInformationContainer(out);
	}

	void GetSession(ComplexVector<ScenarioListVO>* out)
	{
		GetScenarioListContainer(out);
	}

	void GetSession(ComplexVector<PadOptionVO>* out)
	{
		GetPadOptionContainer(out);
	}

	void ResetSession()
	{
		ResetNoteInformation();
		ResetScenarioList();
		ResetPadOption();
	}

private:


};

