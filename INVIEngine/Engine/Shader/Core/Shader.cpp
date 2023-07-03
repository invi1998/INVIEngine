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
	// ���� HLSL��High-Level Shader Language�����벢�����ֽ������ݡ�
	D3DCompileFromFile(
		InFileName.c_str(),                             // �ļ���
        nullptr,                                        // �궨������
        D3D_COMPILE_STANDARD_FILE_INCLUDE,              // �Զ��� include �ļ��ӿ� �������shader������Ҫ����include�����shader�ļ�����ô���ֵ��������Ϊnull)
        InEntryFunName.c_str(),                         // ��ڵ㺯������
        InShaderVersion.c_str(),                        // Ŀ����ɫ��ģ��(shader�汾��
#if _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // ����ѡ��(��������ģʽ,�����Ż��׶Σ������Ա����Ĵ�������Ż������Ա��ڿ��ٵ��Ժ���֤��ɫ���߼�)
#else
		0,
#endif
        0,                                              // ������������Ϊ 0
        &ShaderCode,                                  // ���ر�����������
        &ErrorShaderMsg                               // ���ر��������Ϣ
	);

    if (ErrorShaderMsg)
    {
	    ENGINE_LOG_ERROR("%s������󣬱�������\n%s", InFileName.c_str(), static_cast<char*>(ErrorShaderMsg->GetBufferPointer()))
    }
}
