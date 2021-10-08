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

	void SetSessionInt(int in)
	{
		m_kernel->SetInt(in);
	}

	void SetSessionString(ComplexString in)
	{
		m_kernel->SetString(in);
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

	void SetSession(TimelineVO& in)
	{
		m_kernel->SetTimeline(in);
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

	void SetSession(ComplexVector<TimelineVO>& in)
	{
		ComplexVector<TimelineVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetTimeline(iter->value);
			iter++;
		}
	}

	void GetSessionInt(int* out)
	{
		m_kernel->GetInt(out);
	}

	void GetSessionString(ComplexString* out)
	{
		m_kernel->GetString(out);
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

	void GetSession(TimelineVO* out, int get_row_index = 0)
	{
		m_kernel->GetTimeline(out, get_row_index);
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

	void GetSession(ComplexVector<TimelineVO>* out)
	{
		m_kernel->GetTimelineContainer(out);
		m_kernel->ResetTimeline();
	}

	void ResetSession()
	{
		m_kernel->ResetInt();
		m_kernel->ResetString();
		m_kernel->ResetNoteInformation();
		m_kernel->ResetScenarioList();
		m_kernel->ResetPadOption();
		m_kernel->ResetTimeline();
	}

private:

	DBDataKernel* m_kernel;

};

