#include "EngineMinimal.h"
#include "ScreenSpaceAmbientOcclusion.h"

#include "SSAOConstant.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

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

	// ����SSAO
	// GetD3dGraphicsCommandList()->SetGraphicsRootSignature(SSAORootSignature.GetRootSignature());
	SSAORootSignature.PreDraw(DeltaTime);

	// ��SSAO��Ⱦ
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(AmbientBuffer.GetRenderTarget().get()))
	{
		// ˢ�°󶨳�����������SSAO������������
		auto SSAOConstantBuffer = SSAOConstantBufferView.GetBuffer()->GetGPUVirtualAddress();
		GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(
			0,		// ��ǩ����0��λ��
			SSAOConstantBuffer);	// ������������ַ

		// �󶨷��ߣ�����֮ǰ�� NormalBuffer.Draw(DeltaTime); �Ѿ���Ⱦ��������Ҫ�ķ��ߣ�����ֻ��Ҫ�󶨣�
		if (std::shared_ptr<FRenderTarget> NormalBufferRenderTarget = NormalBuffer.GetRenderTarget())
		{
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
				3,	// ��ǩ����1��λ��
				NormalBufferRenderTarget->GetGPUShaderResourceView());
		}

		auto viewport = renderTarget->GetViewport();
		auto rect = renderTarget->GetScissorRect();

		// �����ӿ�
		GetD3dGraphicsCommandList()->RSSetViewports(1, &viewport);
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &rect);

		// ����Դ��һ��״̬ת������һ��״̬
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),	//	��ȡ��ȾĿ��
			D3D12_RESOURCE_STATE_GENERIC_READ,	//	�ӿɶ�״̬
			D3D12_RESOURCE_STATE_RENDER_TARGET);	//	����ȾĿ��״̬ ת��

		// �����ȾĿ��
		GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition);

		const float ClearColor[] = { 1.f, 1.f, 1.f, 1.f };	// ���ߵ�Ĭ��ֵ��(0,0,1)
		// �����ȾĿ����ͼ����ջ�����
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			renderTarget->GetCPURenderTargetView(),
			ClearColor,
			0,
			nullptr);

		// ������ģ�建������������Ϊ��֮ǰ�Ѿ���������ȣ��������ﲻ��Ҫ�����

		// �ϲ�״̬��û�����ֵ����nullptr
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,
			&renderTarget->GetCPURenderTargetView(),
			true,
			nullptr
		);

		// ��ȾSSAO
		RenderLayer->Draw(EMeshRenderLayerType::RENDER_LAYER_SSAO, DeltaTime);

		CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,	//	����Ⱦ״̬
			D3D12_RESOURCE_STATE_GENERIC_READ);	//	���ɶ�״̬ ת��

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition2);
	}
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
		1,	// ��Ϊ������һ���µĳ�������, �������ǵĶ���������1
		false
	);
}

void FScreenSpaceAmbientOcclusion::SaveSSAOToBuffer()
{
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		9,	// ��ǩ����9��λ��
		NormalBuffer.GetRenderTarget()->GetGPUShaderResourceView()
	);
}
