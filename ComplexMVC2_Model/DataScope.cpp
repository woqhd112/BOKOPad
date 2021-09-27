#include "pch.h"
#include "DataScope.h"


static std::shared_ptr<RequestAttribute> g_request;
static std::shared_ptr<SessionAttribute> g_session;

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
