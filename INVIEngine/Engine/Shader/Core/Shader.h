#pragma once

class FShader
{
public:
	LPVOID GetBufferPointer() const;
	SIZE_T GetBufferSize() const;

	// ����shader��shader�ļ�·����shader����㣨main����������shader�汾��
	void BuildShader(const std::wstring& InFileName, const std::string& InEntryFunName, const std::string& InShaderVersion, const D3D_SHADER_MACRO* shaderMacro = nullptr);

private:
	ComPtr<ID3DBlob> ShaderCode;
};

