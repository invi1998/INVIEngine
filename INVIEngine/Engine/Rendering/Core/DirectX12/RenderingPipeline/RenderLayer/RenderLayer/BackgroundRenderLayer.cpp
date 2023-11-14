#include "EngineMinimal.h"
#include "BackgroundRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	RenderPriority = 1000;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_BACKGROUND;
}

FBackgroundRenderLayer::~FBackgroundRenderLayer()
{
}

int FBackgroundRenderLayer::GetRenderLayerType() const
{
	return RenderLayerType;
}

void FBackgroundRenderLayer::PreDraw(float deltaTime)
{
	FRenderLayer::PreDraw(deltaTime);
}

void FBackgroundRenderLayer::Draw(float deltaTime)
{
	DirectXPipelineState->ResetPSO(Background);

	FRenderLayer::Draw(deltaTime);
}

void FBackgroundRenderLayer::PostDraw(float deltaTime)
{
	FRenderLayer::PostDraw(deltaTime);
}

void FBackgroundRenderLayer::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	FRenderLayer::Init(geometry, directXPipelineState);
}

void FBackgroundRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/Sky.hlsl", "VertexShaderMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/Sky.hlsl", "PixelShaderMain", "ps_5_1", D3dShaderMacro.data());
	// ��shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// ���벼��
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	 // ���ߣ��������ƫ����12�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ���Ե��˷�������͵�ƫ�� ��3��*4 = 12�ֽڵ�ƫ����
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv���꣬�������ƫ����24�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, �ټ���3������Ԫ�أ�����uv����͵�ƫ�� ��3+3��*4 = 24�ֽڵ�ƫ����
	};

	// �����벼��
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FBackgroundRenderLayer::UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FRenderLayer::UpdateCaculations(DeltaTime, ViewportInfo);
}

void FBackgroundRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;		// ��պв�ϣ�����òü�
	DirectXPipelineState->SetRasterizerState(rasterizerDesc);

	CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;	// �������ģ�����ΪС�ڵ���
	DirectXPipelineState->SetDepthStencilState(depthStencilDesc);


	DirectXPipelineState->BuildPipelineState(Background);
}
