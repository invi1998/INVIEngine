#pragma once
#include "CodeReflection/CppSourceCodeType.h"

namespace Intermdiate
{
	bool Builder(const FClassAnalysis& ClassAnalysis, std::vector<std::string>& OutAnalysisRawH, std::vector<std::string>& OutAnalysisRawCpp);
}

