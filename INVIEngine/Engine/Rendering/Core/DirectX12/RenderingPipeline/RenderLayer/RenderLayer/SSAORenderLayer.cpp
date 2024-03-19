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
	///构建shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/AO/SSAO.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/AO/SSAO.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// 绑定shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// 输入布局(这里我们不需要输入布局，因为我们不需要输入顶点信息，我们只需要屏幕空间的纹理坐标，而这些信息已经在GBuffer中了)
	InputElementDesc.clear();

	// 绑定输入布局
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FSSAORenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();	// 通过基类的代理，我们已经成功绑定了根签名

	auto psoDesc = DirectXPipelineState->GetGPSDesc();

	psoDesc.DepthStencilState.DepthEnable = false;	// 关闭深度测试（因为深度信息已经在GBuffer中了，已经由我们之前的法线信息NormalBuffer提供了）
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 深度写入掩码设置为0，不写入深度信息
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16_UNORM;	// 设置渲染目标格式
	psoDesc.SampleDesc.Count = 1;	// 设置采样数量
	psoDesc.SampleDesc.Quality = 0;	// 设置采样质量

	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;	// 设置深度模板格式(未知，因为我们不需要深度模板)

	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(SSA0);
}

void FSSAORenderLayer::Draw(float deltaTime)
{
	// 渲染之前，重置PSO
	ResetPSO();

	// 这里不需要执行基类的绘制操作，因为我们不需要输入顶点信息，我们只需要屏幕空间的纹理坐标，而这些信息已经在GBuffer中了，所以这里执行自定义的绘制操作
	GetD3dGraphicsCommandList()->IASetIndexBuffer(nullptr);	// 设置索引缓冲区
	GetD3dGraphicsCommandList()->IASetVertexBuffers(0, 0, nullptr);	// 设置顶点缓冲区
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 设置图元拓扑类型
	GetD3dGraphicsCommandList()->DrawInstanced(6, 1, 0, 0);	// 执行绘制操作

}

void FSSAORenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(SSA0);
}
