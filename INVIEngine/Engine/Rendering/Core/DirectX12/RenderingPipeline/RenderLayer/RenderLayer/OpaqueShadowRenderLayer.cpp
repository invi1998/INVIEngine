#include "EngineMinimal.h"
#include "OpaqueShadowRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
	RenderPriority = 1280;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_OPAQUE_SHADOW;
}

FOpaqueShadowRenderLayer::~FOpaqueShadowRenderLayer()
{
}

void FOpaqueShadowRenderLayer::PreDraw(float deltaTime)
{
	FRenderLayer::PreDraw(deltaTime);
}

void FOpaqueShadowRenderLayer::Draw(float deltaTime)
{
	DirectXPipelineState->ResetPSO();

	FRenderLayer::Draw(deltaTime);
}

void FOpaqueShadowRenderLayer::PostDraw(float deltaTime)
{
	FRenderLayer::PostDraw(deltaTime);
}

void FOpaqueShadowRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/Shadow.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/Shadow.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// 绑定shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// 输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv坐标，这里这个偏移是52字节，因为我们上面位置是3个元素，每个元素是4字节（32位），到了uv这里，就得偏移 （3）*4 = 12字节的偏移了

	};

	// 绑定输入布局
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GPSDesc = DirectXPipelineState->GetGPSDesc();

	// 偏移补偿 为了解决阴影失真问题
	// d = 1 / pow(2, 24);  2^24
	// b（偏移量） = DepthBias（固定偏移量） * d + SlopeScaledDepthBias（缩放因子） * MaxDepthSlope（最大深度）
	GPSDesc.RasterizerState.DepthBias = 100000;				// 斜率 固定偏移量
	GPSDesc.RasterizerState.DepthBiasClamp = 0.0f;			// 缩放	偏移量的限制，偏移量上限，最大偏移量
	GPSDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;	// 偏移量 根据多边形斜率来控制的一个缩放因子

	// 关闭RenderTarget（将格式设置为位置，然后renderTarget数量设置为0）
	GPSDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	GPSDesc.NumRenderTargets = 0;

	DirectXPipelineState->BuildPipelineState(EPipelineState::OrthographicShadow);		// 构建正交阴影pso

	GPSDesc.RasterizerState.DepthBias = 100;				// 斜率 固定偏移量
	DirectXPipelineState->BuildPipelineState(EPipelineState::PerspectiveShadow);		// 构建透视阴影pso
	
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(OrthographicShadow);
}

void FOpaqueShadowRenderLayer::ResetPSO(EPipelineState ps)
{
	FRenderLayer::ResetPSO(ps);

	DirectXPipelineState->ResetPSO(ps);
}

void FOpaqueShadowRenderLayer::DrawMesh(float DeltaTime, ERenderCondition rc)
{
	FRenderLayer::DrawMesh(DeltaTime, rc);
}
