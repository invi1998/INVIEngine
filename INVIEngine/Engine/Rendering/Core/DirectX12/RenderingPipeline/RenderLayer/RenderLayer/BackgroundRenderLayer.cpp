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
	///构建shader HLSL
	///

	char TextureNumBuff[10] = { 0 };

	// shader宏定义
	D3D_SHADER_MACRO ShaderMacro[] = {
		"TEXTURE2DNUM", _itoa(GeometryMap->GetDrawTexture2DCount(), TextureNumBuff, 10),
		"CUBE_MAP_NUM", _itoa(GeometryMap->GetDrawCubeMapCount(), TextureNumBuff, 10),
		NULL, NULL,
	};

	VertexShader.BuildShader(L"Shader/HLSL/Sky.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"Shader/HLSL/Sky.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
	// 绑定shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// 输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	 // 法线，这里这个偏移是12字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 所以到了法线这里，就得偏移 （3）*4 = 12字节的偏移了
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv坐标，这里这个偏移是24字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 再加上3个法线元素，到了uv这里，就得偏移 （3+3）*4 = 24字节的偏移了
	};

	// 绑定输入布局
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FBackgroundRenderLayer::UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FRenderLayer::UpdateCaculations(DeltaTime, ViewportInfo);
}

void FBackgroundRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	CD3DX12_RASTERIZER_DESC rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;		// 天空盒不希望设置裁剪
	DirectXPipelineState->SetRasterizerState(rasterizerDesc);

	CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;	// 设置深度模板测试为小于等于
	DirectXPipelineState->SetDepthStencilState(depthStencilDesc);


	DirectXPipelineState->BuildPipelineState(Background);
}
