#pragma once
#include "TransactionProcess.h"

class COMPLEXMVC2_MODEL_DLL TransactionManager
{
public:

	TransactionManager()
	{
		m_transaction = new TransactionProcess;
	}

	~TransactionManager()
	{
		if (m_transaction)
		{
			delete m_transaction;
			m_transaction = nullptr;
		}
	}

	bool RequestSavePoint(ComplexString strCreateSavePointName)
	{
		return m_transaction->RequestSavePoint(strCreateSavePointName);
	}

	bool ReleaseSavePoint(ComplexString strReleaseSavePointName)
	{
		return m_transaction->ReleaseSavePoint(strReleaseSavePointName);
	}

	void SetAutoCommit(bool useAutoCommit)
	{
		m_transaction->SetAutoCommit(useAutoCommit);
	}

	void Commit()
	{
		m_transaction->Commit();
	}

	void Rollback(ComplexString strRollbackSavePointName = "")
	{
		m_transaction->Rollback(strRollbackSavePointName);
	}

	ComplexMap<int, ComplexString>& GetSavePointNameDB()
	{
		return m_transaction->GetSavePointNameDB();
	}

private:

	TransactionProcess* m_transaction;

};