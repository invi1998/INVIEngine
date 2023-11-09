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

	DirectXRootSignature.BuildRootSignature(GeometryMap.GetDrawTextureCount());	// ������ǩ��
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// �󶨸�ǩ��

	// ��Ⱦÿ���㼶��shader
	RenderLayerManage.BuildShader();

	// ����ģ��
	GeometryMap.Build();

	// ����������
	GeometryMap.BuildDescriptorHeap();

	// ��������������
	GeometryMap.BuildMeshConstantBuffer();

	// �������ʳ���������
	GeometryMap.BuildMaterialShaderResourceView();

	// �����ƹⳣ��������
	GeometryMap.BuildLightConstantBuffer();

	// �����ӿڳ���������
	GeometryMap.BuildViewportConstantBuffer();

	// ����shader��Դ�ӿ� SRV
	GeometryMap.BuildTextureConstBuffer();

	// ����PSO����
	DirectXPipelineState.BuildParam();

	// ������Ⱦ����
	DirectXPipelineState.BuildPipelineState(EPipelineState::Wireframe);
	DirectXPipelineState.SetFillModle(false);

	DirectXPipelineState.BuildPipelineState(EPipelineState::Solid);
}

void FRenderingPipeline::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	GeometryMap.UpdateCalculations(delta_time, viewport_info);
	RenderLayerManage.UpdateCaculations(delta_time, viewport_info);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);
	RenderLayerManage.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);

	DirectXRootSignature.PreDraw(DeltaTime);

	GeometryMap.Draw(DeltaTime);
	RenderLayerManage.Draw(DeltaTime);

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
