#include "pch.h"
#include "DataScope.h"


static std::shared_ptr<RequestAttribute> g_request;
static std::shared_ptr<SessionAttribute> g_session;
static std::shared_ptr<TransactionManager> g_transaction;

RequestAttribute* GetRequestScope()
{
	if (g_request.get() == NULL)
	{
		g_request.reset(new RequestAttribute);
	}

	return g_request.get();
}

SessionAttribute* GetSessionScope()
{
	if (g_session.get() == NULL)
	{
		g_session.reset(new SessionAttribute);
	}

	return g_session.get();
}

TransactionManager* GetTransactionManager()
{
	if (g_transaction.get() == NULL)
	{
		g_transaction.reset(new TransactionManager);
	}

	return g_transaction.get();
}