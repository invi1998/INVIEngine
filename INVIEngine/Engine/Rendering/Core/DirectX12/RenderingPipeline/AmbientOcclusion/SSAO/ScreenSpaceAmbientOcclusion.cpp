#include "EngineMinimal.h"
#include "ScreenSpaceAmbientOcclusion.h"

#include "SSAOConstant.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

FScreenSpaceAmbientOcclusion::FScreenSpaceAmbientOcclusion(): RenderLayer(nullptr)
{
}

FScreenSpaceAmbientOcclusion::~FScreenSpaceAmbientOcclusion()
{
}

void FScreenSpaceAmbientOcclusion::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	RenderLayer = inRenderLayer;

	NormalBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
	AmbientBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FScreenSpaceAmbientOcclusion::SetBufferSize(int wid, int hei)
{
	NormalBuffer.SetBufferSize(wid, hei);
	AmbientBuffer.SetBufferSize(wid, hei);
}

void FScreenSpaceAmbientOcclusion::Build()
{
	SSAORootSignature.BuildRootSignature(1);		// ������ǩ��

	BuildSSAOConstantBufferView();	// ����SSAO����������ͼ

	// ��PSO��������
	BindBuildPso();
}

void FScreenSpaceAmbientOcclusion::BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc)
{
	// �󶨸�ǩ��
	OutPSODesc.pRootSignature = SSAORootSignature.GetRootSignature();
}

void FScreenSpaceAmbientOcclusion::BindBuildPso()
{
	if (RenderLayer)
	{
		if (std::shared_ptr<FRenderLayer> layer = RenderLayer->FindByRenderLayer(EMeshRenderLayerType::RENDER_LAYER_SSAO))
		{
			// ��ί��
			layer->BuildPsoDelegate.Bind(this, &FScreenSpaceAmbientOcclusion::BuildPSO);
		}
		
	}
}

void FScreenSpaceAmbientOcclusion::Draw(float DeltaTime)
{
	NormalBuffer.Draw(DeltaTime);
	AmbientBuffer.Draw(DeltaTime);
}

void FScreenSpaceAmbientOcclusion::DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info)
{
	FSSAOConstant SSAOConstant;
	// ���ӿ���Ϣ�л�ȡͶӰ����
	SSAOConstant.ProjectionMatrix = viewport_info.ProjectionMatrix;

	// ͶӰ����������
	XMMATRIX invProjection = XMLoadFloat4x4(&SSAOConstant.ProjectionMatrix);	// ����ͶӰ����
	XMVECTOR det = XMMatrixDeterminant(invProjection);	// ��������ʽ
	XMMATRIX invView = XMMatrixInverse(&det, invProjection);	// ���������
	XMStoreFloat4x4(&SSAOConstant.InversiveProjectionMatrix, XMMatrixTranspose(invView));		// ת������󲢴��볣������

	// [-1, 1] -> [0. 1]
	XMMATRIX halfLambertMatrix = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f };

	XMMATRIX textureMatrix = XMMatrixMultiply(invProjection, halfLambertMatrix);	// ͶӰ����Ͱ������ؾ�����˵õ�����ռ����NDC)
	XMStoreFloat4x4(&SSAOConstant.TextureProjectionMatrix, textureMatrix);			// ���볣������

	SSAOConstantBufferView.Update(0, &SSAOConstant);	// ���³�������
}

void FScreenSpaceAmbientOcclusion::UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info)
{
	NormalBuffer.UpdateCalculations(DeltaTime, viewport_info);
	AmbientBuffer.UpdateCalculations(DeltaTime, viewport_info);

	DrawSSAOConstantBuffer(DeltaTime, viewport_info);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptor()
{
	NormalBuffer.BuildDescriptor();
	NormalBuffer.BuildRenderTargetRTVOffset();
	NormalBuffer.BuildSRVDescriptor();
	NormalBuffer.BuildRTVDescriptor();

	AmbientBuffer.BuildDescriptor();
	AmbientBuffer.BuildRenderTargetRTVOffset();
	AmbientBuffer.BuildSRVDescriptor();
	AmbientBuffer.BuildRTVDescriptor();
}

void FScreenSpaceAmbientOcclusion::BuildSSAOConstantBufferView()
{
	SSAOConstantBufferView.CreateConstant(
		sizeof(FSSAOConstant),	// ���������С
		0,	// ��Ϊ������һ���µĳ�������, �������ǵ�ƫ������0
		false
	);
}
