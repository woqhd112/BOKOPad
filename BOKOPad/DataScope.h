#pragma once
#include "RequestAttribute.h"
#include "SessionAttribute.h"

static RequestAttribute* RequestScope = ComplexSingleton<RequestAttribute>::GetInstancePtr();
static SessionAttribute* SessionScope = ComplexSingleton<SessionAttribute>::GetInstancePtr();
