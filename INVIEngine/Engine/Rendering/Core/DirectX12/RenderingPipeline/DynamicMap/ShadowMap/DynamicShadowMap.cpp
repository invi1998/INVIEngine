#include "EngineMinimal.h"
#include "DynamicShadowMap.h"

#include "Component/Mesh/Core/MeshComponentType.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
{
	CreateRenderTarget<FShadowMapRenderTarget>();
}

FDynamicShadowMap::~FDynamicShadowMap()
{
}

void FDynamicShadowMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicShadowMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);

	if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
	{
		// ������Դ״̬Ϊ��д
		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			innerRenderTarget->GetRenderTarget(),				// ��Դ
			D3D12_RESOURCE_STATE_GENERIC_READ,				// ��Դ״̬ �ɶ�
			D3D12_RESOURCE_STATE_DEPTH_WRITE);		// ��Դ״̬ תΪ ��д

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

		// ��Ҫÿ֡���µ����� �󶨾��ο�
		auto RenderTargetViewPort = innerRenderTarget->GetViewport();
		auto RenderTargetScissorRect = innerRenderTarget->GetScissorRect();

		GetD3dGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewPort);
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

		UINT CBVDescriptorSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

		// ������ģ�建����
		GetD3dGraphicsCommandList()->ClearDepthStencilView(			
			innerRenderTarget->CPUDepthStencilView,		// DSV ������
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,					// ������ģ�建����
			1.0f,											// ���ֵ
			0,												// ģ��ֵ
			0,												// �����������
			nullptr);										// �������)

		// ����ϲ���Ӱ��ͼ ����Ϊ���ﲻ���ǽ���������ɫ�����Բ���Ҫ������ȾĿ�꣨��ȾĿ������Ϊ0����
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			0,				// ��ȾĿ������
			nullptr,		// ƫ��
			false,			// �Ƿ���õ������������ȾĿ����ͼ�����ģ����ͼ
			&innerRenderTarget->CPUDepthStencilView);		// ָ�����ģ����ͼ

		// �������
		auto ViewportAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
		ViewportAddr += (1 + GeometryMap->GetDynamicViewportNum()) * CBVDescriptorSize;

		GetD3dGraphicsCommandList()->SetGraphicsRootShaderResourceView(		
				1,	// ����������ʼ����
				ViewportAddr	// GPU��Դ��ͼ
				);

		DrawShadowMapTexture(deltaTime);

		RenderLayers->ResetPSO(RENDER_LAYER_OPAQUE_SHADOW);

		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE);

		// ����Դ״̬�ɿ�дת��Ϊ�ɶ�
		CD3DX12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			innerRenderTarget->GetRenderTarget(),				// ��Դ
			D3D12_RESOURCE_STATE_DEPTH_WRITE,				// ��Դ״̬ ��д
			D3D12_RESOURCE_STATE_GENERIC_READ);		// ��Դ״̬ תΪ �ɶ�

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);
	}
}

void FDynamicShadowMap::SetViewportPosition(const XMFLOAT3& position)
{
	ShadowViewPort->SetPosition(position);
	BuildViewMatrix();
}

void FDynamicShadowMap::SetViewportRotation(const XMFLOAT3& rotation)
{
	ShadowViewPort->SetRotation(fvector_3d(rotation.x, rotation.y, rotation.z));
	BuildViewMatrix();
}

void FDynamicShadowMap::BuildViewPort(const XMFLOAT3& position)
{
	ShadowViewPort = CreateObject<GClientViewPort>(new GClientViewPort());
	ShadowViewPort->SetPosition(position);
	ShadowViewPort->FaceTarget(position, { 10.f, 0.f, 0.f });
	ShadowViewPort->SetFrustum(XM_PIDIV2, 1.f, 1.f, 0.1f, 10000.f);
	BuildViewMatrix();
}

void FDynamicShadowMap::BuildViewMatrix()
{
	ShadowViewPort->BuildViewMatrix();
}

void FDynamicShadowMap::BuildDepthStencilViewDesc()
{
	// �õ�DSV��������ƫ��ֵ
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
	{
		innerRenderTarget->CPUDepthStencilView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),		// ���ӿ�DSV���ڴ˻�����ƫ��
			1 + 1,													// 1�����ӿ�DSV + 1��CubeMap��DSV��ƫ�ƹ�������֮�󣬲�����Ӱ��DSV
			DescriptorHandleIncrementSize);							// ������ƫ��ֵ
	}
}

void FDynamicShadowMap::BuildShadowMapRenderTargetDescriptor()
{
	BuildRenderTargetSRV();

	RenderTarget->Init(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void FDynamicShadowMap::DrawShadowMapTexture(float DeltaTime)
{
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
			7,	// ����������ʼ����
			RenderTarget->GetGPUShaderResourceView()	// GPU��Դ��ͼ
		);
}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
	if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// ΪCubeMap����CPU shader��Դ��ͼ ��Ҫ�Ǵ���Shadow����������
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// ƫ���� Texture2D + CubeMap + DynamicCubeMap
			CBVDescriptorSize	// SRVƫ����
		);

		// ΪCubeMap����GPU shader��Դ��ͼ ����������Ⱦ��Ӱ��ͼ��ʱ����Ҫ�������Դ��ͼ�󶨵���ɫ����
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// ƫ����
			CBVDescriptorSize	// SRVƫ����
		);
	}
}
