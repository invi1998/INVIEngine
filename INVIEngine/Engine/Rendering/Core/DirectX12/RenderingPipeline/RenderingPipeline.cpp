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
	DirectXPipelineState.ResetGPSDesc();	// ����ͼ�ι�����Դ������

	// ��Ⱦ�㼶��ʼ��
	RenderLayerManage.Init(&GeometryMap, &DirectXPipelineState);

	RenderLayerManage.Sort();		// ��Ⱦ�㼶����

	// ����������ͼ
	GeometryMap.LoadTexture();

	GeometryMap.BuildFog();

	// ��ʼ����̬cubeMap
	DynamicCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// ��ʼ��AO
	SSAO.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	SSAO.SetBufferSize(512, 512);	// ����AO��������С

	// shadowMap ��ʼ��
	GeometryMap.DynamicShadowMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// ShadowCubeMap ��ʼ��
	GeometryMap.DynamicShadowCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// ��ʼ����ǩ��
	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DCount());	// ������ǩ��
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// �󶨸�ǩ��

	// ����ģ�ͼ���
	GeometryMap.Build();

	// ������̬�������
	GeometryMap.BuildDynamicReflectionMeshComponent();

	// ��������������
	GeometryMap.BuildDescriptorHeap();

	SSAO.BuildDescriptor();	// ����������

	// ��ʼ��UI����
	UiPipeline.Initialize(GeometryMap.GetHeap(), GeometryMap.GetDrawTexture2DCount() + GeometryMap.GetDrawCubeMapCount() + 1 + 1 + 1);

	// ��ʼ��CubeMap�����
	DynamicCubeMap.BuildViewPort(XMFLOAT3{ 0.f, 0.f, 0.f });

	// �������ģ��������
	DynamicCubeMap.BuildDepthStencilDescriptor();

	// ����CubeMap
	// DynamicCubeMap.Build(XMFLOAT3{ 15.f, 12.f, 0.f });

	// ����CubeMap��ȾĿ�� RTVDesc
	DynamicCubeMap.BuildCubeMapRenderTargetDescriptor();

	// �������ģ��
	DynamicCubeMap.BuildDepthStencil();

	// ������Ӱ
	GeometryMap.BuildShadow();

	// ��������������
	GeometryMap.BuildMeshConstantBuffer();

	// �������ʳ���������
	GeometryMap.BuildMaterialShaderResourceView();

	// �����ƹⳣ��������
	GeometryMap.BuildLightConstantBuffer();

	// �����ӿڳ���������
	GeometryMap.BuildViewportConstantBuffer();

	// ����������ͼ
	GeometryMap.BuildTextureConstBuffer();

	// ������ĳ���������
	GeometryMap.BuildFogConstantBuffer();

	// ������������ע���������Ĵ���һ��Ҫ�ڳ��������Ѵ���֮��
	SSAO.Build();

	// �洢һ��Ĭ�ϵ�GPS������
	DirectXPipelineState.SaveGPSDescAsDefault();

	// �ø�����Ⱦ�㼶�����Լ���PSO
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

	// ��Ⱦ���գ�������ͼ
	GeometryMap.Draw(DeltaTime);

	// ��ȾAO
	SSAO.Draw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);

	// ����SSAO��ָ����Buffer
	SSAO.SaveSSAOToBuffer();

	// ������ӿ�
	ClearMainSwapChainCanvas();

	// ���°���ͼ
	GeometryMap.DrawTexture2D(DeltaTime);

	// ��Ⱦ��Ӱ
	GeometryMap.DrawShadow(DeltaTime);

	// ��ȾShadowCubeMap
	GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	// �ж��Ƿ���ڶ�̬�������
	if (DynamicCubeMap.IsExitDynamicReflectionMesh())
	{
		// ��Ⱦ��̬CubeMap ��Ԥ����ȾCubeMap��Ȼ��Ԥ��Ⱦ�õ�CubeMap�ύ��shader������ʹ�ã�
		DynamicCubeMap.PreDraw(DeltaTime);
	}

	RenderLayerManage.PreDraw(DeltaTime);

}

void FRenderingPipeline::Draw(float DeltaTime)
{
	// ��Ⱦ���ӿ�
	GeometryMap.DrawViewport(DeltaTime);

	// CubeMap ����ԭ�ȱ��޸ĵ�CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	RenderLayerManage.Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_OPAQUE, DeltaTime);
	RenderLayerManage.Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);

	RenderLayerManage.Draw(RENDER_LAYER_SELECT, DeltaTime);

	// ����Ⱦ���������Ƭ
	RenderLayerManage.Draw(RENDER_LAYER_ROT_PLANE, DeltaTime);

	// ����Ⱦ�������
	RenderLayerManage.Draw(RENDER_LAYER_OPERATE, DeltaTime);

	// ��ȾUI
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
