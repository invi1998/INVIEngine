#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(Mesh, MeshData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPipelineState.ResetGPSDesc();	// ����ͼ�ι�����Դ������

	DirectXRootSignature.BuildRootSignature();	// ������ǩ��
	DirectXPipelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());	// �󶨸�ǩ��

	DirectXPipelineState.BuildPipeline();	// ������Ⱦ����
}
