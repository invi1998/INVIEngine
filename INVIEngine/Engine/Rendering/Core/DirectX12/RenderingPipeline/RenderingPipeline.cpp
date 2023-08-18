#include "RenderingPipeline.h"

#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/Core/ObjectTransformation.h"

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_0")
	// ��shader
	DirectXPipelineState.BindShader(VertexShader, PixelShader);

	// ���벼��
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// ��ɫ���������ƫ����12�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ�������Ե�����ɫ������� 3*4 = 12�ֽڵ�ƫ����
	};

	// �����벼��
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());

	// ����ģ��
	GeometryMap.Build();

	// ����������
	GeometryMap.BuildDescriptorHeap();

	// ��������������
	GeometryMap.BuildConstantBuffer();

	// �����ӿڳ���������
	GeometryMap.BuildViewportConstantBuffer();

	// ������Ⱦ����
	DirectXPipelineState.BuildPipeline();
}

void FRenderingPipeline::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	
}
