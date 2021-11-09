#pragma once
#include "RequestAttribute.h"
#include "SessionAttribute.h"
#include "..\..\Transaction\TransactionManager.h"
#include <memory>

COMPLEXMVC2_MODEL_DLL RequestAttribute* GetRequestScope();
COMPLEXMVC2_MODEL_DLL SessionAttribute* GetSessionScope();
COMPLEXMVC2_MODEL_DLL TransactionManager* GetTransactionManager();

#define RequestScope (GetRequestScope())
#define SessionScope (GetSessionScope())
#define TransactionInstance (GetTransactionManager())
