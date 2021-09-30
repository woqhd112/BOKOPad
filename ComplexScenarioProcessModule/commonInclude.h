#pragma once
#include "ComplexString.h"
#include "ComplexThread.h"
#include "ComplexCondition.h"

#ifdef COMPLEXSCENARIOPROCESSMODULE_EXPORTS
#define COMPLEXSCENARIOPROCESSMODULE_DLL _declspec(dllexport)
#else
#define COMPLEXSCENARIOPROCESSMODULE_DLL _declspec(dllimport)
#endif

using namespace ComplexLibrary;