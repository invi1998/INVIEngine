#include "EngineMinimal.h"
#include "SelectRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FSelectRenderLayer::FSelectRenderLayer()
{
	RenderPriority = 3450;

	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_SELECT;
}

void FSelectRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/Select.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/Select.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// ��shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// ���벼��
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// ��ɫ���������ƫ����12�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ�������Ե�����ɫ������� 3*4 = 12�ֽڵ�ƫ����
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	 // ���ߣ��������ƫ����28�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ����Ե��˷�������͵�ƫ�� ��3+4��*4 = 28�ֽڵ�ƫ����
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // ���ߣ�U���ߣ����������ƫ����32�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ��ټ���3������Ԫ�أ�������������͵�ƫ�� ��3+4+3��*4 = 40�ֽڵ�ƫ����
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv���꣬�������ƫ����52�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ��ټ���3������Ԫ�أ��ټ���3������Ԫ�أ�����uv����͵�ƫ�� ��3+4+3+3��*4 = 52�ֽڵ�ƫ����

	};

	// �����벼��
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FSelectRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	// ͸�����
	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;
	RenderTargetBlendDesc.BlendEnable = true;		// ����ͼ����
	RenderTargetBlendDesc.LogicOpEnable = false;	// �ر��߼���ϣ�������ϱ��뽫��ֵ�رգ���Ϊ�����ǻ���ģ�

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	// ����Դ��ɫ�ͻ�Ϻ���ɫ֮��ĳ˷���ʽ������Ϊ��ֵ��ʾ ��Դ��ɫ����Դ���ص� alpha ֵ��
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// ��ʾĿ����ɫ�ͻ�Ϻ���ɫ֮��ĳ˷���ʽ������Ϊ��ֵ��ʾ ��Ŀ�����ص���ɫֵ����ΪԴ���ص�͸����ֵȡ������ 1 - Դ���ص�͸����ֵ����

	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// ��ʾ��ϲ��������ͣ��ӷ����������������������Ϊ��ֵ��ʾ ��Դ��ɫ��Ŀ����ɫ��ӡ�
	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ��ʾԴ͸���Ⱥͻ�Ϻ�͸����֮��ĳ˷���ʽ������Ϊ��ֵ��ʾ ��Դ���ص�͸����ֵ����Ϊ 1�� F(1,1,1)
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// ��ʾĿ��͸���Ⱥͻ�Ϻ�͸����֮��ĳ˷���ʽ������Ϊ��ֵ��ʾ ��Դ���ص�͸����ֵ����Ϊ 0��F(0,0,0)
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ��ʾ͸���Ȼ�ϲ��������ͣ��ӷ����������������,����Ϊ��ֵ��ʾ ��Դ��ɫ��Ŀ����ɫ���

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;		// ��ʾ�߼���������ͣ��롢�����ȣ�,����Ϊ��ֵ��ʾ ���������ɫ�����κδ���
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// ��ʾ��Ҫд�뵱ǰ��ȾĿ�����Щ��ɫͨ��������Ϊ��ֵ��ʾ ��������ͨ����

	// ������ȾĿ��
	DirectXPipelineState->SetRenderTarget(0, RenderTargetBlendDesc);

	// �����ڵ��޳�
	CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	DirectXPipelineState->SetDepthStencilState(DepthStencilDesc);

	DirectXPipelineState->BuildPipelineState(EPipelineState::Selection);
}

void FSelectRenderLayer::Draw(float deltaTime)
{
	ResetPSO();

	FRenderLayer::Draw(deltaTime);
}

void FSelectRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(EPipelineState::Selection);
}
