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

	// 初始化AO
	SSAO.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	SSAO.SetBufferSize(512, 512);	// 设置AO缓冲区大小

	// shadowMap 初始化
	GeometryMap.DynamicShadowMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// ShadowCubeMap 初始化
	GeometryMap.DynamicShadowCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// 初始化根签名
	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DCount());	// 构建根签名
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// 绑定根签名

	// 构建模型几何
	GeometryMap.Build();

	// 构建动态反射组件
	GeometryMap.BuildDynamicReflectionMeshComponent();

	// 构建常量描述堆
	GeometryMap.BuildDescriptorHeap();

	SSAO.BuildDescriptor();	// 构建描述符

	// 初始化UI管线
	UiPipeline.Initialize(GeometryMap.GetHeap(), GeometryMap.GetDrawTexture2DCount() + GeometryMap.GetDrawCubeMapCount() + 1 + 1 + 1);

	// 初始化CubeMap摄像机
	DynamicCubeMap.BuildViewPort(XMFLOAT3{ 0.f, 0.f, 0.f });

	// 构建深度模板描述符
	DynamicCubeMap.BuildDepthStencilDescriptor();

	// 构建CubeMap
	// DynamicCubeMap.Build(XMFLOAT3{ 15.f, 12.f, 0.f });

	// 构建CubeMap渲染目标 RTVDesc
	DynamicCubeMap.BuildCubeMapRenderTargetDescriptor();

	// 构建深度模板
	DynamicCubeMap.BuildDepthStencil();

	// 构建阴影
	GeometryMap.BuildShadow();

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

	// 创建描述符，注意描述符的创建一定要在常量描述堆创建之后
	SSAO.Build();

	// 存储一个默认的GPS描述符
	DirectXPipelineState.SaveGPSDescAsDefault();

	// 让各个渲染层级构建自己的PSO
	RenderLayerManage.BuildPSO();

}

void FRenderingPipeline::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	SSAO.UpdateCalculations(delta_time, viewport_info);
	GeometryMap.DynamicShadowCubeMap.UpdateCalculations(delta_time, viewport_info);
	DynamicCubeMap.UpdateCalculations(delta_time, viewport_info);
	GeometryMap.UpdateCalculations(delta_time, viewport_info);
	RenderLayerManage.UpdateCaculations(delta_time, viewport_info);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.PreDraw(DeltaTime);

	DirectXRootSignature.PreDraw(DeltaTime);

	// 渲染光照，材质贴图
	GeometryMap.Draw(DeltaTime);

	// 渲染AO
	SSAO.Draw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);

	// 保存SSAO到指定的Buffer
	SSAO.SaveSSAOToBuffer();

	// 清除主视口
	ClearMainSwapChainCanvas();

	// 重新绑定贴图
	GeometryMap.DrawTexture2D(DeltaTime);

	// 渲染阴影
	GeometryMap.DrawShadow(DeltaTime);

	// 渲染ShadowCubeMap
	GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	// 判断是否存在动态反射组件
	if (DynamicCubeMap.IsExitDynamicReflectionMesh())
	{
		// 渲染动态CubeMap （预先渲染CubeMap，然后将预渲染好的CubeMap提交给shader做反射使用）
		DynamicCubeMap.PreDraw(DeltaTime);
	}

	RenderLayerManage.PreDraw(DeltaTime);

}

void FRenderingPipeline::Draw(float DeltaTime)
{
	// 渲染主视口
	GeometryMap.DrawViewport(DeltaTime);

	// CubeMap 覆盖原先被修改的CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	RenderLayerManage.Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_OPAQUE, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);

	RenderLayerManage.Draw(RENDER_LAYER_SELECT, DeltaTime);

	// 先渲染操作句柄面片
	RenderLayerManage.Draw(RENDER_LAYER_ROT_PLANE, DeltaTime);

	// 再渲染操作句柄
	RenderLayerManage.Draw(RENDER_LAYER_OPERATE, DeltaTime);

	// 渲染UI
	UiPipeline.Draw(DeltaTime);

	DirectXPipelineState.Draw(DeltaTime);
	
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayerManage.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(size_t hashKey, std::shared_ptr<FRenderingData>& rendering_data, int layer)
{
	return GeometryMap.FindMeshRenderingDataByHash(hashKey, rendering_data, layer);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* my_mesh, std::shared_ptr<FRenderingData>& rendering_data)
{
	GeometryMap.DuplicateMesh(my_mesh, rendering_data);
}
