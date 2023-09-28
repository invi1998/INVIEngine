#pragma once

class FShader
{
public:
	LPVOID GetBufferPointer() const;
	SIZE_T GetBufferSize() const;

	// ����shader��shader�ļ�·����shader����㣨main����������shader�汾��
	void BuildShader(const std::wstring& InFileName, const std::string& InEntryFunName, const std::string& InShaderVersion);

private:
	ComPtr<ID3DBlob> ShaderCode;
};

