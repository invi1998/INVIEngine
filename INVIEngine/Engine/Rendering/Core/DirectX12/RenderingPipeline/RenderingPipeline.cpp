#include "EngineMinimal.h"

#include "RenderingPipeline.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/Core/ObjectTransformation.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(meshHash, Mesh, MeshData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPipelineState.ResetGPSDesc();	// 重置图形管线资源描述符

	// 渲染层级初始化
	RenderLayerManage.Init(&GeometryMap, &DirectXPipelineState);

	RenderLayerManage.Sort();		// 渲染层级排序

	// 加载纹理贴图
	GeometryMap.LoadTexture();

	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTextureCount());	// 构建根签名
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// 绑定根签名

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建shader HLSL
	///

	char TextureNumBuff[10] = { 0 };

	// shader宏定义
	D3D_SHADER_MACRO ShaderMacro[] = {
		"TEXTURE2DNUM", _itoa(GeometryMap.GetDrawTextureCount(), TextureNumBuff, 10),
		NULL, NULL,
	};

	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_1", ShaderMacro);
	// 绑定shader
	DirectXPipelineState.BindShader(VertexShader, PixelShader);

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
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());

	// 构建模型
	GeometryMap.Build();

	// 构建描述堆
	GeometryMap.BuildDescriptorHeap();

	// 构建常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	// 构建材质常量缓冲区
	GeometryMap.BuildMaterialShaderResourceView();

	// 构建灯光常量缓冲区
	GeometryMap.BuildLightConstantBuffer();

	// 构建视口常量缓冲区
	GeometryMap.BuildViewportConstantBuffer();

	// 构建shader资源视口 SRV
	GeometryMap.BuildTextureConstBuffer();

	// 构建渲染管线
	DirectXPipelineState.BuildPipelineState();
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

	GeometryMap.Draw(DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
	
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data)
{
	return GeometryMap.FindMeshRenderingDataByHash(hashKey, rendering_data);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* my_mesh, const FRenderingData& rendering_data)
{
	GeometryMap.DuplicateMesh(my_mesh, rendering_data);
}
