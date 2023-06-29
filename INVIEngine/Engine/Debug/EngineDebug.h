#pragma once

#include "EngineMinimal.h"

#define ANALYSIS_RESULT(InValue)\
{\
	HRESULT HandleResult = InValue;\
	if(FAILED(HandleResult))\
	{\
		ENGINE_LOG_ERROR("Error = %i", (int)(HandleResult));\
		assert(0);\
	}\
	else if (SUCCEEDED(HandleResult))\
	{\
		ENGINE_LOG_SUCCESS("Success !");\
	}\
}


