#include "pch.h"
#include "TransactionProcess.h"
#include "..\Model\DlgModel.h"

TransactionProcess::TransactionProcess()
{
	m_savePointCount = 0;
}

TransactionProcess::~TransactionProcess()
{

}

bool TransactionProcess::RequestSavePoint(ComplexString strCreateSavePointName)
{
	ComplexMap<int, ComplexString>::iterator iter = m_savePointContainer.begin();
	
	while (iter != m_savePointContainer.end())
	{
		if (iter->value.value == strCreateSavePointName)
			return false;
		iter++;
	}

	DB_INSTANCE->SetSavePoint(strCreateSavePointName);
	m_savePointContainer.insert(m_savePointCount, strCreateSavePointName);
	m_savePointCount++;

	return true;
}

bool TransactionProcess::ReleaseSavePoint(ComplexString strReleaseSavePointName)
{
	ComplexMap<int, ComplexString>::iterator iter = m_savePointContainer.begin();

	bool bFind = false;
	while (iter != m_savePointContainer.end())
	{
		if (iter->value.value == strReleaseSavePointName)
		{
			bFind = true;
			break;
		}
		iter++;
	}

	if (!bFind)
		return false;

	DB_INSTANCE->ReleaseSavePoint(strReleaseSavePointName);
	m_savePointContainer.erase(iter->value.key);
	m_savePointCount--;

	return true;
}

void TransactionProcess::SetAutoCommit(bool useAutoCommit)
{
	DB_INSTANCE->SetAutoCommit(useAutoCommit);
}

void TransactionProcess::Commit()
{
	DB_INSTANCE->Commit();
}

void TransactionProcess::Rollback(ComplexString strRollbackSavePointName)
{
	DB_INSTANCE->Rollback(strRollbackSavePointName);
}

ComplexMap<int, ComplexString>& TransactionProcess::GetSavePointNameDB()
{
	return m_savePointContainer;
}
