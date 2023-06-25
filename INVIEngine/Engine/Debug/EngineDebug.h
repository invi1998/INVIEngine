#pragma once

#include "EngineMinimal.h"

#define ANALYSIS_RESULT(InValue)\
{\
	if(FAILED(InValue))\
	{\
		ENGINE_LOG_ERROR("Error = %i", (int)(InValue));\
	}\
	else if (SUCCEEDED(InValue))\
	{\
		ENGINE_LOG_SUCCESS("Success !");\
	}\
}


