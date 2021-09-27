#pragma once
#include "DBDataKernel.h"


class COMPLEXMVC2_MODEL_DLL SessionAttribute
{
public:

	SessionAttribute()
	{
		m_kernel = new DBDataKernel;
	}

	~SessionAttribute()
	{
		if (m_kernel)
		{
			delete m_kernel;
			m_kernel = nullptr;
		}
	}

	void SetSession(NoteInformationVO& in)
	{
		m_kernel->SetNoteInformation(in);
	}

	void SetSession(ScenarioListVO& in)
	{
		m_kernel->SetScenarioList(in);
	}

	void SetSession(PadOptionVO& in)
	{
		m_kernel->SetPadOption(in);
	}

	void SetSession(ComplexVector<NoteInformationVO>& in)
	{
		ComplexVector<NoteInformationVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetNoteInformation(iter->value);
			iter++;
		}
	}

	void SetSession(ComplexVector<ScenarioListVO>& in)
	{
		ComplexVector<ScenarioListVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetScenarioList(iter->value);
			iter++;
		}
	}

	void SetSession(ComplexVector<PadOptionVO>& in)
	{
		ComplexVector<PadOptionVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetPadOption(iter->value);
			iter++;
		}
	}

	void GetSession(NoteInformationVO* out, int get_row_index = 0)
	{
		m_kernel->GetNoteInformation(out, get_row_index);
	}

	void GetSession(ScenarioListVO* out, int get_row_index = 0)
	{
		m_kernel->GetScenarioList(out, get_row_index);
	}

	void GetSession(PadOptionVO* out, int get_row_index = 0)
	{
		m_kernel->GetPadOption(out, get_row_index);
	}

	void GetSession(ComplexVector<NoteInformationVO>* out)
	{
		m_kernel->GetNoteInformationContainer(out);
	}

	void GetSession(ComplexVector<ScenarioListVO>* out)
	{
		m_kernel->GetScenarioListContainer(out);
	}

	void GetSession(ComplexVector<PadOptionVO>* out)
	{
		m_kernel->GetPadOptionContainer(out);
	}

	void ResetSession()
	{
		m_kernel->ResetNoteInformation();
		m_kernel->ResetScenarioList();
		m_kernel->ResetPadOption();
	}

private:

	DBDataKernel* m_kernel;

};

