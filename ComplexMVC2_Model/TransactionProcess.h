#pragma once
#include "commonInclude.h"

class TransactionProcess
{
public:

	TransactionProcess();

	~TransactionProcess();

	bool RequestSavePoint(ComplexString strCreateSavePointName);

	bool ReleaseSavePoint(ComplexString strReleaseSavePointName);

	void SetAutoCommit(bool useAutoCommit);

	void Commit();

	void Rollback(ComplexString strRollbackSavePointName = "");

	ComplexMap<int, ComplexString>& GetSavePointNameDB();

private:

	int m_savePointCount;
	ComplexMap<int, ComplexString> m_savePointContainer;
};

