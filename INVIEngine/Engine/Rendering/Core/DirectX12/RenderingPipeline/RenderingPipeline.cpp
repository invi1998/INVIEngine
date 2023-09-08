#include "RenderingPipeline.h"

#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/Core/ObjectTransformation.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(GMesh* Mesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(Mesh, MeshData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPipelineState.ResetGPSDesc();	// 重置图形管线资源描述符

	DirectXRootSignature.BuildRootSignature();	// 构建根签名
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// 绑定根签名

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建shader HLSL
	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_0");
	// 绑定shader
	DirectXPipelineState.BindShader(VertexShader, PixelShader);

	// 输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// 颜色，这里这个偏移是12字节，因为我们上面位置是3个元素，每个元素是4字节（32位），所以到了颜色这里就是 3*4 = 12字节的偏移了
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// 法线，这里这个偏移是28字节，因为我们上面位置是3个元素，每个元素是4字节（32位）, 加上颜色4个元素，所以到了法线这里，就得偏移 （3+4）*4 = 28字节的偏移了
	};

	// 绑定输入布局
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());

	// 构建模型
	GeometryMap.Build();

	// 构建描述堆
	GeometryMap.BuildDescriptorHeap();

	// 构建常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	// 构建材质常量缓冲区
	GeometryMap.BuildMaterialConstantBuffer();

	// 构建灯光常量缓冲区
	GeometryMap.BuildLightConstantBuffer();

	// 构建视口常量缓冲区
	GeometryMap.BuildViewportConstantBuffer();

	// 构建渲染管线
	DirectXPipelineState.BuildPipeline();
}

void FRenderingPipeline::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	GeometryMap.UpdateCalculations(delta_time, viewport_info);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);

	DirectXRootSignature.PreDraw(DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);

	GeometryMap.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
