#pragma once
#include "RequestAttribute.h"
#include "SessionAttribute.h"
#include <memory>

COMPLEXMVC2_MODEL_DLL RequestAttribute* GetRequestScope();
COMPLEXMVC2_MODEL_DLL SessionAttribute* GetSessionScope();

#define RequestScope (GetRequestScope())
#define SessionScope (GetSessionScope())
