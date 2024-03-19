#include "EngineMinimal.h"
#include "SSAORenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FSSAORenderLayer::FSSAORenderLayer()
{
	RenderPriority = 99744;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_SSAO;
}

void FSSAORenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/AO/SSAO.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/AO/SSAO.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// ��shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// ���벼��(�������ǲ���Ҫ���벼�֣���Ϊ���ǲ���Ҫ���붥����Ϣ������ֻ��Ҫ��Ļ�ռ���������꣬����Щ��Ϣ�Ѿ���GBuffer����)
	InputElementDesc.clear();

	// �����벼��
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FSSAORenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();	// ͨ������Ĵ��������Ѿ��ɹ����˸�ǩ��

	auto psoDesc = DirectXPipelineState->GetGPSDesc();

	psoDesc.DepthStencilState.DepthEnable = false;	// �ر���Ȳ��ԣ���Ϊ�����Ϣ�Ѿ���GBuffer���ˣ��Ѿ�������֮ǰ�ķ�����ϢNormalBuffer�ṩ�ˣ�
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// ���д����������Ϊ0����д�������Ϣ
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16_UNORM;	// ������ȾĿ���ʽ
	psoDesc.SampleDesc.Count = 1;	// ���ò�������
	psoDesc.SampleDesc.Quality = 0;	// ���ò�������

	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;	// �������ģ���ʽ(δ֪����Ϊ���ǲ���Ҫ���ģ��)

	// ������Ⱦ����
	DirectXPipelineState->BuildPipelineState(SSA0);
}

void FSSAORenderLayer::Draw(float deltaTime)
{
	// ��Ⱦ֮ǰ������PSO
	ResetPSO();

	// ���ﲻ��Ҫִ�л���Ļ��Ʋ�������Ϊ���ǲ���Ҫ���붥����Ϣ������ֻ��Ҫ��Ļ�ռ���������꣬����Щ��Ϣ�Ѿ���GBuffer���ˣ���������ִ���Զ���Ļ��Ʋ���
	GetD3dGraphicsCommandList()->IASetIndexBuffer(nullptr);	// ��������������
	GetD3dGraphicsCommandList()->IASetVertexBuffers(0, 0, nullptr);	// ���ö��㻺����
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// ����ͼԪ��������
	GetD3dGraphicsCommandList()->DrawInstanced(6, 1, 0, 0);	// ִ�л��Ʋ���

}

void FSSAORenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(SSA0);
}
