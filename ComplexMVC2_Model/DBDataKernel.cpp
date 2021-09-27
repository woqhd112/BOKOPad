#include "pch.h"
#include "DBDataKernel.h"

DBDataKernel::DBDataKernel()
{

}

DBDataKernel::~DBDataKernel()
{
	
}

void DBDataKernel::ResetNoteInformation()
{
	m_noteInformationVOContainer.clear();
}

void DBDataKernel::ResetScenarioList()
{
	m_scenarioListVOContainer.clear();
}

void DBDataKernel::ResetPadOption()
{
	m_padOptionVOContainer.clear();
}

void DBDataKernel::SetNoteInformation(NoteInformationVO& in)
{
	NoteInformationVO noteinfo;
	noteinfo.SetNotSEQ(in.GetNotSEQ());
	noteinfo.SetSceSEQ(in.GetSceSEQ());
	noteinfo.SetNotLOCK(in.IsNotLOCK());
	noteinfo.SetNotCONTENT(in.GetNotCONTENT());
	m_noteInformationVOContainer.push_back(noteinfo);
}

void DBDataKernel::SetScenarioList(ScenarioListVO& in)
{
	ScenarioListVO scenarioList;
	scenarioList.SetSceSEQ(in.GetSceSEQ());
	scenarioList.SetNotCNT(in.GetNotCNT());
	scenarioList.SetSceTITLE(in.GetSceTITLE());
	m_scenarioListVOContainer.push_back(scenarioList);
}

void DBDataKernel::SetPadOption(PadOptionVO& in)
{
	PadOptionVO padOption;
	padOption.SetTest1(in.GetTest1());
	padOption.SetTest2(in.GetTest2());
	m_padOptionVOContainer.push_back(padOption);
}

void DBDataKernel::SetNoteInformation(int notSEQ, int sceSEQ, bool notLOCK, ComplexString notCONTENT)
{
	NoteInformationVO noteinfo;
	noteinfo.SetNotSEQ(notSEQ);
	noteinfo.SetSceSEQ(sceSEQ);
	noteinfo.SetNotLOCK(notLOCK);
	noteinfo.SetNotCONTENT(notCONTENT);
	m_noteInformationVOContainer.push_back(noteinfo);
}

void DBDataKernel::SetScenarioList(int sceSEQ, ComplexString sceTITLE, int notCNT)
{
	ScenarioListVO scenarioList;
	scenarioList.SetSceSEQ(sceSEQ);
	scenarioList.SetNotCNT(notCNT);
	scenarioList.SetSceTITLE(sceTITLE);
	m_scenarioListVOContainer.push_back(scenarioList);
}

void DBDataKernel::SetPadOption(int test1, int test2)
{
	PadOptionVO padOption;
	padOption.SetTest1(test1);
	padOption.SetTest2(test2);
	m_padOptionVOContainer.push_back(padOption);
}

void DBDataKernel::GetNoteInformation(NoteInformationVO* out, int get_row_index)
{
	if (m_noteInformationVOContainer.empty())
		return;

	if (out)
	{
		out->SetNotSEQ(m_noteInformationVOContainer.at(get_row_index).GetNotSEQ());
		out->SetSceSEQ(m_noteInformationVOContainer.at(get_row_index).GetSceSEQ());
		out->SetNotLOCK(m_noteInformationVOContainer.at(get_row_index).IsNotLOCK());
		out->SetNotCONTENT(m_noteInformationVOContainer.at(get_row_index).GetNotCONTENT());
	}
	//ResetNoteInformation();
}

void DBDataKernel::GetScenarioList(ScenarioListVO* out, int get_row_index)
{
	if (m_scenarioListVOContainer.empty())
		return;

	if (out)
	{
		out->SetSceSEQ(m_scenarioListVOContainer.at(get_row_index).GetSceSEQ());
		out->SetNotCNT(m_scenarioListVOContainer.at(get_row_index).GetNotCNT());
		out->SetSceTITLE(m_scenarioListVOContainer.at(get_row_index).GetSceTITLE());
	}
	//ResetScenarioList();
}

void DBDataKernel::GetPadOption(PadOptionVO* out, int get_row_index)
{
	if (m_padOptionVOContainer.empty())
		return;

	if (out)
	{
		out->SetTest1(m_padOptionVOContainer.at(get_row_index).GetTest1());
		out->SetTest2(m_padOptionVOContainer.at(get_row_index).GetTest2());
	}
	//ResetPadOption();
}

void DBDataKernel::GetNoteInformation(int* out_notSEQ, int* out_sceSEQ, bool* out_notLOCK, ComplexString* out_notCONTENT, int get_row_index)
{
	if (m_noteInformationVOContainer.empty())
		return;

	if (out_notSEQ)
		*out_notSEQ = m_noteInformationVOContainer.at(get_row_index).GetNotSEQ();
	if (out_sceSEQ)
		*out_sceSEQ = m_noteInformationVOContainer.at(get_row_index).GetSceSEQ();
	if (out_notLOCK)
		*out_notLOCK = m_noteInformationVOContainer.at(get_row_index).IsNotLOCK();
	if (out_notCONTENT)
		*out_notCONTENT = m_noteInformationVOContainer.at(get_row_index).GetNotCONTENT();
	//ResetNoteInformation();
}

void DBDataKernel::GetScenarioList(int* out_sceSEQ, int* out_notCNT, ComplexString* out_sceTITLE, int get_row_index)
{
	if (m_scenarioListVOContainer.empty())
		return;

	if (out_sceSEQ)
		*out_sceSEQ = m_scenarioListVOContainer.at(get_row_index).GetSceSEQ();
	if (out_notCNT)
		*out_notCNT = m_scenarioListVOContainer.at(get_row_index).GetNotCNT();
	if (out_sceTITLE)
		*out_sceTITLE = m_scenarioListVOContainer.at(get_row_index).GetSceTITLE();
	//ResetScenarioList();
}

void DBDataKernel::GetPadOption(int* out_test1, int* out_test2, int get_row_index)
{
	if (m_padOptionVOContainer.empty())
		return;

	if (out_test1)
		*out_test1 = m_padOptionVOContainer.at(get_row_index).GetTest1();
	if (out_test2)
		*out_test2 = m_padOptionVOContainer.at(get_row_index).GetTest2();
	//ResetPadOption();
}

void DBDataKernel::GetNoteInformationContainer(ComplexVector<NoteInformationVO>* out_noteInformationVOContainer)
{
	if (out_noteInformationVOContainer)
	{
		ComplexVector<NoteInformationVO>::iterator iter = m_noteInformationVOContainer.begin();

		while (iter != m_noteInformationVOContainer.end())
		{
			NoteInformationVO in_noteInformation;
			in_noteInformation.SetNotSEQ(iter->value.GetNotSEQ());
			in_noteInformation.SetSceSEQ(iter->value.GetSceSEQ());
			in_noteInformation.SetNotLOCK(iter->value.IsNotLOCK());
			in_noteInformation.SetNotCONTENT(iter->value.GetNotCONTENT());
			out_noteInformationVOContainer->push_back(in_noteInformation);
			iter++;
		}
	}
	//ResetNoteInformation();
}

void DBDataKernel::GetScenarioListContainer(ComplexVector<ScenarioListVO>* out_scenarioListVOContainer)
{
	if (out_scenarioListVOContainer)
	{
		ComplexVector<ScenarioListVO>::iterator iter = m_scenarioListVOContainer.begin();

		while (iter != m_scenarioListVOContainer.end())
		{
			ScenarioListVO in_scenarioList;
			in_scenarioList.SetNotCNT(iter->value.GetNotCNT());
			in_scenarioList.SetSceSEQ(iter->value.GetSceSEQ());
			in_scenarioList.SetSceTITLE(iter->value.GetSceTITLE());
			out_scenarioListVOContainer->push_back(in_scenarioList);
			iter++;
		}
	}
	//ResetScenarioList();
}

void DBDataKernel::GetPadOptionContainer(ComplexVector<PadOptionVO>* out_padOptionVOContainer)
{
	if (out_padOptionVOContainer)
	{
		ComplexVector<PadOptionVO>::iterator iter = m_padOptionVOContainer.begin();

		while (iter != m_padOptionVOContainer.end())
		{
			PadOptionVO in_padOption;
			in_padOption.SetTest1(iter->value.GetTest1());
			in_padOption.SetTest2(iter->value.GetTest2());
			out_padOptionVOContainer->push_back(in_padOption);
			iter++;
		}
	}
	//ResetPadOption();
}