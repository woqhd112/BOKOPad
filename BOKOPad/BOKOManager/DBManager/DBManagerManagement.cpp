#include "pch.h"
#include "DBManagerManagement.h"

DlgController* DBManagerManagement::MVC_Controller;

DBManagerManagement::DBManagerManagement()
	: m_bOperateTransaction(false)
	, m_saveTransactionName(nullptr)
{
	if (MVC_Controller == nullptr)
	{
		MVC_Controller = new DlgController;
	}

	m_inputScenarioVO.Clear();
	m_inputNoteVO.Clear();
	m_inputTimeVO.Clear();
}

DBManagerManagement::~DBManagerManagement()
{

}

void DBManagerManagement::StartTransaction(const char* transactionName)
{
	TransactionInstance->RequestSavePoint(transactionName);
	m_saveTransactionName = const_cast<char*>(transactionName);
	m_bOperateTransaction = true;
}

void DBManagerManagement::RollbackTransaction()
{
	TransactionInstance->Rollback(m_saveTransactionName);
	TransactionInstance->ReleaseSavePoint(m_saveTransactionName);
	m_bOperateTransaction = false;
	m_saveTransactionName = nullptr;
}

void DBManagerManagement::CommitTransaction()
{
	TransactionInstance->ReleaseSavePoint(m_saveTransactionName);
	TransactionInstance->Commit();
	m_bOperateTransaction = false;
	m_saveTransactionName = nullptr;
}