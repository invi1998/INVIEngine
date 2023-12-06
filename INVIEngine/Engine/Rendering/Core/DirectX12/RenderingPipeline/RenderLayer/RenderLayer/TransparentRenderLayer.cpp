#include "EngineMinimal.h"
#include "TransparentRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	RenderPriority = 3000;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_TRANSPARENT;
}

void FTransparentRenderLayer::BuildShader()
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

void FTransparentRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;
	RenderTargetBlendDesc.BlendEnable = true;		// 开启图层混合
	RenderTargetBlendDesc.LogicOpEnable = false;	// 关闭逻辑混合（开启混合必须将该值关闭，因为他两是互斥的）

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	// 设置源颜色和混合后颜色之间的乘法方式，设置为该值表示 将源颜色乘以源像素的 alpha 值。
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 表示目标颜色和混合后颜色之间的乘法方式。设置为该值表示 将目标像素的颜色值设置为源像素的透明度值取反（即 1 - 源像素的透明度值）。

	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// 表示混合操作的类型（加法、减法或反向减法），设置为该值表示 将源颜色和目标颜色相加。
	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// 表示源透明度和混合后透明度之间的乘法方式。设置为该值表示 将源像素的透明度值设置为 1。 F(1,1,1)
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// 表示目标透明度和混合后透明度之间的乘法方式。设置为该值表示 将源像素的透明度值设置为 0。F(0,0,0)
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 表示透明度混合操作的类型（加法、减法或反向减法）,设置为该值表示 将源颜色和目标颜色相加

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;		// 表示逻辑运算的类型（与、或、异或等）,设置为该值表示 不对输出颜色进行任何处理。
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// 表示需要写入当前渲染目标的哪些颜色通道，设置为该值表示 启用所有通道。

	// 设置渲染目标
	DirectXPipelineState->SetRenderTarget(0, RenderTargetBlendDesc);

	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(Transparent);
}

void FTransparentRenderLayer::Draw(float deltaTime)
{
	// 渲染之前，重置PSO
	ResetPSO();

	FRenderLayer::Draw(deltaTime);

}

void FTransparentRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(Transparent);
}
