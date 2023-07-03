#pragma once

#include "EngineMinimal.h"

class FShader
{
public:
	// 构建shader，shader文件路径，shader进入点（main函数名），shader版本。
	void BuildShader(const std::wstring& InFileName, const std::string& InEntryFunName, const std::string& InShaderVersion);

private:
	ComPtr<ID3DBlob> ShaderCode;
};

