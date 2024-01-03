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
	///构建shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/Select.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/Select.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
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

void FSelectRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

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
