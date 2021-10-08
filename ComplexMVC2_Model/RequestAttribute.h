#pragma once
#include "DBDataKernel.h"


class COMPLEXMVC2_MODEL_DLL RequestAttribute
{
public:

	RequestAttribute()
	{
		m_kernel = new DBDataKernel;
	}

	~RequestAttribute()
	{
		if (m_kernel)
		{
			delete m_kernel;
			m_kernel = nullptr;
		}
	}

	void SetRequestInt(int in)
	{
		m_kernel->SetInt(in);
	}

	void SetRequestString(ComplexString in)
	{
		m_kernel->SetString(in);
	}

	void SetRequestAttributes(NoteInformationVO& in)
	{
		m_kernel->SetNoteInformation(in);
	}

	void SetRequestAttributes(ScenarioListVO& in)
	{
		m_kernel->SetScenarioList(in);
	}

	void SetRequestAttributes(PadOptionVO& in)
	{
		m_kernel->SetPadOption(in);
	}

	void SetRequestAttributes(TimelineVO& in)
	{
		m_kernel->SetTimeline(in);
	}

	void SetRequestAttributes(ComplexVector<NoteInformationVO>& in)
	{
		ComplexVector<NoteInformationVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetNoteInformation(iter->value);
			iter++;
		}
	}

	void SetRequestAttributes(ComplexVector<ScenarioListVO>& in)
	{
		ComplexVector<ScenarioListVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetScenarioList(iter->value);
			iter++;
		}
	}

	void SetRequestAttributes(ComplexVector<PadOptionVO>& in)
	{
		ComplexVector<PadOptionVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetPadOption(iter->value);
			iter++;
		}
	}

	void SetRequestAttributes(ComplexVector<TimelineVO>& in)
	{
		ComplexVector<TimelineVO>::iterator iter = in.begin();
		while (iter != in.end())
		{
			m_kernel->SetTimeline(iter->value);
			iter++;
		}
	}

	void GetRequestInt(int* out)
	{
		m_kernel->GetInt(out);
		m_kernel->ResetInt();
	}

	void GetRequestString(ComplexString* out)
	{
		m_kernel->GetString(out);
		m_kernel->ResetString();
	}

	void GetRequestAttributes(NoteInformationVO* out, int get_row_index = 0)
	{
		m_kernel->GetNoteInformation(out, get_row_index);
		m_kernel->ResetNoteInformation();
	}

	void GetRequestAttributes(ScenarioListVO* out, int get_row_index = 0)
	{
		m_kernel->GetScenarioList(out, get_row_index);
		m_kernel->ResetScenarioList();
	}

	void GetRequestAttributes(PadOptionVO* out, int get_row_index = 0)
	{
		m_kernel->GetPadOption(out, get_row_index);
		m_kernel->ResetPadOption();
	}

	void GetRequestAttributes(TimelineVO* out, int get_row_index = 0)
	{
		m_kernel->GetTimeline(out, get_row_index);
		m_kernel->ResetTimeline();
	}

	void GetRequestAttributes(ComplexVector<NoteInformationVO>* out)
	{
		m_kernel->GetNoteInformationContainer(out);
		m_kernel->ResetNoteInformation();
	}

	void GetRequestAttributes(ComplexVector<ScenarioListVO>* out)
	{
		m_kernel->GetScenarioListContainer(out);
		m_kernel->ResetScenarioList();
	}

	void GetRequestAttributes(ComplexVector<PadOptionVO>* out)
	{
		m_kernel->GetPadOptionContainer(out);
		m_kernel->ResetPadOption();
	}

	void GetRequestAttributes(ComplexVector<TimelineVO>* out)
	{
		m_kernel->GetTimelineContainer(out);
		m_kernel->ResetTimeline();
	}

private:

	DBDataKernel* m_kernel;

};

