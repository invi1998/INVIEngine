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

	// shadowMap ��ʼ��
	GeometryMap.DynamicShadowMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayerManage);

	// ��ʼ����ǩ��
	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DCount());	// ������ǩ��
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// �󶨸�ǩ��

	// ����ģ�ͼ���
	GeometryMap.Build();

	// ������̬�������
	GeometryMap.BuildDynamicReflectionMeshComponent();

	// ��������������
	GeometryMap.BuildDescriptorHeap();

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

	// �ø�����Ⱦ�㼶�����Լ���PSO
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

	// ������ӿ�
	ClearMainSwapChainCanvas();

	// ��Ⱦ���գ�������ͼ
	GeometryMap.Draw(DeltaTime);

	// ��Ⱦ��Ӱ
	GeometryMap.DrawShadow(DeltaTime);

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
