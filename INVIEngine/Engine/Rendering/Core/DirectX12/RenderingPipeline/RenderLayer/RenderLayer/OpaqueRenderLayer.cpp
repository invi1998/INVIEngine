#include "EngineMinimal.h"
#include "OpaqueRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	RenderPriority = 2000;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_OPAQUE;
}

void FOpaqueRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// 绑定shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// 输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// 颜色，这里这个偏移是12字节，因为我们上面位置是3个元素，每个元素是4字节（32位），所以到了颜色这里就是 3*4 = 12字节的偏移了
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	 // 法线，这里这个偏移是28字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 加上颜色4个元素，所以到了法线这里，就得偏移 （3+4）*4 = 28字节的偏移了
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // 切线（U切线），这里这个偏移是32字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 加上颜色4个元素，再加上3个法线元素，到了切线这里，就得偏移 （3+4+3）*4 = 40字节的偏移了
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv坐标，这里这个偏移是52字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 加上颜色4个元素，再加上3个法线元素，再加上3个切线元素，到了uv这里，就得偏移 （3+4+3+3）*4 = 52字节的偏移了

	};

	// 绑定输入布局
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(EPipelineState::Solid);

	DirectXPipelineState->SetFillModle(true);

	DirectXPipelineState->BuildPipelineState(EPipelineState::Wireframe);
}

void FOpaqueRenderLayer::Draw(float deltaTime)
{
	DirectXPipelineState->ResetPSO();

	FRenderLayer::Draw(deltaTime);
}
