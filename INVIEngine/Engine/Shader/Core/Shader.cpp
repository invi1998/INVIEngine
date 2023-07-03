#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
    return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
    return ShaderCode->GetBufferSize();
}

void FShader::BuildShader(const std::wstring& InFileName, const std::string& InEntryFunName,
                          const std::string& InShaderVersion)
{
    ComPtr<ID3DBlob> ErrorShaderMsg;
	// 编译 HLSL（High-Level Shader Language）代码并生成字节码数据。
	D3DCompileFromFile(
		InFileName.c_str(),                             // 文件名
        nullptr,                                        // 宏定义数组
        D3D_COMPILE_STANDARD_FILE_INCLUDE,              // 自定义 include 文件接口 （如果在shader里面需要包含include另外的shader文件，那么这个值不能设置为null)
        InEntryFunName.c_str(),                         // 入口点函数名称
        InShaderVersion.c_str(),                        // 目标着色器模型(shader版本）
#if _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // 编译选项(开启调试模式,跳过优化阶段，即不对编译后的代码进行优化处理，以便于快速调试和验证着色器逻辑)
#else
		0,
#endif
        0,                                              // 保留参数，设为 0
        &ShaderCode,                                  // 返回编译后代码数据
        &ErrorShaderMsg                               // 返回编译错误信息
	);

    if (ErrorShaderMsg)
    {
	    ENGINE_LOG_ERROR("%s编译错误，报错如下\n%s", InFileName.c_str(), static_cast<char*>(ErrorShaderMsg->GetBufferPointer()))
    }
}
