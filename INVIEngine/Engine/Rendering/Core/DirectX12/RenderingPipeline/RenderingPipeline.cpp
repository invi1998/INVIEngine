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

	GeometryMap.BuildFog();

	// 初始化动态cubeMap
	DynamicCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DCount());	// 构建根签名
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// 绑定根签名

	// 构建模型几何
	GeometryMap.Build();

	// 构建常量描述堆
	GeometryMap.BuildDescriptorHeap();

	// 构建CubeMap
	DynamicCubeMap.Build(XMFLOAT3{ 15.f, 12.f, 0.f });

	// 构建常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	// 构建材质常量缓冲区
	GeometryMap.BuildMaterialShaderResourceView();

	// 构建灯光常量缓冲区
	GeometryMap.BuildLightConstantBuffer();

	// 构建视口常量缓冲区
	GeometryMap.BuildViewportConstantBuffer();

	// 构建纹理贴图
	GeometryMap.BuildTextureConstBuffer();

	// 构建雾的常量缓冲区
	GeometryMap.BuildFogConstantBuffer();

	// 让各个渲染层级构建自己的PSO
	RenderLayerManage.BuildPSO();

}

void FRenderingPipeline::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	DynamicCubeMap.UpdateCalculations(delta_time, viewport_info);
	GeometryMap.UpdateCalculations(delta_time, viewport_info);
	RenderLayerManage.UpdateCaculations(delta_time, viewport_info);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.PreDraw(DeltaTime);

	DirectXRootSignature.PreDraw(DeltaTime);

	// 渲染动态CubeMap （预先渲染CubeMap，然后将预渲染好的CubeMap提交给shader做反射使用）
	DynamicCubeMap.PreDraw(DeltaTime);

	RenderLayerManage.PreDraw(DeltaTime);

}

void FRenderingPipeline::Draw(float DeltaTime)
{
	// 渲染主视口
	GeometryMap.DrawViewport(DeltaTime);

	// 渲染动态CubeMap
	DynamicCubeMap.Draw(DeltaTime);

	// 绘制不透明反射层
	RenderLayerManage.Draw(RENDER_LAYER_OPAQUE_REFLECT, DeltaTime);

	// CubeMap 覆盖原先被修改的CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	RenderLayerManage.Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_OPAQUE, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
	
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayerManage.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer)
{
	return GeometryMap.FindMeshRenderingDataByHash(hashKey, rendering_data, layer);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* my_mesh, const FRenderingData& rendering_data)
{
	GeometryMap.DuplicateMesh(my_mesh, rendering_data);
}
